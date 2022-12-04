#include<ncurses.h>
#include<fstream>
#include"fun.h"
#include"mod.h"
#include <vector>
#include <cstdio>
#define N 100001
char dft[100]={'a','.','o'};
char *nm;
int max_y,Y,X;          //cur,which  start from 0
int nameid,op;
int sv,cr,de;
// Y ,X size of screen i.e. 28&118
//max_y lines of the text - 1 ,start from 0,i.e.
//show_y1 show_y2  lines I want to show started from 0
int show_y1,show_y2;

std::vector<int>tmp[N];//string

void readIn(WINDOW* FileWin, WINDOW* InfWin, WINDOW* LNWin,char *argc);

int main(int argv, char *argc[]) {
    if(argv==1){ //default filename
        argc[1]=dft;
        nameid=1;
    }
    else if(argv==2){
        if(argc[1][0]=='-'){  //Vim -r
            nameid=2;
            argc[2]=dft;
            if(argc[1][1]=='t'||argc[1][1]=='T'){
                op=1;
            }
            else if(argc[1][1]=='r'||argc[1][1]=='R'){
                op=2;
            }
        }
        else{
            nameid=1;   //vim a.o
        }
    }
    else {
        nameid=2;    //vim -r a.o
    }
    nm = argc[nameid];
    initscr();//create the initial window
    raw();    //show what you type at once
    noecho(); //close echo
    int y,x;
    getmaxyx(stdscr,y,x);
    X=x-2;  //lines of fileWin
    Y=y-2;
    WINDOW *FileWin = newwin(y-2,x-2,0,2);
    WINDOW *InfWin = newwin(1,x,y-2,0);
    WINDOW *ComWin = newwin(1,x,y-1,0);
    WINDOW *LNWin  = newwin(y-2,2,0,0);
    refresh();
    init(FileWin,InfWin,ComWin,LNWin); //color
    readIn(FileWin, InfWin, LNWin, argc[nameid]);
    if(op==1)
        NormalMod(FileWin, InfWin, ComWin,LNWin,0,0);
    else
        NormalMod(FileWin, InfWin, ComWin,LNWin,max_y,(int)tmp[max_y].size());

    //move mod
    if(sv)
        move(argc[nameid]);
    if(de)
        remove(argc[nameid]);
    //destroy all windows
    endwin();
    return 0;
}

void readIn(WINDOW* FileWin, WINDOW* InfWin,WINDOW* LNWin,char *argc){
    std::ifstream Ti(argc);
    if(!Ti)cr=1;
    Ti.close();
    std::ofstream T; //create file if not exist
    T.open(argc,std::ios::app);
    T.close();

    std::ifstream iFile;
    iFile.open(argc,std::ios::binary);

    char ch;
    int y=0;
    while(!iFile.eof()) {
        iFile.get(ch);
        if (iFile.eof())break;
        if (ch == '\n') {
            y++;
        } else {
            tmp[y].push_back(ch);
        }
    }
    max_y=y;
    if(op!=1) {
        show_y2 = max_y;
        if (max_y >= Y) { show_y1 = max_y - Y + 1; }
        else { show_y1 = 0; }
    }
    else {
        show_y1=0;
        show_y2=(max_y>=Y?Y:max_y);
    }
    refreshInfWin(FileWin,InfWin,y,(int)tmp[y].size(),1);
    refreshLNWin(FileWin,LNWin);


    iFile.close();
}