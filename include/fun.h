
#ifndef MINIVIM_FUN_H
#define MINIVIM_FUN_H

#include <ncurses.h>
#include "cursor.h"

void showlineX(int x,int mode);

void refreshWin(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&LNWin,cursor &cur, int mode);

void init(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin);

void move(char *argc);

void move_cursor(int ch,cursor &cur);

void dellin(int y);

void addlin(int y, int x);

void insert(int ch,cursor &cur);
#endif //MINIVIM_FUN_H
/*  优秀但不好用的链表
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
    */
