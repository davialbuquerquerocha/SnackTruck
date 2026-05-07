#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int secver = 1;
void verification(int a[100], int c){
    if (a[0] > 100){
        printf("We dont have this, why don't you pick something else? (item number)\n");
        scanf("%d", a);

        c++;
        if (c == 5){
// it's prohibiten to compute on this line
            printf("wrong answer or Rademaker tried to break the program\n");
            exit(1);
        }//verification

        verification(a, c);
    }
}

int costomer(int a[100]){
    int c; //truth or false  
    char s[5]; //string for user answer
    int it = 0; 
    

    do {
        printf("\nwhat would you like to order? (item number)\n");
        scanf("%d", a);
        
        verification(a, secver);

        printf("\nwould you want to order something else? ('yes' or 'no')\n");
        scanf("%s", s);

        if(s[0] == 'y' || s[0] == 'Y'){
            c = 0;
            }
        if(s[0] == 'n' || s[0] == 'N'){
            c = 1;
        }

        if (s[0] != 'n' && s[0] != 'y' && s[0] != 'N' && s[0] != 'Y'){
            printf("yes or no Rademaker, not gonna fall for this\n");
            exit(1);
        }

        a++;
        it++;
    } while(c == 0);

    return it;
}

void each_item(FILE *file, int alf[100], int c, char prod[][50]){
    
    int d;
    char linha[100];
    for (int i = 0; i < c; i++){
        int target = alf[i];
        rewind(file);
        
        while (fgets(linha, sizeof(linha), file)){
            sscanf(linha, "%d", &d);
            if (strchr(linha, ':') != NULL){
                if (d == target){
                    strcpy(prod[i], linha);
                    break;
                }    
            }
        }
    }
}

void total(char prod[][50], int c, float *f){
    for(int i = 0; i < c; i++){
        int index = strcspn(prod[i], ":");
        index++;
        
        char *ptr_fim;

        float fl = strtof(&prod[i][index], &ptr_fim);
        *f = *f + fl;
    }
}

int main (void){

    printf("take a look at our menu\n");
    FILE *archive;

        archive = fopen("menu.txt","r");

        if (archive == NULL){
            printf("error, can't open file");
            return 1;
        }

        fseek(archive, 0, SEEK_END);
        long size = ftell(archive); //archive size
        rewind(archive); 
        char line[size + 1];
    
        while(fgets(line, (int)(size + 1), archive) != NULL){
            printf("%s", line);
        }
        printf("\n");
        rewind(archive);

        float s = 0;
        float *price = &s;
        int alfa[100]; //cardinal number of each item
        int itens = costomer(alfa); //number of itens
        
        char eachitem[itens][50]; //item and order
        memset(eachitem, 0, sizeof(eachitem));

        each_item(archive, alfa, itens, eachitem);

        total(eachitem, itens, price);

        printf("\n\nCHECK\n--------------------\n");
        for (int i = 0; i < itens; i++){
            int h = 3;
            int *p = &h;
            if (eachitem[i][2] == '0'){
                h = 4;
            }
            printf("%s", eachitem[i] + h);
        }
        printf("\n--------------------");
        printf("\nThe total value of the order is R$: %f\n", s);

        fclose(archive);

    return 0;
}
