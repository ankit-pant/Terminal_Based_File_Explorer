//Trailblazer File Explorer
//By:
//   Ankit Pant
//   2018201035

#include "ui.h"
#include "show_dir.h"
#include "commandproc.h"
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
#include <unistd.h>
#include <sys/wait.h>
#include <stack>
#include <linux/limits.h>


using namespace std;
struct termios term_i;              //Stores the initial terminal settings
FILE *file_descriptor;              //The file descriptor to the terminal

stack<const char*> backwards,forwards;      //Stacks used to keep track of directory traversed
char* root_path;                            //Stores the location (path) at which the application was initiated


//Function to print a template at the top of the screen; contains print statements 
//to make UI more user-friendly by providing details about the various columns in the application window

void Print_Template(){
    cout<<"\033[2J";
    cout<<"\033[3J";
    cout<<"\033[1;40H";
    cout<<"Trailblazer File Explorer\n";
    cout<<">Press : to go to command mode\t>Press Esc to go back to Normal Mode\t>Press q to quit\n";
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
}


//used by the Command Mode to display any changes made by executing commands
//updates the normal mode screen in case of any changes to files/folders

void Repaint_Directory(const char *path, int rows){
    struct dirent **entry;
    DIR *directory_pointer;
    directory_pointer = opendir(path);  //open the current working directory
    
    if (directory_pointer == NULL) 
    {
        cout<<"Error! Cannot open directory\n";
        cout<<"path: "<<path<<"\n";
        cout<<"Press h to go back to Home screen";
    }
    
    int d,e=0;
    d = scandir(path,&entry,NULL,alphasort); 
    //print the contents of the current directory
    Print_Directory(entry,d,e,rows);
}



