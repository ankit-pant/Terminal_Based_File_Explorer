//Trailblazer File Explorer
//By:
//   Ankit Pant
//   2018201035



#include "commandproc.h"
#include "ui.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <fstream>
 #include <sys/stat.h>
 #include <dirent.h>

using namespace std;


//Process the copy command
void Copy_Stub(vector<string> &str){
    int len = str.size();
    //check if copy has minimum number of parameters
    if(len<3){
        cout<<"Invalid Command. Usage: copy <filename(s)> <directory>. Press Esc to continue";
        cin.get();
        return;
    }
    vector<string>input_files;
    vector<string>output_files;
    vector<string>::iterator original_string_iter = str.begin();
    original_string_iter++; //skipping command
    vector<string>::iterator original_string_iter_end = str.end();
    original_string_iter_end--;
    int i = 0;
    vector<string>::iterator input_files_iter;
    //Recognising all input files that need to be copied
    while(original_string_iter!=original_string_iter_end){
        string temp0 = *original_string_iter;
        input_files.push_back(temp0);
        original_string_iter++;
    }
    //Output files corresponding to input files
    for(i=0;i<len-2;i++){
        string temp = *original_string_iter_end;
        output_files.push_back(temp);
    }
    //Extracting file name from complete path
    vector<string> temp_input_disection;
    vector<string>::iterator output_files_iter = output_files.begin();
    for(input_files_iter = input_files.begin();input_files_iter!=input_files.end();++input_files_iter,output_files_iter++){
        string temp1;
        stringstream strstream(*input_files_iter);
        while(getline(strstream,temp1,'/')){
            temp_input_disection.push_back(temp1);
        }
        
        vector<string>::iterator temp_input_disection_iter = temp_input_disection.begin();
        while(temp_input_disection_iter!=temp_input_disection.end()){
            ++temp_input_disection_iter;
        }
        temp_input_disection_iter--;
        string temp2 = *temp_input_disection_iter;
        *output_files_iter+="/"+temp2;
        
    }

    //Copying each of the files specified
    input_files_iter = input_files.begin();
    output_files_iter = output_files.begin();
    while(input_files_iter!=input_files.end()){
        ifstream copy_source;
        ofstream copy_dest;
        copy_source.open(*input_files_iter,ios::in);
        copy_dest.open(*output_files_iter,ios::out);
        //If copying not possible
        if(!copy_source){
            cout<<"Cannot Open File. Press Esc to continue";
            cin.get();
            return;
        }
        else{
        copy_dest<<copy_source.rdbuf();
        struct stat64 source_file_stat;
        string t = *input_files_iter;
        const char *temp = t.c_str();
        stat64(temp,&source_file_stat);
        string t1 = *output_files_iter;
        const char *tp = t1.c_str();

        //Ensuring that the file permissions stay intact
        //User Permissions
        int user_read,user_write,user_exe;
        user_read = source_file_stat.st_mode & S_IRUSR;
        user_write = source_file_stat.st_mode & S_IWUSR;
        user_exe = source_file_stat.st_mode & S_IXUSR;
        
        //Group Permissions
        int group_read,group_write,group_exe;
        group_read = source_file_stat.st_mode & S_IRGRP;
        group_write = source_file_stat.st_mode & S_IWGRP;
        group_exe = source_file_stat.st_mode & S_IXGRP;
        
        //Other permissions
        int other_read,other_write,other_exe;
        other_read = source_file_stat.st_mode & S_IROTH;
        other_write = source_file_stat.st_mode & S_IWOTH;
        other_exe = source_file_stat.st_mode & S_IXOTH;
        chmod(tp,user_read|user_write|user_exe|group_read|group_write|group_exe|other_read|other_write|other_exe);
        copy_source.close();
        copy_dest.close();
        input_files_iter++;
        output_files_iter++;
        }
        
    }

}


//Process the rename command
void Rename_Stub(vector<string> &str){
    int len = str.size();
    //check if rename has minimum number of parameters
    if(len<3){
        cout<<"Invalid Command. Usage: rename <filename_old> <filename_new>. Press Esc to continue";
        cin.get();
        return;
    }
    string old_file_name;
    string new_file_name;
    vector<string>::iterator filenames = str.begin();
    filenames++;
    old_file_name = *filenames;
    filenames++;
    new_file_name = *filenames;
    const char* old_name = old_file_name.c_str();
    const char* new_name = new_file_name.c_str();
    rename(old_name,new_name);
}

//Process the create_file command
void Create_File_Stub(vector<string> &str){
    int len = str.size();
    //check if create_file has minimum number of parameters
    if(len<3){
        cout<<"Invalid Command. Usage: create_file <filename> <directory>. Press Esc to continue";
        cin.get();
        return;
    }
    vector<string>::iterator iter = str.begin();
    iter++;
    string file_name = *iter;
    iter++;
    string location = *iter;
    location+="/"+file_name;
    const char* file_path = location.c_str();
    ofstream new_file;
    new_file.open(file_path,ios::out);
    new_file.close();
}

