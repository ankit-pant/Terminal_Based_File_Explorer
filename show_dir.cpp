 #include <iostream>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <time.h>
 #include <iomanip>
 #include <pwd.h>
 #include <grp.h>
 #include "ui.h"

using namespace std;

 void List_Directory(const char *path, int rows) 
{
  struct dirent **entry;
  DIR *directory_pointer;
  struct stat64 f_stat;
  directory_pointer = opendir(path);
  if (directory_pointer == NULL) 
  {
    cout<<("cannot open directory\n");
  }
  int d,i = 6,j=0;
  int scrolling = 0;
  
  d = scandir(path,&entry,NULL,alphasort);
  while(j<d && i<rows){
    //File Name
    stat64(entry[j]->d_name,&f_stat);

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

    /* **********************   */
    /*    Outputting Data       */
    /* ***********************  */

    cout<<"\033["<<i<<";"<<0<<"H";
    cout<<(entry[j]->d_name);
    cout<<"\033["<<i<<";"<<27<<"H";

    if(flag){
        cout<<fixed;
        cout<<setprecision(2);
        cout<<file_size<<" KB";
    }
    else
        cout<<file_size<<" B";

    cout<<"\033["<<i<<";"<<40<<"H";
    cout<<userid->pw_name;

    cout<<"\033["<<i<<";"<<50<<"H";
    cout<<groupid->gr_name;

    cout<<"\033["<<i<<";"<<64<<"H";

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

    cout<<"\033["<<i<<";"<<78<<"H";
    cout<<ctime(&m_time);
    cout<<endl;


    free(entry[j]);
    i++;
    j++;
  }
  closedir(directory_pointer);
}



