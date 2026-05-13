#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 200
#define MAX_NAME 64

typedef struct {
    int id;
    char name[MAX_NAME];
    float price;
} Item;

typedef struct {
    int menu_index;
    int qty;
} OrderEntry;

static int load_menu(const char *path, Item items[], int max) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[256];
    int n = 0;

    /* primeira linha do menu.txt é o total de itens, ignorar */
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }

    while (n < max && fgets(line, sizeof(line), f)) {
        char *space = strchr(line, ' ');
        char *colon = strchr(line, ':');
        if (!space || !colon || colon < space) continue;

        *space = '\0';
        *colon = '\0';

        char *name = space + 1;
        char *end = colon - 1;
        while (end > name && *end == ' ') { *end = '\0'; end--; }

        items[n].id = atoi(line);
        strncpy(items[n].name, name, MAX_NAME - 1);
        items[n].name[MAX_NAME - 1] = '\0';
        items[n].price = strtof(colon + 1, NULL);
        n++;
    }

    fclose(f);
    return n;
}

static int find_in_order(OrderEntry order[], int n_order, int menu_index) {
    for (int i = 0; i < n_order; i++) {
        if (order[i].menu_index == menu_index) return i;
    }
    return -1;
}

static void add_to_order(OrderEntry order[], int *n_order, int menu_index) {
    int idx = find_in_order(order, *n_order, menu_index);
    if (idx >= 0) {
        order[idx].qty++;
    } else {
        order[*n_order].menu_index = menu_index;
        order[*n_order].qty = 1;
        (*n_order)++;
    }
}

static void remove_from_order(OrderEntry order[], int *n_order, int menu_index) {
    int idx = find_in_order(order, *n_order, menu_index);
    if (idx < 0) return;
    if (order[idx].qty > 1) {
        order[idx].qty--;
        return;
    }
    for (int i = idx; i < *n_order - 1; i++) order[i] = order[i + 1];
    (*n_order)--;
}

static float order_total(Item items[], OrderEntry order[], int n_order) {
    float total = 0.0f;
    for (int i = 0; i < n_order; i++) {
        total += items[order[i].menu_index].price * order[i].qty;
    }
    return total;
}

static void draw_menu(WINDOW *win, Item items[], int n_items, int top, int sel) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Menu ");

    int h, w;
    getmaxyx(win, h, w);
    int rows = h - 2;
    int name_w = w - 16;
    if (name_w < 10) name_w = 10;

    for (int i = 0; i < rows && top + i < n_items; i++) {
        int idx = top + i;
        if (idx == sel) wattron(win, A_REVERSE);
        mvwprintw(win, i + 1, 1, " %3d %-*.*s %6.2f ",
                  items[idx].id, name_w, name_w,
                  items[idx].name, items[idx].price);
        if (idx == sel) wattroff(win, A_REVERSE);
    }
    wrefresh(win);
}

static void draw_order(WINDOW *win, Item items[], OrderEntry order[], int n_order) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Pedido ");

    int h, w;
    getmaxyx(win, h, w);
    int rows = h - 4;
    int name_w = w - 16;
    if (name_w < 10) name_w = 10;

    for (int i = 0; i < n_order && i < rows; i++) {
        Item *it = &items[order[i].menu_index];
        mvwprintw(win, i + 1, 1, " %2dx %-*.*s %6.2f ",
                  order[i].qty, name_w, name_w,
                  it->name, it->price * order[i].qty);
    }

    mvwhline(win, h - 3, 1, ACS_HLINE, w - 2);
    mvwprintw(win, h - 2, 2, "Total: R$ %.2f", order_total(items, order, n_order));
    wrefresh(win);
}

static void draw_status(WINDOW *win) {
    werase(win);
    mvwprintw(win, 0, 1,
              "[Up/Dn] navegar  [Enter] adicionar  [d] remover  [q] finalizar");
    wrefresh(win);
}

int main(void) {
    Item items[MAX_ITEMS];
    int n_items = load_menu("menu.txt", items, MAX_ITEMS);
    if (n_items <= 0) {
        fprintf(stderr, "Erro: nao foi possivel ler menu.txt\n");
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < 10 || cols < 60) {
        endwin();
        fprintf(stderr, "Terminal muito pequeno (minimo 60x10)\n");
        return 1;
    }

    int win_h = rows - 2;
    int menu_w = cols / 2;
    int order_w = cols - menu_w;

    WINDOW *menu_win   = newwin(win_h, menu_w, 0, 0);
    WINDOW *order_win  = newwin(win_h, order_w, 0, menu_w);
    WINDOW *status_win = newwin(2, cols, win_h, 0);

    OrderEntry order[MAX_ITEMS];
    int n_order = 0;
    int sel = 0;
    int top = 0;
    int rows_visible = win_h - 2;

    draw_status(status_win);
    draw_menu(menu_win, items, n_items, top, sel);
    draw_order(order_win, items, order, n_order);

    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q') {
        switch (ch) {
            case KEY_UP:
                if (sel > 0) sel--;
                if (sel < top) top = sel;
                break;
            case KEY_DOWN:
                if (sel < n_items - 1) sel++;
                if (sel >= top + rows_visible) top = sel - rows_visible + 1;
                break;
            case KEY_PPAGE:
                sel -= rows_visible;
                if (sel < 0) sel = 0;
                top = sel;
                break;
            case KEY_NPAGE:
                sel += rows_visible;
                if (sel >= n_items) sel = n_items - 1;
                if (sel >= top + rows_visible) top = sel - rows_visible + 1;
                break;
            case '\n':
            case KEY_ENTER:
            case ' ':
                add_to_order(order, &n_order, sel);
                break;
            case 'd':
            case 'D':
            case KEY_DC:
            case KEY_BACKSPACE:
                remove_from_order(order, &n_order, sel);
                break;
        }
        draw_menu(menu_win, items, n_items, top, sel);
        draw_order(order_win, items, order, n_order);
    }

    delwin(menu_win);
    delwin(order_win);
    delwin(status_win);
    endwin();

    printf("\nCHECK\n--------------------\n");
    for (int i = 0; i < n_order; i++) {
        Item *it = &items[order[i].menu_index];
        printf("%2dx %-30s %6.2f\n",
               order[i].qty, it->name, it->price * order[i].qty);
    }
    printf("--------------------\n");
    printf("The total value of the order is R$: %.2f\n",
           order_total(items, order, n_order));

    return 0;
}
