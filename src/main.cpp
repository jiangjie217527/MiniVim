#include<ncurses.h>
#include<fstream>
#include<cstdio> //delete file
#include"fun.h"
//LINES=30 COLS=120 esc=27
#define FILE_COLOR_NUM 1
#define INF_COLOR_NUM 2
#define COM_COLOR_NUM 3
#define LN_COLOR_NUM 4  //number of line win

int fileLen;
struct chain{
    char ch;
    chain *nxt,*pre;
}*file;
char tmp[10]={'_','_','t','m','p','.','o'};//file name
//char Log[10]={'l','o','g','.','o'};
int x,y,max_y;          //cur
int colNum[10001];//number of char in line y and the input position

void init(WINDOW* FileWin,WINDOW* InfWin,WINDOW* ComWin,WINDOW* LNWin);
void readIn(WINDOW* FileWin, WINDOW* InfWin, WINDOW* LNWin,char *argc, std::ofstream &oTmp);
void typeIn(WINDOW* FileWin, WINDOW* InfWin, WINDOW* ComWin,WINDOW* LNWin,std::ofstream &oTmp);
void move(char *argc);// move the string from tmp to target



void refreshFileWIn(WINDOW* FileWin,WINDOW* InfWin){
    wclear(InfWin);
    wprintw(InfWin,"Line:%d,Col:%d",y+1,x+1);   //printf l&c
    wrefresh(InfWin);
    wmove(FileWin,y,x);
    wrefresh(FileWin);
}

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
    getmaxyx(stdscr,y,x);
    WINDOW *FileWin = newwin(y-2,x,0,2);
    WINDOW *InfWin = newwin(1,x,y-2,0);
    WINDOW *ComWin = newwin(1,x,y-1,0);
    WINDOW *LNWin  = newwin(y-2,2,0,0);
    refresh();
    init(FileWin,InfWin,ComWin,LNWin); //color

    //tmp stream
    std::ofstream oTmp;
    oTmp.open(tmp);

    //in mod
    readIn(FileWin, InfWin, LNWin, argc[1], oTmp);

    //out mod
    typeIn(FileWin, InfWin, ComWin,LNWin,oTmp);

    //move mod
    move(argc[1]);
    remove(tmp);

    //destroy all windows
    endwin();
    return 0;
}

void init(WINDOW* FileWin,WINDOW* InfWin,WINDOW* ComWin,WINDOW* LNWin){
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

void readIn(WINDOW* FileWin, WINDOW* InfWin,WINDOW* LNWin,char *argc, std::ofstream &oTmp){
    std::ifstream iFile;
    iFile.open(argc,std::ios::binary);

    char ch;
    getyx(FileWin,y,x);
    while(!iFile.eof()) {
        iFile.get(ch);
        if (iFile.eof())break;
        if (ch == '\n') {
            y++;x = 0;
        } else {
            mvwprintw(FileWin, y, x, "%c", ch);
            x++;colNum[y]++;
        }
        oTmp << ch;
        fileLen++;
    }
    max_y=y;
    refreshLNWin(LNWin);
    wclear(InfWin);
    wprintw(InfWin,"Line:%d,Col:%d",y+1,x+1);   //printf l&c
    wmove(FileWin,y,x);
    wrefresh(InfWin);
    wrefresh(FileWin);
    iFile.close();

}

void typeIn(WINDOW* FileWin, WINDOW* InfWin,WINDOW* ComWin,WINDOW* LNWin, std::ofstream &oTmp){
    char ch;int esc_flg=1;
    while(true){
        if(esc_flg)
            ch=getch();
        else
            esc_flg=1;
        if(ch==27){
            if((ch=getch())!='[')
                esc_flg=0;
            else{
                move_cursor();
                refreshFileWIn(FileWin,InfWin);
            }
            continue;
        }
        else if(ch==8||ch==127){
            int flg=1;
            getyx(FileWin,y,x);

            if(x==0){
                flg=0;y--;max_y--;
                x=colNum[y];
            }
            else{
                x--;wmove(FileWin,y,x);
            }
            if(flg){
                wdelch(FileWin);
                colNum[y]--;
            }
        }
        else if(ch==58){
            if(ComMod(ComWin)){
                break;
            }
        }
        else{
            wprintw(FileWin,"%c",ch);
            if(ch!='\n'){
                x++;colNum[y]++;
            }
            else {
                y++;x=0;
                max_y++;
            }
        }

        oTmp<<ch;
        fileLen++;

        refreshLNWin(LNWin);
        refreshFileWIn(FileWin,InfWin);
    }
    oTmp.close();
}
//note:we only need to delete and ignore something and I finally decided to use chain
//
void move(char *argc){  //modified!!!
    std::ofstream oFile;
    oFile.open(argc);
    std::ifstream iTmp;
    iTmp.open(tmp,std::ios::binary);

    file=new chain[fileLen+5];
    if(fileLen<=1){
        char ch;
        iTmp>>ch;
        if(ch!=127&&ch!=8)
            oFile<<ch;
    }
    else {
        for (int i = 1; i <= fileLen; i++) {
            file[i].nxt = file + i + 1;
            file[i].pre = file + i - 1;
        }
        file->nxt = file + 1;
        file[fileLen+1].pre = file + fileLen ;

        struct chain *p = file+1;
        while (!iTmp.eof()) {
            iTmp.get(p->ch);
            if (iTmp.eof())break;
            p = p->nxt;
        }
        p->ch='\0';
        for (p=file->nxt; p->ch!='\0'; p=p->nxt) {
            if (p-> ch == 8 || p->ch == 127) {
                    if(p->pre == file) {
                        file->nxt=p->nxt;
                        p->nxt->pre=file;
                    }
                    else {
                        p->pre->pre->nxt = p->nxt;
                        p->nxt->pre      =p->pre->pre;
                    }
            }
        }
        for (p=file->nxt; p->ch!='\0'; p=p->nxt) {
            oFile<<p->ch;
        }
    }
    oFile.close();
    iTmp.close();
}