//Process the create_dir command
void Create_Directory_Stub(vector<string> &str){
    int len = str.size();
    //check if create_dir has minimum number of parameters
    if(len<3){
        cout<<"Invalid Command. Usage: create_dir <directory name> <directory path>. Press Esc to continue";
        cin.get();
        return;
    }
    vector<string>::iterator iter = str.begin();
    iter++;
    string dir_name = *iter;
    iter++;
    string location = *iter;
    location+= "/"+dir_name;
    const char *dir_path = location.c_str();
    //creating directory with default permissions
    mkdir(dir_path,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
}

//Process the delete_file command
void Delete_File_Stub(vector<string> &str){
    int len = str.size();
    //check if delete_file has minimum number of parameters
    if(len<2){
        cout<<"Invalid Command. Usage: delete_file <path/filename> Press Esc to continue";
        cin.get();
        return;
    }
    vector<string>::iterator iter = str.begin();
    iter++;
    string file_name = *iter;
    const char* file_path = file_name.c_str();
    unlink(file_path);
}

//Process the delete_dir command
void Delete_Directory_Stub(vector<string> &str){
    int len = str.size();
    if(len<2){
        cout<<"Invalid Command. Usage: delete_dir <path/directory>. Press Esc to continue";
        cin.get();
        return;
    }
    vector<string>::iterator iter = str.begin();
    iter++;
    string dir_name = *iter;
    const char* file_path = dir_name.c_str();
    rmdir(file_path);
}

//Process the move command
void Move_Stub(vector<string> &str){
    int len = str.size();
    //check if move has minimum number of parameters
    if(len<3){
        cout<<"Invalid Command. Usage: move <filename> <directory>. Press Esc to continue";
        cin.get();
        return;
    }
    Copy_Stub(str);
    Delete_File_Stub(str);
}

//Process the goto command
void Goto_Stub(vector<string> &str,int rows, struct termios term_n,char *root_path){
    int len = str.size();
    //check if goto has minimum number of parameters
    if(len<2){
        cout<<"Invalid Command. Usage: goto <path>. Press Esc to continue";
        cin.get();
        return;
    }
    chdir(root_path);
    vector<string>::iterator iter = str.begin();
    iter++;
    string path = *iter;
    if(path=="/"){
        path = root_path;
    }
    const char* goto_pth = path.c_str();
    List_Directory(goto_pth,rows,term_n);}

//Process the search stub
void Search_Stub(vector<string> &str){
    int len = str.size();
    //check if search has minimum number of parameters
    if(len<2){
        cout<<"Invalid Command. Usage: search <filename>. Press Esc to continue";
        cin.get();
        return;
    }
    cout<<"@ Search Stub... Coming Soon. Press Esc to continue";
    cin.get();
    return;
}

//Helper function for snapshot command
//prints directory information to dump file

void Print_Dir_Dump(string path, struct dirent **entry,int dir_list, string dump){
    ofstream dump_file;
    dump_file.open(dump,ios::app);
    dump_file<<path<<":\n";
    int i = 0;
     while(i<dir_list){
        string name = entry[i]->d_name;
        if(name[0]!='.' && name!="..")
            dump_file<<name<<"\t ";
        i++;
    }
    dump_file<<"\n\n";
    dump_file.close();
}

//Helper function to traverse directories recursively

void Traverse_dir(const char *path, string dump_file){
    struct dirent **entry;
    int dir_list = scandir(path,&entry,NULL,alphasort);
    Print_Dir_Dump(path,entry,dir_list,dump_file);
    for(int i=2;i<dir_list;i++){
        if(entry[i]->d_type==DT_DIR){
            string pth = path;
            string name = entry[i]->d_name;
            if(name[0]=='.'){
                continue;
            }
            string pathname = pth;
            pathname+="/";
            pathname+=name;
            const char *p = pathname.c_str();
            Traverse_dir(p,dump_file);
        }
    }

}


//Process the snapshot stub
void Snapshot_Stub(vector<string> &str){
    int len = str.size();
    //check if snapshot has minimum number of parameters
    if(len<3){
        cout<<"Invalid Command. Usage: snapshot <directory> <dump_file>. Press Esc to continue";
        cin.get();
        return;
    }
    vector<string>::iterator iter = str.begin();
    iter++;
    string path = *iter;
    ++iter;
    string dump_file = *iter;
    const char* pth = path.c_str();
    Traverse_dir(pth,dump_file);
}

//Process the restore stub
void Restore_Stub(vector<string> &str){
    cout<<"@ Restore Stub... Coming Soon. Press Esc to continue";
    cin.get();
    return;
}

//Takes the string entered into command mode and processes it in order to support commands
void Process_Commands(string str,int rows,struct termios term_n,char *root_path){
    vector<string> command_disection;
    string temp;
    stringstream strstreem(str); 
    //seperate command components source/destination, etc. with space 
    if(str.length()>1){ 
        while(getline(strstreem,temp,' ')){
            //cout<<temp<<endl;
            command_disection.push_back(temp);
        }
    }
    //checking for various commands entered
    vector<string>::iterator it = command_disection.begin();
    if(*it=="copy"){
        Copy_Stub(command_disection);
        return ;
    }
    else if(*it=="move"){
        Move_Stub(command_disection);
        return;
    }
    else if(*it=="rename"){
        Rename_Stub(command_disection);
        return;
    }
    else if(*it=="create_file"){
        Create_File_Stub(command_disection);
        return;
    }
    else if(*it=="create_dir"){
        Create_Directory_Stub(command_disection);
        return;
    }
    else if(*it=="delete_file"){
        Delete_File_Stub(command_disection);
        return;
    }
    else if(*it=="delete_dir"){
        Delete_Directory_Stub(command_disection);
        return;
    }
    else if(*it=="goto"){
        Goto_Stub(command_disection,rows,term_n,root_path);
        return;
    }
    else if(*it=="search"){
        Search_Stub(command_disection);
        return;
    }
    else if(*it=="snapshot"){
        Snapshot_Stub(command_disection);
        return;
    }
    else if(*it=="restore"){
        Restore_Stub(command_disection);
        return;
    }
    else{
        cout<<"Invalid Command. Press Esc to continue";
        cin.get();
    }
}