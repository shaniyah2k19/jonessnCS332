#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_MAX 1024

int main(){
    char command[COMMAND_MAX];
    FILE *fp;

    while(1){
        printf("Enter command: ");
        if (fgets(command, COMMAND_MAX,stdin) == NULL){
            perror("fgets");
            exit(EXIT_FAILURE);
        }
        command[strcspn(command,"\n")] = '\0';

        if (strcmp(command,"quit") == 0){
            printf("Exiting program...bye!\n");
            break;
        }
        if ((fp = popen(command,"r")) == NULL){
            perror("popen");
            continue;
        }
        char output[COMMAND_MAX];
        while(fgets(output, sizeof(output),fp) != NULL){
            printf("%s",output);
        }
        if(pclose(fp) == -1){
            perror("pclose");
        }
    }
    return 0;
}