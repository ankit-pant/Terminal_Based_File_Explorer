#include "ui.h"
#include <fstream>
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

void Command_Mode(FILE *file_descriptor){
    struct winsize window_size;
    ioctl(fileno(file_descriptor),TIOCGWINSZ, &window_size);
    int rows = window_size.ws_col;
    cout<<"\033["<<rows<<";"<<1<<"H:";
    string str;
    char ch;
    ifstream instr;
   do {
        ch = getchar();
        str+=ch;
        if(ch=='\n'){
            //cout<<"hello";
            cout<<"\033[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            //Parse_String(str);
            
        }
        else 
            cout<<ch;
        
    }while(ch!='\033');// && ch!= '^[[A' && ch!= ^[[B && ch!= ^[[C && ch!= ^[[D);

}

void Normal_Mode(){
     struct termios term_i, term_n;
    const char* trem_tty = "/dev/tty";
    FILE *file_descriptor;
    file_descriptor = fopen(trem_tty,"rw");

    if(!file_descriptor){
        cout<<"Error Opening Terminal";
    }
    cout<<"\033c";
    cout<<"\033[3J";
    tcgetattr(fileno(file_descriptor),&term_i);
    term_n = term_i;
    term_n.c_lflag &= ~ICANON;
    term_n.c_lflag &= ~ECHO;
    term_n.c_cc[VMIN] = 1;
    term_n.c_cc[VTIME] = 0;
    tcsetattr(fileno(file_descriptor),TCSANOW,&term_n);
    struct winsize window_size;
    ioctl(fileno(file_descriptor),TIOCGWINSZ, &window_size);
    int rows = window_size.ws_col;
    cout<<"\033["<<rows<<";"<<1<<"H:";
    cout<<"\033[1;1H";
    cout<<"\t\t\tTrailblazer File Explorer\n";
    cout<<">Press : to go to command mode\t>Press Esc to go back to Normal Mode\t>Press q to quit\n";
    char ch;
    char temp;
    while(ch!='q'){
        //tcflush(fileno(file_descriptor),TCIFLUSH);
        //tcflush(fileno(file_descriptor),TCOFLUSH);
        ch = getchar();
        if(ch==58){
            Command_Mode(file_descriptor);
            cout<<"\33[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            cout<<"\033[3;1H";
        }  
    }
    tcsetattr(fileno(file_descriptor),TCSANOW,&term_i);
    cout<<"\033[60;1H";
    cout<<"\033c";
    cout<<"\033[3J";
}