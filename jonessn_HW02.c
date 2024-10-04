/*
Name: Shaniyah Jones
BlazerId: jonessn
Project #: 2
To compile & Run: make make run
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>
#include <limits.h>
//maximum path length
#define MAX_PATH_LENGTH 1024
//struct to hold file details
typedef struct FileDetails{
    char filename[MAX_PATH_LENGTH]; //File name
    off_t filesize; // File size
    int isSymbolicLink; //Flag for symbolic link
    char symbolicLinkTarget[MAX_PATH_LENGTH]; //Target of the symbolic link
}FileDetails;
//Global variables for file array managemet
FileDetails *fileArray = NULL; //Dynamic array to hold file details
size_t totalFiles = 0; //Current number of files
size_t maxFiles = 10; //Maximum number of files allocated
//Function to add a file's details to the array
void add_file(const char *filename, off_t filesize, int isSymbolicLink,const char *symbolicLinkTarget){
    //Checking to see if we need to resize the array
    if(totalFiles >= maxFiles){
        maxFiles *= 2; //Double the size of the array
        fileArray = realloc(fileArray,maxFiles * sizeof(FileDetails)); //Reallocate memory
        if (!fileArray){//Checking to see if memory allocation was successful
            perror("uh oh Failed to allocate the memory🤬");
            exit(EXIT_FAILURE);
        }
     }
        //Copying the file details into the array
        strcpy(fileArray[totalFiles].filename,filename);
        fileArray[totalFiles].filesize = filesize;
        fileArray[totalFiles].isSymbolicLink = isSymbolicLink;//Copying link target if it's a symbolic link
        if(isSymbolicLink){
            strcpy(fileArray[totalFiles].symbolicLinkTarget,symbolicLinkTarget);
        }
        totalFiles++; //Increment the total files count
}
//Function to compare file sizes for sorting
int compare_filesize(const void *a, const void *b){
        FileDetails *fileA = (FileDetails *)a; //Cast to FileDetails
        FileDetails *fileB = (FileDetails *)b;
        return (fileA->filesize > fileB->filesize) - (fileA->filesize < fileB->filesize);
    }
    //Function to list files and directories recursively
    void list_files(const char *baseDir, int displaySize, off_t minFileSize, const char *filterPattern, int sortReverse, int fileType){
        struct dirent *directoryEntry; //Directory entry structure
        struct stat fileStat; //File statistics structure
        char fullPath[MAX_PATH_LENGTH]; //Buffer to hold the full path of files
        DIR *directory = opendir(baseDir); //Open the directory
        //Check if the directory was opened successfully
        if (directory == NULL){
            perror("opendir");
            return;
        }
        //Read each entry in the directory
        while((directoryEntry = readdir(directory))!= NULL){
            // Skip the current and parent directory entries
            if (strcmp(directoryEntry->d_name,".")==0 || strcmp(directoryEntry->d_name,"..")==0){
                continue;
            }
            //Create the full path to the file
            snprintf(fullPath,sizeof(fullPath),"%s/%s",baseDir,directoryEntry->d_name);
            //Get the file status
            if(lstat(fullPath,&fileStat)<0){
                perror("lstat");
                continue;
            }
            //Checking if the entry is a symbolic link
            int isSymbolicLink = S_ISLNK(fileStat.st_mode);
            off_t filesize = isSymbolicLink ? 0 : fileStat.st_size;
            //Checking if the entry is a directory
            if(directoryEntry->d_type == DT_DIR){
                if(fileType == 0){
                    printf("%s:\n",directoryEntry->d_name);
                    //Recursive call to list files in the subdirectiry
                    list_files(fullPath,displaySize,minFileSize,filterPattern,sortReverse,fileType); 
                }
            } else {
                if(fileType == 1){// 1 for regular files only
                    add_file(directoryEntry->d_name,filesize,isSymbolicLink,"");
                    continue;
                }
                // Filter based on provided pattern
                if(filterPattern && fnmatch(filterPattern,directoryEntry->d_name,0)!=0){
                    continue; // Skip if pattern does not match
                }
                // Skip files smaller than the minimum size
                if(filesize < minFileSize){
                    continue;
                }
                // Handle symbolic link
                if(isSymbolicLink){
                    char linkTarget[MAX_PATH_LENGTH];
                    ssize_t len = readlink(fullPath,linkTarget,sizeof(linkTarget)-1); //Get link target
                    if (len != -1){
                        linkTarget[len] = '\0'; // Null-terminate the string
                        add_file(directoryEntry->d_name,0,isSymbolicLink,linkTarget); //Add symbolic link details
                    }
                } else {
                    add_file(directoryEntry->d_name,filesize,isSymbolicLink,""); //Add regular file details
                }
            }
        }
        closedir(directory); //Closing the directory
}
//function to print details of the files collected
void print_file_details(int displaySize){
    for(size_t i = 0; i < totalFiles; i++){
        if(fileArray[i].isSymbolicLink){
            printf("%s -> %s\n",fileArray[i].filename,fileArray[i].symbolicLinkTarget); //Print symbolic link
        }else {
            if(displaySize){
                printf(" %s (%ld bytes)\n",fileArray[i].filename,fileArray[i].filesize); //Print file size
            } else {
                printf("%s\n",fileArray[i].filename); //Print file name only
            }
        }
    }
}
// Main function to process command-line arguments and start the listing
int main(int argc, char *argv[]){
    const char *startingDir = "."; //Default starting directory
    int displaySize = 0; // Flag for displaying size
    off_t minFileSize = 0; // Minimum file size
    const char *filterPattern = NULL; //Filter pattern for files
    int sortReverse = 0; // Flag for reverse sorting
    int fileType = 0;// File type filter
    int opt; //Variable for command-line options
    //Processing command-line arguments
    while((opt = getopt(argc,argv,"Ss:f:r:t:"))!= -1){
        switch(opt){
            case 'S':
                displaySize = 1; //Setting flag to display file sizes
                break;
            case 's':
                minFileSize = atol(optarg); //Setting minimum file size
                break;
            case 'f':
                filterPattern = optarg; //Setting file name pattern filter
                break;
            case 'r':
                sortReverse = 1; //Setting flag for reverse sorting
                break;
            case 't':
                if(strcmp(optarg,"f")==0){
                    fileType = 1; //Regular files only
                }else if(strcmp(optarg,"d")==0){
                    fileType = 2; //Directories only
                }
                break;
            default:
                fprintf(stderr,"Usage: %s [-S] [-s size] [-f pattern] [-r] [-t f|d] [directory]\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (optind < argc){
        startingDir = argv[optind]; //Getting the starting directory if provided
    }
    fileArray = malloc(maxFiles * sizeof(FileDetails)); //Allocate memory for the file array
    if(!fileArray){
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    //Start listing files from the specified directory
    list_files(startingDir,displaySize,minFileSize,filterPattern,sortReverse,fileType);
    //Sorting files if the reverse flag is set
    if(sortReverse){
        qsort(fileArray,totalFiles,sizeof(FileDetails),compare_filesize);
    }
    print_file_details(displaySize); //Print collected file details
    free(fileArray); //Free allocated memory
    return 0; //Return success
}
//Resources I used that help me with this
//https://www.learn-c.org/
//https://cboard.cprogramming.com/c-programming/109410-directories-files-using-dirent-h.html
//https://www.tutorialspoint.com/c_standard_library/limits_h.htm

