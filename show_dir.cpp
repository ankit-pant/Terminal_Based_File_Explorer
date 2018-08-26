 #include <iostream>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <time.h>
 #include <iomanip>
using namespace std;

 void List_Directory(const char *path) 
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
  
  d = scandir(path,&entry,NULL,alphasort);
  while(j<d){
    stat64(entry[j]->d_name,&f_stat);
    cout<<"\033["<<i<<";"<<0<<"H";
    cout<<(entry[j]->d_name);
    cout<<"\033["<<i<<";"<<27<<"H";
    float file_size = f_stat.st_size;
    bool flag = false;
    if(file_size/1024>=1){
      file_size = file_size/1024;
      flag = true;
    }
    if(flag){
      cout<<fixed;
      cout<<setprecision(2);
      cout<<file_size<<" KB";
    }
    else
      cout<<file_size<<" B";
    cout<<"\033["<<i<<";"<<44<<"H";
    int user_read,user_write,user_exe;
    user_read = f_stat.st_mode & S_IRUSR;
    user_write = f_stat.st_mode & S_IWUSR;
    user_exe = f_stat.st_mode & S_IXUSR;
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
    cout<<"\033["<<i<<";"<<64<<"H";
    int group_read,group_write,group_exe;
    group_read = f_stat.st_mode & S_IRGRP;
    group_write = f_stat.st_mode & S_IWGRP;
    group_exe = f_stat.st_mode & S_IXGRP;
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
    cout<<"\033["<<i<<";"<<76<<"H";
    time_t m_time = f_stat.st_mtime;
    cout<<ctime(&m_time);
    cout<<endl;
    free(entry[j]);
    //d--;
    i++;
    j++;
  }
  closedir(directory_pointer);
}



