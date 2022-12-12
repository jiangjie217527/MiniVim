#include "trie.h"
#include "prefix.h"
#include "fun.h"
#include <ncurses.h>
#include <cstring>
#include <fstream>
#include "cursor.h"
#include <vector>

using Lexicon = trie::trie_map<char, trie::SetCounter>;
using WordList = std::vector<std::string>;
WordList words;
Lexicon lexicon;
extern int Y, X;
char p[100];
extern std::vector<int> tmp[];

void initwordlist() {
    std::ifstream alpha;
    alpha.open("words_alpha.txt");
    std::string str;
    while (getline(alpha, str))
        words.push_back(str);
    for (const auto &word: words)
        lexicon.insert(word);
}

int complete(WINDOW *&FileWin, WINDOW *&ComWin, cursor &cur) {
    WordList tmpword;
    int len = 0;
    std::string str;
    int l, r;
    l = r = cur.x;
    do {
        l--;
    } while (l > 0 && tmp[cur.y][l - 1] != ' ');
    for (int i = l; i < r; i++)str += (char) tmp[cur.y][i];
/*
    wprintw(ComWin,"%d %d ",l,r);
    strcpy(p,str.c_str());
    wprintw(ComWin,"%s ",p);
    wrefresh(ComWin);
    getch();
*/
    for (auto it = lexicon.find_prefix(str); it != lexicon.end(); ++it) {
        tmpword.push_back(it.key());
        len++;
    }
    int i = 0, ch;
    while (true) {
        werase(ComWin);
        for (int j = 0; j < 5 && i + j < len; j++) {
            strcpy(p, tmpword[i + j].c_str());
            p[strlen(p) - 1] = '\0';
            wprintw(ComWin, "%d:%s ", j + 1, p);
        }
        wrefresh(ComWin);
        wrefresh(FileWin);
        ch = wgetch(FileWin);

        if (ch >= '1' && ch <= '5'&&i + ch - '1' < len) {
            int j = ch - '1';
            int prelen = r - l;
            strcpy(p, tmpword[i + j].c_str());
            p[strlen(p) - 1] = '\0';
            for (int k = prelen; k < strlen(p); k++)
                insert(p[k], cur);
            insert(' ', cur);
            werase(ComWin);
            wrefresh(ComWin);
            return 0;
        } else {
            int breakflg = 0;
            switch (ch) {
                case 266:
                    if (i + 5 >= len)continue;
                    i += 5;
                    break;
                case 265:
                    if (i <= 0)continue;
                    i -= 5;
                    break;
                default:
                    breakflg = 1;
            }
            if (breakflg == 1)break;
        }
    }
    werase(ComWin);
    wrefresh(ComWin);
    return ch;
}
