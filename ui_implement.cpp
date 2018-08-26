#include "ui.h"
#include <fstream>
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
#include "show_dir.h"

using namespace std;


void Paint_Screen(int rows,char *path){
    //cout<<"\033c";
    cout<<"\033[2J";
    cout<<"\033["<<rows<<";"<<1<<"H:";
    cout<<"\033[1;40H";
    cout<<"Trailblazer File Explorer\n";
    cout<<">Press : to go to command mode\t>Press Esc to go back to Normal Mode\t>Press q to quit\n\n";
    cout<<"\033[4;0H";
    cout<<"\033["<<4<<";"<<5<<"H";
    cout<<"Name";
    cout<<"\033["<<4<<";"<<28<<"H";
    cout<<"Size";
    cout<<"\033["<<4<<";"<<37<<"H";
    cout<<"User Permissions";
    cout<<"\033["<<4<<";"<<56<<"H";
    cout<<"Group Permissions";
    cout<<"\033["<<4<<";"<<80<<"H";
    cout<<"LastModified\n";
    List_Directory(path);
    cout<<"\033[6;0H";
}



void Go_Backwards(){
    cout<<"backr";
}

void Go_Forwards(){
    cout<<"next\r";
    
}

void Goto_Home(){
   Paint_Screen(80,"./");

}

void Goto_Up_Level(){
    cout<<"level up\n";

}

void Move_Cursor_Up(){
    cout<<"\033[A";

}

void Move_Cursor_Down(){
    cout<<"\033[B";

}





void Command_Mode(FILE *file_descriptor){
    struct winsize window_size;
    ioctl(fileno(file_descriptor),TIOCGWINSZ, &window_size);        //gives the size of terminal window
    int rows = window_size.ws_col;          //number of rows in terminal
    cout<<"\033["<<rows<<";"<<1<<"H\033[K:";
    //cout<<"\033[1;37m";
    string str;
    char ch1;
    ifstream instr;
   do {
        ch1 = getchar();
       /* if(ch1=='\033'){
            char ch2, ch3;
            ch2 = getchar();
            if(ch2==0)
                continue;
            ch3 = getchar();
            if(ch2==91){
                    if(ch3==65)
                        ch1 = ' ';
                    else if(ch3==66)
                        ch1 = ' ';
                    else if(ch3==67){
                        cout<<"\033[C";
                        ch1 = ' ';
                    }
                    else if(ch3==68){
                        cout<<"\033[C";
                        ch1 = ' ';
                    }
            }
        }*/
        if(ch1=='\n'){
            cout<<"\033[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            //cout<<"\033[1;37m";
        }
        else{
            str+=ch1;
            cout<<ch1;       //outputting character as echo is off
        }
            
    }while(ch1!='\033');  

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
    char *pwd = "./";
    Paint_Screen(rows,pwd);
    char ch, ch2, ch3;
    ch = ' ';
    while(ch!='q'){
        ch = getchar();
        if(ch==58){
            Command_Mode(file_descriptor);
            //cout<<"\033[0m";
            cout<<"\33[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            cout<<"\033[3;1H";
            Paint_Screen(rows,pwd);
            continue;
        }
        else if(ch==127 || ch==8){
            Goto_Up_Level();
        }
        else if(ch=='h'){
            Goto_Home();
        }
        else if (ch=='\033') {
            ch2 = getchar();
            ch3 = getchar();
            if(ch2==91){
                 //Up Arrow 65, Down Arrow 66, Left Arrow 67, Right Arrow 68
                if(ch3==65)
                    Move_Cursor_Up();
                else if(ch3==66)
                    Move_Cursor_Down();
                else if(ch3==67)
                    Go_Forwards();
                else if(ch3==68)
                    Go_Backwards();
            }
        }
    }
    tcsetattr(fileno(file_descriptor),TCSANOW,&term_i);
    cout<<"\033[0m";
    cout<<"\033[60;1H";
    cout<<"\033c";
    cout<<"\033[3J";
}