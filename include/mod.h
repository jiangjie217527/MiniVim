
#ifndef MINIVIM_MOD_H
#define MINIVIM_MOD_H
#include "fun.h"
#include <ncurses.h>
void NormalMod(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin,int y,int x);
bool ComMod(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin,int &y,int &x);
void InsertMod(WINDOW* &FileWin,WINDOW* &InfWin,WINDOW* &ComWin,WINDOW* &LNWin,int y,int x);

#endif //MINIVIM_MOD_H
