# 🚚 SnackTruck

> A tiny terminal point-of-sale for your favorite snack truck.

![C](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)
![Built with ncurses](https://img.shields.io/badge/built%20with-ncurses-FF6F00)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey)
![Status](https://img.shields.io/badge/status-active-success)
![Made with love](https://img.shields.io/badge/made%20with-%E2%9D%A4-red)

---

## ✨ What it does

Loads a menu from `menu.txt` and opens a two-pane terminal UI where the
customer browses items, builds an order, and prints the final check.

```
┌──────────── Menu ────────────┐┌─────────── Pedido ──────────┐
│   1 Hot Dog            8.00  ││  2x Hot Dog          16.00  │
│ ▶ 2 Cheeseburger      14.00  ││  1x Soda              5.00  │
│   3 Soda               5.00  ││                             │
│   4 Fries              7.00  ││                             │
│   5 Milkshake          9.00  ││ ─────────────────────────── │
│                              ││  Total: R$ 21.00            │
└──────────────────────────────┘└─────────────────────────────┘
 [Up/Dn] navegar  [Enter] adicionar  [d] remover  [q] finalizar
```

> 📸 Drop a real screenshot at `docs/screenshot.png` and uncomment the
> line below to embed it.
> `<!-- ![SnackTruck screenshot](docs/screenshot.png) -->`

---

## 🚀 Quick start

```sh
cc snacktruck.c -o snacktruck -lncurses
./snacktruck
```

That's it — make sure you launch it from the directory that contains
`menu.txt`.

---

## 🛠️ Building

### Requirements

| Tool          | Notes                    |
|---------------|--------------------------|
| C compiler    | `cc`, `gcc`, or `clang`  |
| ncurses (dev) | system headers + library |

### Install ncurses

| OS               | Command                             |
|------------------|-------------------------------------|
| 🍎 macOS         | ships with Xcode Command Line Tools |
| 🐧 Debian/Ubuntu | `sudo apt install libncurses-dev`   |
| 🎩 Fedora        | `sudo dnf install ncurses-devel`    |
| 🏔️ Arch          | `sudo pacman -S ncurses`            |

### Compile

```sh
cc snacktruck.c -o snacktruck -lncurses
```

---

## 🎮 Controls

| Key                     | Action            |
|-------------------------|-------------------|
| ⬆️ / ⬇️                 | Navigate the menu |
| ⏫ PgUp / ⏬ PgDn       | Page through menu |
| ⏎ Enter / Space         | Add selected item |
| 🅳 `d` / Backspace / Del | Remove one unit   |
| 🆀 `q`                   | Finish and print  |

> Minimum terminal size: **60 columns × 10 rows**.

---

## 📋 Menu format

`menu.txt` starts with a line containing the number of items, followed by
one item per line:

```
<id> <name> : <price>
```

Example:

```
3
1 Hot Dog : 8.00
2 Cheeseburger : 14.00
3 Soda : 5.00
```

---

## 📂 Project layout

```
.
├── README.md
├── menu.txt        ← edit this to change the offering
└── snacktruck.c    ← the program
```

---

## 🧾 Sample output

After pressing `q`, the check is printed to stdout:

```
CHECK
--------------------
 2x Hot Dog                          16.00
 1x Soda                              5.00
--------------------
The total value of the order is R$: 21.00
```

---

Made with 🌭 and a terminal.
