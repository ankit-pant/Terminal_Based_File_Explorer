//Trailblazer File Explorer
//By:
//   Ankit Pant
//   2018201035

#include <string>
#include <termios.h>


#ifndef INC_UI_H
#define INC_UI_H

void Normal_Mode();                 //function that changes terminal settings and initiates UI buildup
void Command_Mode(const char *path, int rows,struct termios term_n);   //function that enables the use of command mode
void List_Directory(const char *path, int rows, struct termios term_n); //function that enables the use of user mode


#endif
