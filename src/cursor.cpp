#include "cursor.h"
#include <vector>

extern int show_y1, show_y2, max_y, X, Y;
extern std::vector<int> tmp[];

void cursor::up() {
    if (y == 0)return;
    y--;

    if (mode == 1) {
        if (pre_x > (int) tmp[y].size() - 1) {
            x = std::max((int) tmp[y].size() - 1, 0);
        } else x = pre_x;
    }
    if (mode == 2) {
        if (pre_x > tmp[y].size()) {
            x = (int) tmp[y].size();
        } else x = pre_x;
    }
}

void cursor::down() {
    if (y == max_y)return;
    y++;

    if (mode == 1) {
        if (pre_x > (int) tmp[y].size() - 1) {
            x = std::max((int) tmp[y].size() - 1, 0);
        } else x = pre_x;
    }
    if (mode == 2) {
        if (pre_x > tmp[y].size()) {
            x = (int) tmp[y].size();
        } else x = pre_x;
    }

}

void cursor::left() {
    if (x <= 0)return;
    x--;
    pre_x = x;
}

void cursor::right() {
    if (mode == 1) {
        if (x < (int) tmp[y].size() - 1)
            x++;
    }
    if (mode == 2) {
        if (x < (int) tmp[y].size())
            x++;
    }
    pre_x = x;
}

void cursor::linelocation() {
    y += x / X;
    if (x >= X)x %= X;
}

void cursor::st() {  //location the cursor to the start
    pre_x = x = 0;
}

void cursor::b() {
    if (mode == 1)while (x < tmp[y].size() - 1 && tmp[y][x] != ' ') right();
    else if (mode == 2) {
        while (x < tmp[y].size() && tmp[y][x] != ' ') right();
    }
    while (tmp[y][x] == ' ')right();
}

void cursor::w() {
    while (x > 0 && tmp[y][x] != ' ')left();
    if (x > 0)left();
    while (x > 0 && tmp[y][x] == ' ')left();
    while (x > 0 && tmp[y][x - 1] != ' ')left();
}
