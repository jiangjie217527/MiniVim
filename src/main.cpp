#include<ncurses.h>
#include<fstream>
#include"fun.h"
#include"mod.h"
#include "cursor.h"
#include "prefix.h"
#include <vector>
#include <cstdio>

#define N 100001

int changeflg;
int max_y, Y, X;          //cur,which  start from 0
int op, sv, cr, de;
// Y ,X size of screen i.e. 28&118
//max_y lines of the text - 1 ,start from 0,i.e.
//show_y1 show_y2  lines I want to show started from 0
int show_y1, show_y2;
std::vector<int> tmp[N];//string
char *nm;
char dft[100] = {'a', '.', 'o'};
int nameid;

void readIn(char *argc);

int main(int argv, char *argc[]) {
    if (argv == 1) { //default filename
        argc[1] = dft;
        nameid = 1;
    } else if (argv == 2) {
        if (argc[1][0] == '-') {  //Vim -r
            nameid = 2;
            argc[2] = dft;
            if (argc[1][1] == 't' || argc[1][1] == 'T') {
                op = 1;
                changeflg = 1;
            } else if (argc[1][1] == 'r' || argc[1][1] == 'R') {
                op = 2;
            }
        } else {
            nameid = 1;   //vim a.o
        }
    } else {
        if (argc[1][1] == 't' || argc[1][1] == 'T') {
            op = 1;
            changeflg = 1;
        } else if (argc[1][1] == 'r' || argc[1][1] == 'R') {
            op = 2;
        }
        nameid = 2;    //vim -r a.o
    }
    nm = argc[nameid];

    initscr();//create the initial window
    raw();    //show what you type at once
    noecho(); //close echo
    getmaxyx(stdscr, Y, X);
    X -= 2;
    Y -= 2;
    WINDOW *FileWin = newwin(Y, X, 0, 2);
    WINDOW *InfWin = newwin(1, X + 2, Y, 0);
    WINDOW *ComWin = newwin(1, X + 2, Y + 1, 0);
    WINDOW *LNWin = newwin(Y, 2, 0, 0);
    refresh();
    init(FileWin, InfWin, ComWin, LNWin); //color


    std::ifstream Ti(argc[nameid]);  //  file does not exist
    if (!Ti)cr=changeflg=1;           //create the file
    Ti.close();

    std::ofstream T; //create file if not exist
    T.open(argc[nameid], std::ios::app);
    T.close();

    if (op != 1) readIn(argc[nameid]);
    initwordlist();
    cursor cur(0, 0, 1, 0);
    NormalMod(FileWin, InfWin, ComWin, LNWin, cur);

    //move mod
    if (sv)
        move(argc[nameid]);
    if (de)
        remove(argc[nameid]);
    //destroy all windows
    endwin();
    return 0;
}

void readIn(char *argc) {

    std::ifstream iFile;
    iFile.open(argc, std::ios::binary);

    char ch;
    int y = 0;
    while (!iFile.eof()) {
        iFile.get(ch);
        if (iFile.eof())break;
        if (ch == '\n') {
            y++;
        } else
            tmp[y].push_back(ch);
    }
    max_y = y;  // [0~max_y] with max_y+1 lines
    showlineX(0, 1);

    iFile.close();
}
