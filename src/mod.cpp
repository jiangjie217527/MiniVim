#include "mod.h"
#include "fun.h"
#include "cursor.h"
#include <vector>
#include <ncurses.h>
#include "prefix.h"

extern int max_y, X, Y, show_y1, show_y2, op, sv, cr, de;
extern std::vector<int> tmp[];
extern char *nm;
extern int changeflg;


bool ComMod(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin, cursor &cur) {
    static int st[100][100];
    static int len[100], lin = 0; //cursor x=len[lin]+1;
    int jmpflg = 0;
    refreshWin(FileWin, InfWin, LNWin, cur, 3);
    werase(ComWin);
    wprintw(ComWin, ":");
    wrefresh(ComWin);
    while (true) {
        if (jmpflg == 1) {
            wrefresh(FileWin);
            jmpflg = 0;
        }
        st[lin][len[lin]] = wgetch(ComWin);
        if (st[lin][len[lin]] == '\n') {
            if (st[lin][0] == 'q') {
                if (st[lin][1] != '!' && changeflg == 1) {
                    wclear(ComWin);
                    wprintw(ComWin,
                            "Detected you may have changed the file.save or not?You can choose only once.(y/n)");
                    wrefresh(ComWin);
                    st[lin][0] = wgetch(ComWin);
                    if (st[lin][0] == 'y' || st[lin][0] == 'Y')
                        sv = 1;
                    else if (cr == 1)de = 1;
                }
                if (st[lin][1] == '!' && cr == 1)de = 1;
                return true;
            }
            if (st[lin][0] == 'w') {
                move(nm);
                changeflg = 0;
                if (st[lin][1] == 'q')return true;
            }
            if (st[lin][0] == 'j' && st[lin][1] == 'm' && st[lin][2] == 'p') {
                int l = 0;
                for (int i = 4; st[lin][i] >= '0' && st[lin][i] <= '9'; i++) {
                    l = l * 10 + st[lin][i] - 48;
                    if (l > max_y) {
                        jmpflg = 1;
                        cur.y = max_y;
                        break;
                    }
                }
                l--;
                if (l >= 0 && l <= max_y) {
                    jmpflg = 1;
                    cur.y = l;
                }
            }
            lin++;
        } else if (st[lin][len[lin]] == 27)return false;
        else if (st[lin][len[lin]] == KEY_BACKSPACE && len[lin] > 0) {
            wmove(ComWin, 0, len[lin]);
            wdelch(ComWin);
            len[lin]--;
        } else if (st[lin][len[lin]] == KEY_UP) { if (lin > 0) lin--; }
        else if (st[lin][len[lin]] == KEY_DOWN) { if (len[lin] != 0) lin++; }
        else {
            wprintw(ComWin, "%c", st[lin][len[lin]]);
            len[lin]++;
        }
        wclear(ComWin);
        wprintw(ComWin, ":");
        for (int i = 0; i < len[lin]; i++)wprintw(ComWin, "%c", st[lin][i]);
        refreshWin(FileWin, InfWin, LNWin, cur, 3);
        wrefresh(ComWin);
    }
}

void InsertMod(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin, cursor &cur) {
    int ch;
    int flg = 0;
    refreshWin(FileWin, InfWin, LNWin, cur, 2);
    while (true) {
        if (flg == 0) { ch = wgetch(FileWin); }
        flg = 0;
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
            move_cursor(ch, cur);
        } else {
            if (ch != 27)changeflg = 1;
            switch (ch) {
                case 27:
                    return;
                case 9: {
                    for (int k = 0; k < 4; k++)insert(' ', cur);
                }
                case KEY_BACKSPACE: {
                    if (cur.x == 0 && cur.y > 0) {
                        int pre = (int) tmp[cur.y - 1].size();
                        for (int i = 0; i < tmp[cur.y].size(); i++)
                            tmp[cur.y - 1].push_back(tmp[cur.y][i]);
                        dellin(cur.y);

                        cur.up();
                        cur.pre_x = cur.x = pre;
                    } else if (cur.x > 0) {
                        cur.left();

                        tmp[cur.y].erase(tmp[cur.y].begin() + cur.x);
                        if (cur.x > 0) {
                            refreshWin(FileWin, InfWin, LNWin, cur, 2);
                            ch = complete(FileWin, ComWin, cur);
                            flg = 1;
                        }
                    }
                    break;
                }
                case 330: { //del
                    if (cur.x < tmp[cur.y].size()) { // with word del
                        tmp[cur.y].erase(tmp[cur.y].begin() + cur.x);
                    } else if (cur.y < max_y) {  //else move next line to now line
                        for (int i = 0; i < tmp[cur.y + 1].size(); i++)tmp[cur.y].push_back(tmp[cur.y + 1][i]);
                        dellin(cur.y + 1);
                    }
                    break;
                }
                case '\n': {
                    addlin(cur.y, cur.x);
                    cur.down();
                    cur.st();
                    break;
                }
                default: {
                    insert(ch, cur);
                    refreshWin(FileWin, InfWin, LNWin, cur, 2);
                    ch = complete(FileWin, ComWin, cur);
                    if (ch != 0)flg = 1;
                }
            }
        }
        refreshWin(FileWin, InfWin, LNWin, cur, 2);
    }
}

void NormalMod(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin, cursor &cur) {
    int ch = '\0', pre;
    int break_flg = 0;
    refreshWin(FileWin, InfWin, LNWin, cur, 1);
    while (true) {
        pre = ch;
        ch = wgetch(FileWin);
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT)
            move_cursor(ch, cur);
        else
            switch (ch) {
                case ':':
                    if (ComMod(FileWin, InfWin, ComWin, LNWin, cur))
                        break_flg = 1;
                    wclear(ComWin);
                    wrefresh(ComWin);
                    break;
                case 'i':
                    if (op != 2) {
                        cur.mode = 2;
                        cur.x = std::max(cur.x, 0);
                        InsertMod(FileWin, InfWin, ComWin, LNWin, cur);
                    }
                    cur.left();
                    cur.mode = 1;
                    break;
                case 'd':
                    if (pre == 'd' && op != 2) {
                        dellin(cur.y);
                        if (cur.y > max_y)cur.y--;
                        changeflg = 1;

                        cur.st();
                    }
                    break;
                case 'O':
                    cur.st();
                    break;
                case '$':
                    cur.pre_x = cur.x = (int) tmp[cur.y].size() - 1;
                    break;
                case 'w':
                    cur.w();
                    break;
                case 'b':
                    cur.b();
                    break;
                default:;
            }
        if (break_flg)break;
        refreshWin(FileWin, InfWin, LNWin, cur, 1);
    }
}
