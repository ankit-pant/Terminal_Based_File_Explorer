#include "ui.h"
#include "show_dir.h"
#include <fstream>
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <time.h>
 #include <iomanip>
 #include <pwd.h>
 #include <grp.h>


using namespace std;





void Paint_Screen(int rows){
    //cout<<"\033c";
    cout<<"\033[2J";
    cout<<"\033[3J";
    cout<<"\033[1;40H";
    cout<<"Trailblazer File Explorer\n";
    cout<<">Press : to go to command mode\t>Press Esc twice to go back to Normal Mode\t>Press q to quit\n\n";
    cout<<"\033[4;0H";
    cout<<"\033["<<4<<";"<<4<<"H";
    cout<<"Name";
    cout<<"\033["<<4<<";"<<32<<"H";
    cout<<"Size";
    cout<<"\033["<<4<<";"<<43<<"H";
    cout<<"User";
    cout<<"\033["<<4<<";"<<53<<"H";
    cout<<"Group";
    cout<<"\033["<<4<<";"<<63<<"H";
    cout<<"Permissions";
    cout<<"\033["<<4<<";"<<83<<"H";
    cout<<"LastModified\n";
    cout<<"\033[6;0H";
}

void List_Directory(const char *path, int rows) 
{
    struct dirent **entry;
    DIR *directory_pointer;
    directory_pointer = opendir(path);
    if (directory_pointer == NULL) 
    {
        cout<<("cannot open directory\n");
    }
    int d,e=0,i=6;
    int scrolling = 0;
  //cout<<rows<<"row";
    d = scandir(path,&entry,NULL,alphasort);
    int total_files = 0, total_folders = 0;
    for(int j=0;j<d;j++){
        if(entry[j]->d_type==DT_DIR)
            total_folders++;
        else if(entry[j]->d_type==DT_REG)
            total_files++;
    }
    cout<<"\033["<<rows<<";"<<1<<"H";
    cout<<"Total Files: "<<total_files<<" Total Folders: "<<total_folders;
    while(e<d && scrolling <rows-6){
        Print_Directory(entry,e,i);
        scrolling++;
        e++;
        i++;
    }
    closedir(directory_pointer);
    cout<<"\033[6;0H";
}

void Go_Backwards(){
    cout<<"back\r";
}

void Go_Forwards(){
    cout<<"next\r";
    
}

void Goto_Home(int rows){
    cout<<"\033[2J";
    cout<<"\033[3J";
    Paint_Screen(rows);
    List_Directory("./",rows);

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





void Command_Mode(int rows){
    
    cout<<"\033["<<rows<<";"<<1<<"H\033[K:";
    //cout<<"\033[1;37m";
    string str;
    string ch1;
    char ch2, ch3;
   do {
        ch1 = getchar();
        char backspc1 = 127;
        string chck_bksp = "";
        chck_bksp+=backspc1;
        if(ch1.compare(chck_bksp)==0){
            cout<<"\033[D";
            cout<<' ';
            cout<<"\033[D";
        }
        
        if(ch1=="\033"){
            ch2 = cin.get();
            if(cin.eof()){
                cout<<"Yes!";
                ch2='\033';
                ch3 = getchar();
                continue;
            }
            else{
                if(ch2==91){
                    ch1=" ";
                    ch3 = cin.get();
                    if(ch3==67){
                        cout<<"\033[C";
                    }
                    else if(ch3==68){
                        cout<<"\033[D";
                    
                    }
                }
                continue;
            }
        }
       
        if(ch1=="\n"){
            cout<<"\033[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            //cout<<"\033[1;37m";
        }
        else{
            str+=ch1;
            cout<<ch1;       //outputting character as echo is off
        }
            
    }while(ch1!="\033");  

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
    const int rows = window_size.ws_row;
    char *pwd = "./";
    Paint_Screen(rows);
    List_Directory(pwd,rows);
    char ch, ch2, ch3;
    ch = ' ';
    while(ch!='q'){
        ch = getchar();
        if(ch==58){
            Command_Mode(rows);
            //cout<<"\033[0m";
            cout<<"\33[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            cout<<"\033[3;1H";
            Paint_Screen(rows);
            List_Directory(pwd,rows);
            continue;
        }

        if(ch=='\n'||ch=='\r'){
            cout<<"Enter pressed\r";
        }

        else if(ch==127 || ch==8){
            Goto_Up_Level();
        }
        else if(ch=='h'){
            Goto_Home(rows);
        }
        else if (ch=='\033') {
            ch2 = getchar();  
            if(ch2=='q'){
                ch='q';
            }
            if(ch2=='h'){
                 Goto_Home(rows);
            }      
            if(ch2=='\n'||ch2=='\r'){
            cout<<"Enter pressed\r";
            }
            if(ch2==58)  {
                Command_Mode(rows);
                //cout<<"\033[0m";
                cout<<"\33[2K";
                cout<<"\033["<<rows<<";"<<1<<"H:";
                cout<<"\033[3;1H";
                Paint_Screen(rows);
                continue;
            } 
           
            if(ch2==91){
                ch3 = getchar();
                 //Up Arrow 65, Down Arrow 66, Left Arrow 68, Right Arrow 67
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