//Provides the functionality of Normal mode
void List_Directory(const char *path, int rows, struct termios term_n) 
{
   
    Print_Template();       //print the template at the top of screen
    cout<<"\033[u";         //restore cursor position (when exiting from command mode)
    struct dirent **entry;
    DIR *directory_pointer;
    string rp = path;
    if(rp=="/"){
        path = root_path;
    }
    directory_pointer = opendir(path);      //open the current directory
    
    if (directory_pointer == NULL) 
    {
        cout<<"Error! Cannot open directory\n";
        cout<<"path: "<<path<<"\n";
        cout<<"Press h to go back to Home screen";
    }
    
    int dir_list;
    int cur_index = 6;
    int dir_index = 0;
    dir_list = scandir(path,&entry,NULL,alphasort);
    int total_files = 0, total_folders = 0;
    //count the number of files and folders to display at the bottom for a better user experience
    for(int j=0;j<dir_list;j++){
        if(entry[j]->d_type==DT_DIR)
            total_folders++;
        else if(entry[j]->d_type==DT_REG)
            total_files++;
    }
    //go to the last row in the terminal screen and print directory status
    cout<<"\033["<<rows<<";"<<1<<"H";
    cout<<"Normal Mode\t";
    cout<<"Total Files: "<<total_files<<" Total Folders: "<<total_folders;
    //print all entries of directory that can fit in the screen
    Print_Directory(entry, dir_list, dir_index,rows);
    int scrolling_pos;
    closedir(directory_pointer);
    cout<<"\033[6;0H";
    
    int upper_screen_limit = 6; //all directory entries are printed from row 6
    scrolling_pos = upper_screen_limit;     //current cursor position
    dir_index = 0;      //current directory entry (on which the cursor is)
    
    
    char ch, ch2, ch3;
    ch = ' ';
    while(1){
        ch = getchar();
        //if : is pressed go to command mode
        if(ch==58){
            cout<<"\033[s";
            Command_Mode(path,rows,term_n);
            List_Directory(path,rows,term_n);
            cout<<"\033["<<rows<<";"<<1<<"H";
            cout<<"Normal Mode\t";
            cout<<"Total Files: "<<total_files<<" Total Folders: "<<total_folders;
            cout<<"\033[05H";
            cout<<"\033[u";
           
        }
        //if q is pressed quit the application
        else if(ch=='q'){
                tcsetattr(fileno(file_descriptor),TCSANOW,&term_i);
                cout<<"\033[0m";
                cout<<"\033[60;1H";
                cout<<"\033c";
                cout<<"\033[3J";
                exit(0);
        }
        //if enter is pressed open the file/directory
        if(ch=='\n'||ch=='\r'){
            if(entry[dir_index]->d_type==DT_DIR){
                    string folder_name = "/";
                    folder_name+=entry[dir_index]->d_name;
                    string p = path;
                    p+=folder_name;
                    const char * pth = p.c_str();
                    //push current path in stack to facilitate back and forward function (arrow keys)
                    backwards.push(path);
                    //open the directory
                    List_Directory(pth,rows,term_n);
                    
                }
            else if(entry[dir_index]->d_type==DT_REG){
                string file_name = "/";
                file_name+=entry[dir_index]->d_name;
                string p = path;
                p+=file_name;
                const char *pth = p.c_str();
                cout<<"\033[s"; //save current cursor position
                //forking a child to execute a file
                int pid = fork();
                if(pid==0){
                    execl("/usr/bin/xdg-open","xdg-open",pth, NULL);
                }
                wait(NULL);
                //on successful opening of file for execution, return to normal mode
                List_Directory(path,rows,term_n);
                cout<<"\033[u";     //restore cursor position
            }
        }
        //if backspace is pressed go up one level
        else if(ch==127 || ch==8){
            string folder_name = "/..";
            string p = path;
            p+=folder_name;
            const char *pth = p.c_str();
            backwards.push(path);
            List_Directory(pth,rows,term_n);
        }
        //if h is pressed goto home directory
        else if(ch=='h'){
            cout<<"\033[2J";
            cout<<"\033[3J";
            backwards.push(path);
            List_Directory(root_path,rows,term_n);
        }
        //if escape is pressed
        else if (ch=='\033') {
            ch2 = getchar();  
            //if h is pressed goto home
            if(ch2=='h'){
                 cout<<"\033[2J";
                cout<<"\033[3J";
                backwards.push(path);
                List_Directory(root_path,rows,term_n);
            }      
            //if q is pressed quit the application
            else if(ch2=='q'){
                tcsetattr(fileno(file_descriptor),TCSANOW,&term_i);
                cout<<"\033[0m";
                cout<<"\033[60;1H";
                cout<<"\033c";
                cout<<"\033[3J";
                exit(0);
            }
            //if enter is pressed open file/directory
            if(ch2=='\n'||ch2=='\r'){
                if(entry[dir_index]->d_type==DT_DIR){
                    string folder_name = "/";
                    folder_name+=entry[dir_index]->d_name;
                    string p = path;
                    p+=folder_name;
                    const char * pth = p.c_str();
                    backwards.push(path);
    
                    List_Directory(pth,rows,term_n);
                }
                else if(entry[dir_index]->d_type==DT_REG){
                    string file_name = "/";
                    file_name+=entry[dir_index]->d_name;
                    string p = path;
                    p+=file_name;
                    const char * pth = p.c_str();
                    cout<<"\033[s";
                    int pid = fork();
                    if(pid==0){
                        execl("/usr/bin/xdg-open","xdg-open",pth, NULL);
                }
                wait(NULL);
                List_Directory(path,rows,term_n);
                cout<<"\033[u";
            }
            }
            //if : is pressed goto command mode
            if(ch2==58)  {
                cout<<"\033[s";
                Command_Mode(path,rows,term_n);
                List_Directory(path,rows,term_n);
                cout<<"\033["<<rows<<";"<<1<<"H";
                cout<<"Normal Mode\t";
                cout<<"Total Files: "<<total_files<<" Total Folders: "<<total_folders;
                cout<<"\033[05H";
                cout<<"\033[u";
            } 
            //checking for the input of arrow keys
            if(ch2==91){
                ch3 = getchar();
                 //Up Arrow 65, Down Arrow 66, Left Arrow 68, Right Arrow 67

                //Up arrow pressed
                if(ch3==65){

                    if(cur_index==upper_screen_limit){
                        if(dir_index>0){
                        Print_Template();
                        Print_Directory(entry,dir_list,dir_index-(rows-7),rows);
                        cout<<"\033["<<rows<<";"<<1<<"H";
                        cout<<"Normal Mode\t";
                        cout<<"Total Files: "<<total_files<<" Total Folders: "<<total_folders;
                        cout<<"\033["<<rows<<";"<<1<<"H";
                        scrolling_pos = rows;
                        cur_index = rows;
                        dir_index++;
                        }   
                    }
                    //stopping cursor from going above the first directory entry
                    if(scrolling_pos>upper_screen_limit){
                        scrolling_pos--;
                        cout<<"\033[A";
                        dir_index--;
                        cur_index--;
                        //cout<<dir_index<<" "<<scrolling_pos<<" "<<cur_index<<"N\r";
                    }
                }
                //down arrow pressed
                else if(ch3==66){

                    if(cur_index==rows-1){
                        Print_Template();
                        Print_Directory(entry,dir_list,dir_index,rows);
                        cout<<"\033["<<rows<<";"<<1<<"H";
                        cout<<"Normal Mode\t";
                        cout<<"Total Files: "<<total_files<<" Total Folders: "<<total_folders;
                        cout<<"\033[05H";
                        scrolling_pos--;
                        dir_index--;
                        cur_index = 5;
                    }
                    //stopping cursor from scrolling below the number of directory entries
                    if(scrolling_pos<dir_list+5){
                        scrolling_pos++;
                        cur_index++;
                        cout<<"\033[B";
                        dir_index++;
                        //cout<<dir_index<<" "<<scrolling_pos<<" "<<cur_index<<"\r";
                        
                    }
                   
                }
                //right arrow key pressed; go forwards
                else if(ch3==67){
                    const char* fpath;
                    if(!forwards.empty()){
                        backwards.push(path);
                        fpath = forwards.top();
                        //cout<<fpath<<" Path ";
                        forwards.pop();
                        List_Directory(fpath,rows,term_n);
                    }

                }
                //left arrow key pressed; go backwards
                else if(ch3==68){
                    const char* bpath;
                    if(!backwards.empty()){
                        forwards.push(path);
                        bpath = backwards.top();
                        backwards.pop();
                        List_Directory(bpath,rows,term_n);
                    }
                }
            }
            
        }
        
    }

    
}


