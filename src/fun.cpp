#include <ncurses.h>
#include <vector>
#include "fun.h"
#include <fstream>
#define FILE_COLOR_NUM 1
#define INF_COLOR_NUM 2
#define COM_COLOR_NUM 3
#define LN_COLOR_NUM 4  //number of line win
extern int max_y,X,Y,show_y1,show_y2,op,sv,cr,de;
extern std::vector<int>tmp[];
extern char *nm;
void dellin(int y){
    for(int i=y;i<max_y;i++) {
        tmp[i].swap(tmp[i + 1]);
    }
    tmp[max_y].clear();
    max_y--;
}
void addlin(int y,int x){
    max_y++;
    tmp[max_y].resize(1);
    for(int i=max_y;i>y+1;i--){
        tmp[i].swap(tmp[i-1]);
    }
    tmp[y+1].clear();

    for(int i=x;i<tmp[y].size();i++){
        tmp[y+1].push_back(tmp[y][i]);
    }
    for(int i=(int)tmp[y].size();i>x;i--){
        tmp[y].pop_back();
    }
}
void refreshLNWin(WINDOW* FileWin, WINDOW* &LNWin){
    wclear(LNWin);
    wclear(FileWin);
    wmove(FileWin,0,0);
    for(int i=show_y1,j=0;j<=Y-1&&i<=show_y2;i++,j++){
        mvwprintw(LNWin,j,0,"%d",i+1);
        j+=(int)tmp[i].size()/X;
        for(const auto &k :tmp[i]){
            wprintw(FileWin,"%c",k);
        }
        wprintw(FileWin,"\n");
    }
    wrefresh(LNWin);
    wrefresh(FileWin);
}

void jmp(int l,int &y,int &x){
    if(l<show_y1){
        show_y2-=(show_y1-l);
        show_y1=l;
    }
    else if(l>max_y){
        y=max_y;
        x=(int)tmp[y].size();
        return ;
    }
    else if(l>show_y2){
        show_y1+=(l-show_y2);
        show_y2=l;
    }
    y=l;
    x=(int)tmp[y].size();
}

void location(int &xx,int &yy,int x,int y){
    xx=x;
    yy=0;
    for(int i=show_y1;i<y;i++,yy++){
        yy+=(int)tmp[i].size()/X;
    }
    yy+=(int)tmp[y].size()/X;
    xx=x%X;
}

void refreshInfWin(WINDOW* &FileWin,WINDOW* &InfWin,int y,int x,int mode){
    wclear(InfWin);
    if(mode==1) {
        if(op!=2)
            wprintw(InfWin, "Mode:Normal,Line:%d,Col:%d", y + 1, x + 1);   //printf l&c//
        else
            wprintw(InfWin, "Mode:Normal(Read-only),Line:%d,Col:%d", y + 1, x + 1);
    }
    else if(mode == 2) {
        wprintw(InfWin, "Mode:Insert,Line:%d,Col:%d", y + 1, x + 1);
    }
    else if(mode == 3){
        wprintw(InfWin, "Mode:Command Mode,(file)Line:%d,Col:%d", y + 1, x + 1);
    }
    wrefresh(InfWin);
    int xx,yy;
    location(xx,yy,x,y);
    wmove(FileWin,yy,xx);
    wrefresh(FileWin);
}


void init(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin){
    keypad(FileWin, true);
    keypad(ComWin, true);
    if (has_colors()) {//returns FALSE if the terminal does not support color.
        start_color();// initializes all the colors supported
        init_pair(FILE_COLOR_NUM, COLOR_WHITE, COLOR_BLACK);
        init_color(COLOR_BLUE,200,0,500);
        init_pair(INF_COLOR_NUM, COLOR_WHITE,COLOR_BLUE);
        init_pair(COM_COLOR_NUM, COLOR_WHITE,COLOR_RED);
        init_color(COLOR_WHITE,1000,1000,1000);
        init_pair(LN_COLOR_NUM , COLOR_YELLOW,COLOR_WHITE);
    }
    wbkgd(FileWin, COLOR_PAIR(FILE_COLOR_NUM));
    wbkgd(InfWin , COLOR_PAIR(INF_COLOR_NUM));
    wbkgd(ComWin,  COLOR_PAIR(COM_COLOR_NUM));
    wbkgd(LNWin , COLOR_PAIR(LN_COLOR_NUM));
    wrefresh(FileWin);
    wrefresh(InfWin);
    wrefresh(ComWin);
    wrefresh(LNWin);
}

void move_cursor(int ch,int &y,int &x){
    switch (ch) {
        case KEY_UP: {
            if (y > 0) {
                y--;
                if (x > (int)tmp[y].size()||(int)tmp[y].size()>=X)
                    x = (int)tmp[y].size();
            }
            if(y<show_y1){
                show_y1--;
                if(show_y2-show_y1>=Y)show_y2--;
            }
            if(show_y2>max_y)show_y2=max_y;
            break;
        }
        case KEY_DOWN: {
            if (y < max_y) {
                y++;
                if (x > (int)tmp[y].size()||(int)tmp[y].size()>=X)
                    x = (int)tmp[y].size();
            }
            if(y>show_y2){
                show_y2++;
                if(show_y2-show_y1>=Y)show_y1++;
            }
            break;
        }
        case KEY_LEFT: {
            if (x > 0)
                x--;
            else {
                if (y > 0) {
                    y--;
                    x = (int)tmp[y].size();
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
        default:break;
    }
}

void move(char *argc){
    std::ofstream oFile;
    oFile.open(argc);
    for(int i=0;i<=max_y;i++){
        for(const auto &j :tmp[i])
            oFile<<(char)j;
        if(i!=max_y)
            oFile<<'\n';
    }
    oFile.close();
}