#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

void log_command(const char *command, time_t start_time, time_t end_time){
    FILE *log_file = fopen("output.log","a");
    if(log_file == NULL){
        perror("uh oh failed to open log file!");
        exit(EXIT_FAILURE);
    }
    
    char start_time_str[26];
    char end_time_str[26];
    //Convert time to readable form
    ctime_r(&start_time, start_time_str);
    ctime_r(&end_time, end_time_str);
    // Remove the newline character from ctime_r output
    start_time_str[strcspn(start_time_str,"\n")] = 0;
    end_time_str[strcspn(end_time_str,"\n")] = 0;

    fprintf(log_file,"%s\t%s\t%s\n",command,start_time_str, end_time_str);
    fclose(log_file);
}
int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr,"Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE *input_file = fopen(argv[1],"r");
    if(input_file == NULL){
        perror("uh oh Failed to open input file");
        return EXIT_FAILURE;
    }
    
    char command[MAX_LINE_LENGTH];
    while(fgets(command, sizeof(command),input_file) != NULL){
        // Remove newline character from command
        command[strcspn(command,"\n")] = 0;
        pid_t pid = fork();
        if (pid < 0){
            perror("uh oh Failed Fork");
            fclose(input_file);
            return EXIT_FAILURE;
        }
        if (pid == 0){
            //Split command into two arguments
            char *args[MAX_LINE_LENGTH / 2 + 1];
            char *token = strtok(command," ");
            int i = 0;
            while(token != NULL && i < (MAX_LINE_LENGTH / 2)){
                args[i++] = token;
                token = strtok(NULL," ");
            }
            args[i] = NULL; // Null teriminiate the argument list
            execvp(args[0],args);
            perror("Exec Failed"); // If exec returns, then it has failed
            exit(EXIT_FAILURE);
        }
        else{
            //Parent process
            time_t start_time = time(NULL);
            wait(NULL); // Wait for child to finish
            time_t end_time = time(NULL);
            //Logging the command and its execution times
            log_command(command, start_time,end_time);
        }
    }
    fclose(input_file);
    return EXIT_SUCCESS;
}