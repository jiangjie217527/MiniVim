#include<ncurses.h>
#include<fstream>
#include"fun.h"
#include <vector>
#define N 100001
//char Log[10]={'l','o','g','.','o'};
int max_y,Y,X;          //cur,which  start from 0
std::vector<int>tmp[N];//string
void dellin(int y){
    for(int i=y;i<max_y;i++){
        tmp[i].swap(tmp[i+1]);
    }
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
    for(int i=tmp[y].size();i>x;i--){
        tmp[y].pop_back();
    }
}
void readIn(WINDOW* FileWin, WINDOW* InfWin, WINDOW* LNWin,char *argc);
void typeIn(WINDOW* FileWin, WINDOW* InfWin, WINDOW* ComWin,WINDOW* LNWin);
void move(char *argc);// move the string from tmp to target

int main(int argv, char *argc[]) {
    if(argv==1){ //default filename
        argc[1]=new char[5];
        argc[1][0]='a';
        argc[1][1]='.';
        argc[1][2]='o';
    }

    initscr();//create the initial window
    raw();    //show what you type at once
    noecho(); //close echo
    int y,x;
    getmaxyx(stdscr,y,x);X=x-2;
    WINDOW *FileWin = newwin(y-2,x-2,0,2);
    WINDOW *InfWin = newwin(1,x,y-2,0);
    WINDOW *ComWin = newwin(1,x,y-1,0);
    WINDOW *LNWin  = newwin(y-2,2,0,0);
    refresh();
    init(FileWin,InfWin,ComWin,LNWin); //color

    //tmp stream
    readIn(FileWin, InfWin, LNWin, argc[1]);
    typeIn(FileWin, InfWin, ComWin,LNWin);

    //move mod
    move(argc[1]);

    //destroy all windows
    endwin();
    return 0;
}

void readIn(WINDOW* FileWin, WINDOW* InfWin,WINDOW* LNWin,char *argc){
    std::ofstream T;
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
    refreshLNWin(LNWin);
    refreshscreen(FileWin);
    refreshInfWIn(FileWin,InfWin,y,tmp[y].size());


    iFile.close();
}

void typeIn(WINDOW* FileWin, WINDOW* InfWin,WINDOW* ComWin,WINDOW* LNWin){
    int ch,y=max_y,x=tmp[y].size();int break_flg=0;
    while(true){
        ch= wgetch(FileWin);
        switch(ch){
            case KEY_UP:{
                if(y>0) {
                    y--;
                    if(x>tmp[y].size())
                        x=tmp[y].size();
                }
                break;
            }
            case KEY_DOWN:{
                if(y<max_y){
                    y++;
                    if(x>tmp[y].size())
                        x=tmp[y].size();
                }
                break;
            }
            case KEY_LEFT:{
                if(x>0)
                    x--;
                else {
                    if(y>0){
                        y--;
                        x=tmp[y].size();
                    }
                }
                break;
            }
            case KEY_RIGHT:{
                if(x<tmp[y].size())
                    x++;
                else {
                    if(y<max_y){
                        y++;
                        x=0;
                    }
                }
                break;
            }
            case ':':{
                if(ComMod(ComWin)){
                    break_flg=1;
                }
                break;
            }
            case KEY_BACKSPACE:{
                if(x==0){
                    if(y>0){
                        dellin(y);
                        y--;x=tmp[y].size();
                    }
                }
                else{
                    x--;
                    tmp[y].erase(tmp[y].begin()+x);
                }
                break;
            }
            default:{
                if(ch=='\n'){
                    addlin(y,x);
                    y++;x=tmp[y].size();
                }
                else {
                    if(x==tmp[y].size()){  //end
                        tmp[y].push_back(ch);
                    }
                    else { //inner
                        tmp[y].insert(tmp[y].begin()+x,ch);
                    }
                    x++;
                }
            }
        }
        if(break_flg)break;

        refreshLNWin(LNWin);
        refreshscreen(FileWin);
        refreshInfWIn(FileWin,InfWin,y,x);
    }
}
//note:we only need to delete and ignore something and I finally decided to use chain
//
void move(char *argc){
    std::ofstream oFile;
    oFile.open(argc);
    for(int i=0;i<=max_y;i++){
        for(int j=0;j<tmp[i].size();j++)
            oFile<<(char)tmp[i][j];
        if(i!=max_y)
            oFile<<'\n';
    }
    oFile.close();
}