//Providing functionality off command mode
void Command_Mode(const char *path, int rows, struct termios term_n){
    //set current working directory as home to support relative addressing
    chdir(path);
    cout<<"\033["<<rows<<";"<<1<<"H\033[K:";
    term_n.c_cc[VMIN] = 1;
    term_n.c_cc[VTIME] = 0;
    //set terminal attribute to read each character
    tcsetattr(fileno(file_descriptor),TCSANOW,&term_n);
    string str = "";
    char ch1;
    char ch2, ch3;
    cin.clear(); 
    cout<<"\033[2K";
    cout<<"\033["<<rows<<";"<<1<<"H:";
    while(1) {
        cin.clear();        //clearing the input buffer
        cout.clear();       //clearing the input stream flags
        ch1 = getchar();    
        //escape key pressed    
        if(ch1==27){        
            term_n.c_cc[VMIN] = 0;
            term_n.c_cc[VTIME] = 1;
            //changing terminal attributes to wait for 1 decisecond for input
            tcsetattr(fileno(file_descriptor),TCSANOW,&term_n);
            ch2 = cin.get();
            //if no other character is read quit to normal mode
            if(ch2==-1){
                chdir(root_path);
                break;
            }
            else{
                //if arrow keys were pressed
                if(ch2==91){
                    ch3 = cin.get();
                    //right arrow key pressed move cursor to right
                    if(ch3==67){
                        cout<<"\033[C";
                    }
                    //left arrow key pressed move cursor to left
                    else if(ch3==68){
                        cout<<"\033[D";
                    
                    }
                }
            }   

        }

        //backspace pressed; erase the previously inputted character
        else if(ch1==127){       
            cout<<"\033[D";
            cout<<' ';
            cout<<"\033[D";
            str = str.substr(0,str.length()-1);
        }
        
        
       //enter pressed execute the command entered
        else if(ch1=='\n'){
            cout<<"\033[2K";
            cout<<"\033["<<rows<<";"<<1<<"H:";
            Process_Commands(str,rows,term_n,root_path);
            Print_Template();
            Repaint_Directory(path,rows);
            cout<<"\033["<<rows<<";"<<1<<"H:";
            str = "";
        }
        
        //if only normal character keys pressed append and store in strings to process commands
        else {
            str+=ch1;
            cout<<ch1;       //outputting character as echo is off
        }
        //set terminal attribute to read each character
        term_n.c_cc[VMIN] = 1;
        term_n.c_cc[VTIME] = 0;
        tcsetattr(fileno(file_descriptor),TCSANOW,&term_n);
       
    } 
    
}



//Changes attributes of terminal and initiates UI buildup
void Normal_Mode(){
    struct termios term_n;      
    const char* trem_tty = "/dev/tty";      
    file_descriptor = fopen(trem_tty,"rw");  //get file descriptor of the terminal

    if(!file_descriptor){
        cout<<"Error Opening Terminal";
    }
    //clear the terminal and erase scrollback
    cout<<"\033c";
    cout<<"\033[3J";
    //get current terminal attributes 
    tcgetattr(fileno(file_descriptor),&term_i);
    term_n = term_i;
    //set new terminal attributes (non-canonical mode, etc.)
    term_n.c_lflag &= ~ICANON;
    term_n.c_lflag &= ~ECHO;
    term_n.c_cc[VMIN] = 1;
    term_n.c_cc[VTIME] = 0;
    tcsetattr(fileno(file_descriptor),TCSANOW,&term_n);
    struct winsize window_size;
    ioctl(fileno(file_descriptor),TIOCGWINSZ, &window_size);
    //get the number of rows in terminal
    const int rows = window_size.ws_row;
    size_t psize = pathconf(".",_PC_PATH_MAX);
    char *tbuf = new char[psize];
    //set current path as root path
    root_path = getcwd(tbuf,psize);
    //list all entris in curent path and initiate UI
    List_Directory(root_path,rows,term_n);
}