 #include <iostream>
 #include <dirent.h>
using namespace std;

 void List_Directory(const char *path) 
{
  struct dirent **entry;
  DIR *directory_pointer;

  directory_pointer = opendir(path);
  if (directory_pointer == NULL) 
  {
    cout<<("cannot open directory\n");
  }
  int d,i = 4,j=0;
  cout<<"\033["<<i<<";"<<5<<"H";
  cout<<"Name";
  cout<<"\033["<<i<<";"<<40<<"H";
  cout<<"Type";
  cout<<"\033["<<i<<";"<<58<<"H";
  cout<<"Size\n";
  i++;

  d = scandir(path,&entry,NULL,alphasort);
  while(j<d){
    cout<<"\033["<<i<<";"<<0<<"H";
    cout<<(entry[j]->d_name);
    cout<<"\033["<<i<<";"<<40<<"H";
    //cout<<(entry[d]->d_type);
    cout<<"\033["<<i<<";"<<60<<"H";
    cout<<(entry[j]->d_reclen);
    cout<<endl;
    free(entry[j]);
    //d--;
    i++;
    j++;
  }
  closedir(directory_pointer);
}



