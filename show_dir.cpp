 //Trailblazer File Explorer
//By:
//   Ankit Pant
//   2018201035
 
 #include <iostream>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <time.h>
 #include <iomanip>
 #include <pwd.h>
 #include <grp.h>
 #include <string>
 #include "ui.h"

using namespace std;

//Prints the directory entries with required information
 void Print_Directory(struct dirent ** entry, int dir_list, int dir_index, int rows) 
{
  int i=6;
  //checking for bounds in the terminal window size
  while(i<rows && dir_index<dir_list && dir_index>=0){ 
    bool is_dir = false;
    bool is_file = false;
    if(entry[dir_index]->d_type==DT_DIR){
      is_dir = true;
    }
    else if(entry[dir_index]->d_type==DT_REG){
      is_file = true;
    }
    struct stat64 f_stat;
    //File Name
    stat64(entry[dir_index]->d_name,&f_stat);

    //File Size
    float file_size = f_stat.st_size;
    bool flag = false;
    if(file_size/1024>=1){
      file_size = file_size/1024;
      flag = true;
    }

    //User and Group Info
    struct passwd *userid = getpwuid(f_stat.st_uid);
    struct group *groupid = getgrgid(f_stat.st_gid);
    
    //User Permissions
    int user_read,user_write,user_exe;
    user_read = f_stat.st_mode & S_IRUSR;
    user_write = f_stat.st_mode & S_IWUSR;
    user_exe = f_stat.st_mode & S_IXUSR;
    
    //Group Permissions
    int group_read,group_write,group_exe;
    group_read = f_stat.st_mode & S_IRGRP;
    group_write = f_stat.st_mode & S_IWGRP;
    group_exe = f_stat.st_mode & S_IXGRP;
    
    //Other permissions
    int other_read,other_write,other_exe;
    other_read = f_stat.st_mode & S_IROTH;
    other_write = f_stat.st_mode & S_IWOTH;
    other_exe = f_stat.st_mode & S_IXOTH;
    
    //File Modification Time
    time_t m_time = f_stat.st_mtime;

    // **********************   
    //    Outputting Data       
    // ***********************  

    
    cout<<"\033["<<i<<";"<<0<<"H";
    string dir_name =entry[dir_index]->d_name;
    if (dir_name.length()>25){
      dir_name = dir_name.substr(0,22);
      dir_name +="...";
    }
    if(entry[dir_index]->d_type==DT_DIR){
      cout<<"\033[1m";
      cout<<"\033[34m";
    }

    //Name
    cout<<dir_name;
    cout<<"\033[0m";
    cout<<"\033["<<i<<";"<<31<<"H";

    //File size
    if(flag){
        cout<<fixed;
        cout<<setprecision(1);
        cout<<file_size<<" KB";
    }
    else
        cout<<file_size<<" B";

    //User Information
    cout<<"\033["<<i<<";"<<43<<"H";
    cout<<userid->pw_name;

    //Group Information
    cout<<"\033["<<i<<";"<<53<<"H";
    cout<<groupid->gr_name;

    //Permissions and kind of entry (file/folder)
    cout<<"\033["<<i<<";"<<64<<"H";
    if(is_dir)
      cout<<"d";
    else if(is_file)
      cout<<"-";

    if(user_read)
          cout<<'r';
        else
          cout<<'-';
        if(user_write)
          cout<<'w';
        else
          cout<<'-';
        if(user_exe)
          cout<<'x';
        else
          cout<<'-';

    if(group_read)
          cout<<'r';
        else
          cout<<'-';
        if(group_write)
          cout<<'w';
        else
          cout<<'-';
        if(group_exe)
          cout<<'x';
        else
          cout<<'-';

    if(other_read)
          cout<<'r';
        else
          cout<<'-';
        if(other_write)
          cout<<'w';
        else
          cout<<'-';
        if(other_exe)
          cout<<'x';
        else
          cout<<'-';

    //Last Modified Time
    cout<<"\033["<<i<<";"<<78<<"H";
    cout<<ctime(&m_time);
    dir_index++;
    i++;
  }
}
  



