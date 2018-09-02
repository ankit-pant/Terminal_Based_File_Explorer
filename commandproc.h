//Trailblazer File Explorer
//By:
//   Ankit Pant
//   2018201035


#include <string>
#include <termios.h>
using namespace std;

#ifndef INC_COMMANDPROC_H
#define INC_COMMANDPROC_H

//Takes the string entered into command mode and processes it in order to support commands
void Process_Commands(string str,int rows, struct termios term_n,char *root_dir);

#endif
