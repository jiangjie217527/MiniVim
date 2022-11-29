#include <ncurses.h>
#include <vector>
#define FILE_COLOR_NUM 1
#define INF_COLOR_NUM 2
#define COM_COLOR_NUM 3
#define LN_COLOR_NUM 4  //number of line win
extern int max_y,X;
extern std::vector<int>tmp[];
void refreshLNWin(WINDOW* &LNWin){
    wclear(LNWin);
    for(int i=0,j=0;i<=max_y;i++,j++){
        mvwprintw(LNWin,j,0,"%d",i+1);
        j+=tmp[i].size()/X;
    }
    wrefresh(LNWin);
}

bool ComMod(WINDOW* &ComWin){
    char st[100];int len=0;
    wprintw(ComWin,":");
    wrefresh(ComWin);
    while(true){
        st[len]=getch();
        if(st[len]=='\n'){
            if(st[0]=='q')return true;
            else return false;
        }
        wprintw(ComWin,"%c",st[len]);
        len++;
        wrefresh(ComWin);
    }
}

void refreshInfWIn(WINDOW* &FileWin,WINDOW* &InfWin,int y,int x){
    wclear(InfWin);
    wprintw(InfWin,"Line:%d,Col:%d",y+1,x+1);   //printf l&c
    wrefresh(InfWin);
    wmove(FileWin,y,x);
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

void refreshscreen(WINDOW* &FileWin){
    wclear(FileWin);
    wmove(FileWin,0,0);

    for(int i=0;i<=max_y;i++){
        for(int j=0;j<tmp[i].size();j++){
                wprintw(FileWin,"%c",tmp[i][j]);
        }
        wprintw(FileWin,"\n");
    }
}