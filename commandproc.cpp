#include "commandproc.h"
#include "ui.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <fstream>
 #include <sys/stat.h>

using namespace std;

void Copy_Stub(vector<string> &str){
    int len = str.size();
    vector<string>input_files;
    vector<string>output_files;
    vector<string>::iterator original_string_iter = str.begin();
    original_string_iter++; //skipping command
    vector<string>::iterator original_string_iter_end = str.end();
    original_string_iter_end--;
    int i = 0;
    vector<string>::iterator input_files_iter;
    while(original_string_iter!=original_string_iter_end){
        string temp0 = *original_string_iter;
        input_files.push_back(temp0);
        original_string_iter++;
    }

    for(i=0;i<len-2;i++){
        string temp = *original_string_iter_end;
        output_files.push_back(temp);
    }

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


    input_files_iter = input_files.begin();
    output_files_iter = output_files.begin();
    while(input_files_iter!=input_files.end()){
        ifstream copy_source;
        ofstream copy_dest;
        copy_source.open(*input_files_iter,ios::in);
        copy_dest.open(*output_files_iter,ios::out);
        if(!copy_source){
            cout<<"\033[K";
            cout<<"Cannot Open File\n";
        }
        else{
        copy_dest<<copy_source.rdbuf();
        struct stat64 source_file_stat;
        string t = *input_files_iter;
        const char *temp = t.c_str();
        stat64(temp,&source_file_stat);
        string t1 = *output_files_iter;
        const char *tp = t1.c_str();
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



void Rename_Stub(vector<string> &str){
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

void Create_File_Stub(vector<string> &str){
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

void Create_Directory_Stub(vector<string> &str){
    vector<string>::iterator iter = str.begin();
    iter++;
    string dir_name = *iter;
    iter++;
    string location = *iter;
    location+= "/"+dir_name;
    const char *dir_path = location.c_str();
    mkdir(dir_path,S_IRWXU|S_IRWXG);
}

void Delete_File_Stub(vector<string> &str){
    vector<string>::iterator iter = str.begin();
    iter++;
    string file_name = *iter;
    //file_name = "./"+file_name;
    const char* file_path = file_name.c_str();
    unlink(file_path);
}
void Delete_Directory_Stub(vector<string> &str){
    vector<string>::iterator iter = str.begin();
    iter++;
    string dir_name = *iter;
    const char* file_path = dir_name.c_str();
    rmdir(file_path);
}

void Move_Stub(vector<string> &str){
    Copy_Stub(str);
    Delete_File_Stub(str);
}

void Goto_Stub(vector<string> &str){
    cout<<"\033[K";
    cout<<"@ Goto Stub";
}

void Search_Stub(vector<string> &str){
    cout<<"\033[K";
    cout<<"@ Search Stub";
}

void Snapshot_Stub(vector<string> &str){
    cout<<"\033[K";
    cout<<"@ Snapshot Stub";
}

void Restore_Stub(vector<string> &str){
    cout<<"\033[K";
    cout<<"@ Restore Stub";
}


void Process_Commands(string str){
    vector<string> command_disection;
    string temp;
    stringstream strstreem(str);  
    if(str.length()>1){ 
        while(getline(strstreem,temp,' ')){
            //cout<<temp<<endl;
            command_disection.push_back(temp);
        }
    }
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
        Goto_Stub(command_disection);
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
}