#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
//Function to get the file type as a string based on the type provided by the dirent structure
char *filetype(unsigned char type){
    char *str;
    switch(type)
    {
    case DT_BLK:
        str = "block device";
        break;
    case DT_CHR:
        str = "character device";
        break;
    case DT_DIR:
        str = "directory";
        break;
    case DT_FIFO:
        str = "named pipe (FIFO)";
        break;
    case DT_REG:
        str = "regular file";
        break;
    case DT_SOCK:
        str = "UNIX domain socket";
        break;
    case DT_UNKNOWN:
        str = "unknown file type";
        break;
    default:
        str = "UNKNOWN";
    }
    return str;
}
// Recursive function to traverse through the directory and its subdirectories
void traverse_directory(const char *dir_name, int depth){
    struct dirent *dirent; //Struct to store information about directory entries
    DIR *dir;// Pointer to the directory stream
    struct stat statbuf; //Struct for file information
    // Attempting to open the directory
    dir = opendir(dir_name);
    if (dir == NULL){
        printf("There is a Error Opening the Directory '%s'\n",dir_name);
        return;
    }
    //Changing current working directory the one being traversed
    chdir(dir_name);
    //Looping through directory entries
    while ((dirent = readdir(dir))!= NULL){
        // Skipping the special entries
        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name,"..")==0)
        continue;
        //Printing the file type and name, with indentation based on the depth in the directory tree
         printf("%*s[%s] %s\n",depth * 2, "",filetype(dirent->d_type),dirent->d_name);
         //If the entry is a directory, call function recursively
          if (dirent->d_type == DT_DIR){
             traverse_directory(dirent->d_name,depth + 1);
         }
    
    }
    //Changing back to the parent directory
    chdir("..");
    //Closing the directory stream
    closedir(dir);
}
int main(int argc, char **argv){
    //Checking to see if the user has provided a directory name as a command line argument
    if (argc < 2){
        printf("Usage: %s <dirname>\n",argv[0]); //Displaying usuage message if directory name is not provided
        exit(-1);
    }
    traverse_directory(argv[1],0);
    return 0; //Returning 0 to indicate a successful execution
}