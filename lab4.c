#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int srcFile, destFile; // File descriptions for source and destination
    char buffer[BUFFER_SIZE]; // Buffer to store data
    ssize_t numBytesRead, numBytesWritten; // Tracking bytes read and written
    // Validating the number of command-line arguments
    if (argc != 3){
        printf("Usage: %s <source_file> <destination_files>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    //Ensuring the source & destination not the same
    if (strcmp(argv[1], argv[2]) == 0){
        printf("Oops! Error: Source & destination files should not be the same.\n");
        exit(EXIT_FAILURE);
    }
    //Opening the source file in read-only mode 
    srcFile = open(argv[1],O_RDONLY);
    if(srcFile == -1){
        printf("Oops! Error: Could not open source file %s.\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    //Opening the destination file in append mode but if it don't exist then create
    destFile = open(argv[2],O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (destFile == -1){
        printf("Oops! Error: Could not open destination file %s.\n",argv[2]);
        close(srcFile);
        exit(EXIT_FAILURE);
    }
    //Copy data from the source file to the destination file
    while ((numBytesRead = read(srcFile,buffer,BUFFER_SIZE))> 0){
        numBytesWritten = write(destFile,buffer,numBytesRead);
        if (numBytesWritten != numBytesRead){
            printf("Oops! Error: Failed to write data to %s.\n",argv[2]);
            close(srcFile);
            close(destFile);
            exit(EXIT_FAILURE);
        }
    }
    if (numBytesRead == -1){
        printf("Oops! Error: Failed to read souce file %s.\n",argv[1]);
    }
    // Closing both files
    close(srcFile);
    close(destFile);
    printf("Successfully🥳 appended the contents of %s to %s.\n", argv[1],argv[2]);
    return 0;
}
// Resources used: https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/