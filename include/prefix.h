#ifndef MINIVIM_PREFIX_H
#define MINIVIM_PREFIX_H

#include "cursor.h"
#include <ncurses.h>

int complete(WINDOW *&FileWin, WINDOW *&ComWin, cursor &cur);

void initwordlist();

#endif //MINIVIM_PREFIX_H
