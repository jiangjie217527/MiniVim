#include <ncurses.h>
#include <vector>
#include "fun.h"
#include "cursor.h"
#include <fstream>

#define FILE_COLOR_NUM 1
#define INF_COLOR_NUM 2
#define COM_COLOR_NUM 3
#define LN_COLOR_NUM 4  //number of line win
extern int max_y, X, Y, show_y1, show_y2, op, sv, cr, de;
extern std::vector<int> tmp[];
extern char *nm;

void dellin(int y) {
    for (int i = y; i < max_y; i++) {
        tmp[i].swap(tmp[i + 1]);
    }
    tmp[max_y].clear();
    max_y--;
    if (show_y2 > max_y)show_y2--;
    if (y - 1 < show_y1) {
        show_y1--;
        if (show_y2 - show_y1 + 1 > Y)show_y2--;
    }
}

void addlin(int y, int x) {
    max_y++;
    if (show_y2 - show_y1 + 1 < Y)show_y2++;
    if (y + 1 > show_y2) {
        show_y2++;
        if (show_y2 - show_y1 + 1 > Y)show_y1++;
    }

    for (int i = max_y; i > y + 1; i--) {
        tmp[i].swap(tmp[i - 1]);
    }

    for (int i = x; i < tmp[y].size(); i++) {
        tmp[y + 1].push_back(tmp[y][i]);
    }
    int t = (int) tmp[y].size();
    for (int i = x; i < t; i++)
        tmp[y].pop_back();

}

void showlineX(int x, int mode) {
    show_y1 = show_y2 = x;
    if (mode == 1) {  //on top
        int l = (int) tmp[show_y2].size() / X + 1;
        while (l < Y && show_y2 < max_y) {  //if show_y2==Y means I have full the screen
            show_y2++;
            l += (int) tmp[show_y2].size() / X + 1;
        }
    }  //on button
    else if (mode == 2) {
        int l = (int) tmp[show_y1].size() / X + 1;
        while (l + (int) tmp[show_y1 - 1].size() / X + 1 <= Y && show_y1 > 0) {
            show_y1--;
            l += (int) tmp[show_y1].size() / X + 1;
        }
    }
}

void move_cursor(int ch, cursor &cur) {
    if (ch == KEY_UP)cur.up();
    if (ch == KEY_DOWN)cur.down();
    if (ch == KEY_LEFT)cur.left();
    if (ch == KEY_RIGHT)cur.right();
}

bool checkbutton(cursor &cur) {
    if (cur.y + cur.x / X > show_y2)return 1;
    else return false;
}

void refreshWin(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&LNWin, cursor &cur, int mode) {
    werase(FileWin);
    werase(InfWin);
    werase(LNWin);
    if (checkbutton(cur))showlineX(cur.y, 2);
    else if (cur.y < show_y1)showlineX(cur.y, 1);
    cursor src(0, std::max(cur.x, 0), 0, 0);
    for (int i = show_y1, src_y = 0; src_y < Y && i <= show_y2; i++) {
        mvwprintw(LNWin, src_y, 0, "%d", i + 1);
        if (i == cur.y)src.y = src_y;
        src_y += (int) tmp[i].size() / X + 1;
        for (const auto &k: tmp[i])wprintw(FileWin, "%c", k);
        wprintw(FileWin, "\n");
    }
//debug:, wprintw(InfWin,"show_y1=%d,show_y2=%d,src.y=%d,src.x=%d",show_y1,show_y2,src.y,src.x)
    if (mode == 1) {
        if (op != 2)   //normal
            wprintw(InfWin, "Mode:Normal,Line:%d,Col:%d ", cur.y + 1, cur.x + 1);   //printf l&c//
        else           //read only mode
            wprintw(InfWin, "Mode:Normal(Read-only),Line:%d,Col:%d", cur.y + 1, cur.x + 1);
    } else if (mode == 2) {  //insert
        wprintw(InfWin, "Mode:Insert,Line:%d,Col:%d", cur.y + 1, cur.x + 1);
    } else if (mode == 3) {  //command
        wprintw(InfWin, "Mode:Command Mode,(file)Line:%d,Col:%d", cur.y + 1, cur.x + 1);
    }
    src.linelocation();

    wrefresh(LNWin);
    wrefresh(InfWin);
    wmove(FileWin, src.y, src.x);
    wrefresh(FileWin);
}

void init(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin) {
    keypad(FileWin, true);
    keypad(ComWin, true);
    if (has_colors()) {//returns FALSE if the terminal does not support color.
        start_color();// initializes all the colors supported
        init_pair(FILE_COLOR_NUM, COLOR_WHITE, COLOR_BLACK);
        init_color(COLOR_BLUE, 200, 0, 500);
        init_pair(INF_COLOR_NUM, COLOR_WHITE, COLOR_BLUE);
        init_pair(COM_COLOR_NUM, COLOR_WHITE, COLOR_RED);
        init_color(COLOR_WHITE, 1000, 1000, 1000);
        init_pair(LN_COLOR_NUM, COLOR_YELLOW, COLOR_WHITE);
    }
    wbkgd(FileWin, COLOR_PAIR(FILE_COLOR_NUM));
    wbkgd(InfWin, COLOR_PAIR(INF_COLOR_NUM));
    wbkgd(ComWin, COLOR_PAIR(COM_COLOR_NUM));
    wbkgd(LNWin, COLOR_PAIR(LN_COLOR_NUM));
    wrefresh(FileWin);
    wrefresh(InfWin);
    wrefresh(ComWin);
    wrefresh(LNWin);
}

void insert(int ch, cursor &cur) {
    if (cur.x == tmp[cur.y].size())   //end
        tmp[cur.y].push_back(ch);
    else  //inner
        tmp[cur.y].insert(tmp[cur.y].begin() + cur.x, ch);
    cur.right();
}

void move(char *argc) {
    std::ofstream oFile;
    oFile.open(argc);
    for (int i = 0; i <= max_y; i++) {
        for (const auto &j: tmp[i])
            oFile << (char) j;
        if (i != max_y)oFile << '\n';
    }
    oFile.close();
}
