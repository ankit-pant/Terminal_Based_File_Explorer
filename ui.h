#include <string>
#include <termios.h>


#ifndef INC_UI_H
#define INC_UI_H

void Normal_Mode();
void Command_Mode(const char *path, int rows,struct termios term_n);
void Paint_Screen(int rows);
void List_Directory(const char *path, int rows, struct termios term_n);


#endif
