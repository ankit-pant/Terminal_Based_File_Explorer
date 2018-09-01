#include <string>
#include <termios.h>
using namespace std;

#ifndef INC_COMMANDPROC_H
#define INC_COMMANDPROC_H

void Process_Commands(string str,int rows, struct termios term_n,char *root_dir);

#endif
