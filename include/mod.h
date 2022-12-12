
#ifndef MINIVIM_MOD_H
#define MINIVIM_MOD_H

#include "fun.h"
#include "cursor.h"
#include <ncurses.h>

void NormalMod(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin,cursor &cur);

bool ComMod(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin, cursor &cur);

void InsertMod(WINDOW *&FileWin, WINDOW *&InfWin, WINDOW *&ComWin, WINDOW *&LNWin, cursor &cur);

#endif //MINIVIM_MOD_H
