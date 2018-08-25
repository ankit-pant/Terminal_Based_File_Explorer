 #include <iostream>
 #include <dirent.h>
using namespace std;

 void List_Directory(const char *path) 
{
  struct dirent *entry;
  DIR *dp;

  dp = opendir(path);
  if (dp == NULL) 
  {
    cout<<("cannot open directory\n");
  }

  while((entry = readdir(dp)))
    cout<<(entry->d_name)<<endl;

  closedir(dp);
}



