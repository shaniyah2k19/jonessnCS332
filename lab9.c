#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sig_quit(int signo){
    printf("Parent proces received SIGQUIT. Exiting...\n");
    exit(0);
}
int main(int argc, char **argv){
    pid_t pid;
    int status;

    if (argc < 2){
        printf("Usage: %s <command> [args]\n",argv[0]);
        exit(-1);
    }
    //Parent process ignores SIGINT and SIGTSTP
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    // Parent process handles SIGQUIT
    signal(SIGQUIT, sig_quit);
    pid = fork();

    if (pid == 0){
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(-1);
    } else if (pid > 0){
        printf("Wait for the child process to terminate \n");
        wait(&status);
        if (WIFEXITED(status)){
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not terminate normally!\n");
        }
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}