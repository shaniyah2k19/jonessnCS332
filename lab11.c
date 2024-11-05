#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char command[256];
    FILE *pipe;
    char output[1024];

    while (1){
        printf("Enter command: ");
        if (fgets(command, sizeof(command), stdin) == NULL){
            perror("Error reading command");
            continue;
        }
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command,"quit")== 0){
            printf("Exiting program bye bye!....\n");
            break;
        }
        pipe = popen(command,"r");
        if(pipe == NULL){
            perror("Uh Oh 😰 popen failed!");
            continue;
        }
        while(fgets(output, sizeof(output),pipe)!= NULL){
            printf("%s",output);
        }
        if(pclose(pipe) == -1){
            perror("Uh Oh 😰 pclose failed!");
        }
    }
    return 0;
}