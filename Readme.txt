Ankit Pant
2018201035
                   
                    Trailblazer File Explorer

Introduction:
    Trailblazer is a basic Command Line file explorer which offers the user the ability to navigate the file system as well as provide simple commands (like copy, create, etcetera).

Modes of Operation:
    The program provides two modes of operations: Normal Mode and Command Mode. The brief description is as follows:
    
    1. Normal Mode:
            In normal mode the user can navigate the directories using the keys on the keyboard.
            Home Key: Takes the user to the directory where the program was launched.
            Backspace Key: Takes user one level up from the current directory.
            'q' key: Quits the program.
            ':' key: Takes the user to command mode.
            'Enter' key: Open the file/directory at the current cursor position.
            'Up' arrow key / 'Down' arrow key: To move the cursor and choose different entries in the current directory
            'Backward' arrow key: To move to the directory from which the current directory was reached.
            'Forwards' arrow key: To move  again to the directory from which the user came Back from.

    2.  Command mode:
            In command mode user can enter simple commands and run them after pressing Enter key. The command entered must be in the format defined for the command else an error message is returned.


Assumptions made during implementation (Including issues and possible bugs):

    0.  The user tries and keep with the desired inputs and any stray input would not be processed. Instead an error message may be generated.
    1.  The user enters file names/directory names that do not contain spaces.
    2.  The program is run full screen or at the terminal resolution of 103*54 for optimum display of UI. A smaller number of columns less than 101 results in renderation issues and wrapping of text leads to scrambling of file/folder names. The minimum number of rows required to correctly display the UI is 8. 
    3.  Since the information about the directory is organised in neat columns and the name of the file/folder is the first column, having file/directory names grater than a permissible length is not possible (otherwise it intrudes into other columns). Thus to handle long file names, they are truncated with ellipses (...) trailing very long filenames.
    4.  Only Copying/Moving files (not folders) are permitted.
    5.  Only empty directories are allowed to be deleted using the delete_dir command.
    6.  After opening/executing a file, the cursor moves back to the initial position in that directory.
    7.  After exiting from command mode, the cursor moves back to the initial position in that directory.
    8.  The user mostly provides relative paths. Absolute paths may not always work.

