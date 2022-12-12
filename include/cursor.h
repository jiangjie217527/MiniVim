#ifndef MINIVIM_CURSOR_H
#define MINIVIM_CURSOR_H

class cursor {

public:
    int x, y, mode, pre_x;

    cursor(int _y, int _x, int _m, int _pre) {
        y = _y;
        x = _x;
        mode = _m;
        pre_x = _pre;
    }

    void up();

    void down();

    void left();

    void right();

    void linelocation();

    void st();

    void b();

    void w();
};

#endif //MINIVIM_CURSOR_H
/*
 void move_cursor(int ch, int &y, int &x) {
    switch (ch) {
        case KEY_UP: {
            if (y > 0) {
                y--;
                if (x > (int) tmp[y].size() || (int) tmp[y].size() >= X)
                    x = (int) tmp[y].size();
            }
            if (y < show_y1) {
                show_y1--;
                if (show_y2 - show_y1 >= Y)show_y2--;
            }
            if (show_y2 > max_y)show_y2 = max_y;
            break;
        }
        case KEY_DOWN: {
            if (y < max_y) {
                y++;
                if (x > (int) tmp[y].size() || (int) tmp[y].size() >= X)
                    x = (int) tmp[y].size();
            }
            if (y > show_y2) {
                show_y2++;
                if (show_y2 - show_y1 >= Y)show_y1++;
            }
            break;
        }
        case KEY_LEFT: {
            if (x > 0)
                x--;
            else {
                if (y > 0) {
                    y--;
                    x = (int) tmp[y].size();
                }
            }
            break;
        }
        case KEY_RIGHT: {
            if (x < tmp[y].size())
                x++;
            else {
                if (y < max_y) {
                    y++;
                    x = 0;
                }
            }
            break;
        }
        default:
            break;
    }
}

void jmp(int l, int &y, int &x) {
    if (l < show_y1) {
        show_y2 -= (show_y1 - l);
        show_y1 = l;
    } else if (l > max_y) {
        y = max_y;
        x = (int) tmp[y].size();
        return;
    } else if (l > show_y2) {
        show_y1 += (l - show_y2);
        show_y2 = l;
    }
    y = l;
    x = (int) tmp[y].size();
}

void location(int &xx, int &yy, int x, int y) {
    xx = x;
    yy = 0;
    for (int i = show_y1; i < y; i++, yy++) {
        yy += (int) tmp[i].size() / X;
    }
    yy += (int) tmp[y].size() / X;
    xx = x % X;
}

 */