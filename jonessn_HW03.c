/*
Name: Shaniyah Jones
BlazerId: jonessn
Project #: Homework 3
To compile: make
To run: 
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
// Function to count number of words in a file
int calculate_word_count(const char *file_location){
    FILE *input_file = fopen(file_location,"r");
    if(!input_file){
        perror("Error: Uh Oh 😰 Unable to open file");
        return -1;
    }
    int word_total = 0;
    char temp_word[256];
    while(fscanf(input_file,"%s",temp_word) == 1){
        word_total++;
    }
    fclose(input_file);
    return word_total;
}
// Function to process individual files
void handle_file(const char *full_file_path){
    struct stat file_info;

    //Fetching file details using stat
    if(stat(full_file_path,&file_info) == - 1){
        perror("Uh Oh 😰 There is an Error retrieving file information");
        return;
    }
    //Displaying file name and size
    printf("File: %s | Size: %ld bytes",full_file_path,file_info.st_size);
    // If its a text file then count and display number of words
    if(strstr(full_file_path,".txt")!= NULL){
        int total_words = calculate_word_count(full_file_path);
        if(total_words != -1){
            printf("| Words: %d",total_words);
        }
    }
    printf("\n");
}
int main(int argc, char *argv[]){
    //Verifying the correct number of arguments that are passed
    if(argc != 2){
        fprintf(stderr,"Usage: %s <directory_name>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    //Opening a certain directory
    DIR *dir_ptr = opendir(argv[1]);
    if(!dir_ptr){
        perror("Uh Oh 😰 There is an Error opening directory");
        exit(EXIT_FAILURE);
    }
    struct dirent *dir_entry;
    //Iterating over directory entries
    while ((dir_entry = readdir(dir_ptr))!= NULL){
        //Constructing the full path to each file
        char full_path[512];
        snprintf(full_path,sizeof(full_path),"%s/%s",argv[1],dir_entry->d_name);
        // Using stat() to confirm it's a regular file, not a directory
        struct stat entry_info;
        if(stat(full_path,&entry_info) != 0){
            perror("Uh Oh 😰 There is an Error retrieving entry info");
            continue;
        }
        if (S_ISREG(entry_info.st_mode)){
            //Creating a child process to handle each file
            pid_t process_id = fork();
            if(process_id < 0){
                perror("Uh Oh 😰 Fork has failed");
                exit(EXIT_FAILURE);
            } else if (process_id == 0){
                // Child process handles file processing
                handle_file(full_path);
                exit(EXIT_SUCCESS);
            }
        }
    }
    //Closing directory stream
    closedir(dir_ptr);
    //Parent waits for all childs process to complete
    while(wait(NULL) > 0);
    return 0;
}