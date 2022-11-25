#include <ncurses.h>
extern int max_y,x,y,colNum[];
void refreshLNWin(WINDOW* LNWin){
    wclear(LNWin);
    for(int i=0;i<=max_y;i++){
        mvwprintw(LNWin,i,0,"%d",i+1);
    }
    wrefresh(LNWin);
}

bool ComMod(WINDOW* ComWin){
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

void move_cursor(){
    char ch;
    ch=getch();
    switch (ch){
        case 'A':{ //up
            if(y>0) {
                y--;
                if(x>colNum[y])x=colNum[y];
            }
            break;
        }
        case 'B':{//down
            if(y<max_y){
                y++;
                if(x>colNum[y])x=colNum[y];
            }
            break;
        }
        case 'C':{//right
            if(x<colNum[y])x++;
            break;
        }
        case 'D':{ //left
            if(x>0)x--;
            break;
        }
        default :return;
    }
}