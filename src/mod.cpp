#include "mod.h"
#include "fun.h"
#include <vector>
#include <ncurses.h>
extern int max_y,X,Y,show_y1,show_y2,op,sv,cr,de;
extern std::vector<int>tmp[];
extern char *nm;
int changeflg;

bool ComMod(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin,int &y,int &x){
    static int st[100][100];static int len[100],lin=0;
    wprintw(ComWin,":");
    wrefresh(ComWin);
    while(true){
        st[lin][len[lin]]=wgetch(ComWin);
        if(st[lin][len[lin]]=='\n'){
            if(st[lin][0]=='q') {
                if(st[lin][1]!='!'){
                    if(changeflg==1){
                        wclear(ComWin);
                        wprintw(ComWin,"Detected you may have changed the file.save or not?You can choose only once.(y/n)");
                        wrefresh(ComWin);
                        st[lin][0]= wgetch(ComWin);
                        if(st[lin][0]=='y'||st[lin][0]=='Y')
                            sv=1;
                    }
                }
                if(st[lin][1]=='!'&&cr==1&&changeflg==0)de=1;
                return true;
            }
            if(st[lin][0]=='w'){
                move(nm);
                if(st[lin][1]=='q')return true;
            }
            if(st[lin][0]=='j'&&st[lin][1]=='m'&&st[lin][2]=='p'){
                int l=0;
                for(int i=4;st[lin][i]>='0'&&st[lin][i]<='9';i++)
                    l=l*10+st[lin][i]-48;
                jmp(l-1, y, x);
                refreshLNWin(FileWin,LNWin);
                refreshInfWin(FileWin,InfWin,y,x,3);
            }
            lin++;
        }
        else if(st[lin][len[lin]]==27){
            len[lin]--;
            return false;
        }
        else if(st[lin][len[lin]]==KEY_BACKSPACE&&len[lin] > 0) {
                wmove(ComWin, 0, len[lin]);
                wdelch(ComWin);
                len[lin]--;
        }
        else if(st[lin][len[lin]]==KEY_UP) {if(lin > 0) lin--; }
        else if(st[lin][len[lin]]==KEY_DOWN) {if(len[lin] != 0) lin++; }
        else {
            wprintw(ComWin, "%c", st[lin][len[lin]]);
            len[lin]++;
        }
        wclear(ComWin);
        wprintw(ComWin,":");
        for(int i=0;i<len[lin];i++)wprintw(ComWin,"%c",st[lin][i]);
        wrefresh(ComWin);
    }
}

void InsertMod(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin,int y,int x){
    int ch;int break_flg=0;refreshInfWin(FileWin, InfWin, y, x,2);
    while(true) {
        ch = wgetch(FileWin);
        if(ch==KEY_UP||ch==KEY_DOWN||ch==KEY_LEFT||ch==KEY_RIGHT){
            move_cursor(ch,y,x);
        }
        else {
            if(ch!=27)changeflg=1;
            switch (ch) {
                case 27:{  //esc
                    NormalMod(FileWin,InfWin,ComWin,LNWin,y,x);
                    break_flg=1;
                    break;
                }
                case KEY_BACKSPACE: {
                    if (x == 0 && y > 0) {
                        int pre = (int)tmp[y-1].size();
                        for(int i=0;i<tmp[y].size();i++)
                            tmp[y-1].push_back(tmp[y][i]);
                        dellin(y);
                        move_cursor(KEY_UP,y,x);
                        x=pre;
                    }
                    else {
                        x--;
                        tmp[y].erase(tmp[y].begin() + x);
                    }
                    break;
                }
                case 330:{
                    if(x<tmp[y].size()){
                        tmp[y].erase(tmp[y].begin() + x);
                    }
                    break;
                }
                case '\n': {
                    addlin(y, x);
                    move_cursor(KEY_DOWN,y,x);
                    break;
                }
                default:{
                    if (x == tmp[y].size())   //end
                        tmp[y].push_back(ch);
                    else  //inner
                        tmp[y].insert(tmp[y].begin() + x, ch);
                    x++;
                }
            }
        }
        if (break_flg)break;

        refreshLNWin(FileWin,LNWin);
        refreshInfWin(FileWin, InfWin, y, x,2);
    }
}

void NormalMod(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin,int y,int x){
    int ch='\0';int break_flg=0;refreshInfWin(FileWin,InfWin,y,x,1);
    int pre;
    while(true){
        pre=ch;
        ch= wgetch(FileWin);
        if(ch==KEY_UP||ch==KEY_DOWN||ch==KEY_LEFT||ch==KEY_RIGHT)
            move_cursor(ch,y,x);
        else
            switch(ch){
                case ':':
                    refreshInfWin(FileWin,InfWin,y,x,3);
                    if(ComMod(FileWin, InfWin, ComWin, LNWin,y,x)){
                        break_flg=1;
                    }
                    wclear(ComWin);
                    wrefresh(ComWin);
                    break;
                case 'i':
                    if(op!=2) {
                        InsertMod(FileWin, InfWin, ComWin, LNWin, y, x);
                        break_flg = 1;
                    }
                    break;
                case 'd':
                    if(pre=='d'){
                        dellin(y);changeflg=1;
                        move_cursor(KEY_UP,y,x);
                        x=(int)tmp[y].size();
                    }break;
                case 'O':x=0;break;
                case '$':x=(int)tmp[y].size();break;
                case 'w':while(x>0&&tmp[y][x]!=' ')x--;if(x>0)x--;break;
                case 'b':while(x<tmp[y].size()&&tmp[y][x]!=' ')x++;if(x<tmp[y].size())x++;break;
                default:break;
            }
        if(break_flg)break;
        refreshLNWin(FileWin,LNWin);
        refreshInfWin(FileWin,InfWin,y,x,1);
    }
}