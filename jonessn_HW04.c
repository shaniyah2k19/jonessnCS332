#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PARENT_THREADS 3
#define NUM_CHILD_THREADS 10
#define NUMBERS_PER_PARENT_THREAD 500
#define NUMBERS_PER_CHILD_THREAD 150

int pipe_fd[2];
pthread_mutex_t pipe_mutex;

void *parent_generate_numbers(void *arg){
    int thread_id = *(int *)arg;
    int numbers[NUMBERS_PER_PARENT_THREAD];

    for (int i = 0; i< NUMBERS_PER_PARENT_THREAD; i++){
        numbers[i] = rand() % 1001;
    }
    pthread_mutex_lock(&pipe_mutex);
    write(pipe_fd[1],numbers,sizeof(numbers));
    pthread_mutex_unlock(&pipe_mutex);
    printf("Parent %d: These are the Numbers that are written to pipe. \n", thread_id);
    return NULL;
}
void *child_calculate_sum(void *arg){
    int thread_id = *(int *)arg;
    int numbers[NUMBERS_PER_CHILD_THREAD];
    int sum = 0;

    read(pipe_fd[0], numbers, sizeof(numbers));

    for (int i = 0; i < NUMBERS_PER_CHILD_THREAD; i++){
        sum += numbers[i];
    }
    printf("Child Thread %d: Sum = %d. \n", thread_id,sum);
    int *result = malloc(sizeof(int));
    *result = sum;
    pthread_exit(result);
}
void signal_notify_child(int sig){
    if (sig == SIGUSR1){
        printf("Child process received signal to start.\n");
    }
}
int main(){
    pthread_t parent_threads[NUM_PARENT_THREADS];
    pthread_t child_threads[NUM_CHILD_THREADS];
    int parent_threads_ids[NUM_PARENT_THREADS];
    int child_threads_ids[NUM_CHILD_THREADS];
    int child_sums[NUM_CHILD_THREADS];
    int total_sum = 0;

    if (pipe(pipe_fd)== -1){
        perror("Pipe has Failed");
        exit(1);
    }
    pthread_mutex_init(&pipe_mutex,NULL);

    pid_t pid = fork();

    if (pid == -1){
        perror("Fork failed");
        exit(1);
    }
    if (pid == 0){
        signal(SIGUSR1, signal_notify_child);
        pause();

        for (int i = 0; i < NUM_CHILD_THREADS; i++){
            child_threads_ids[i] = i;
            pthread_create(&child_threads[i],NULL, child_calculate_sum, &child_threads_ids[i]);
        }
        for (int i = 0; i < NUM_CHILD_THREADS; i++){
            int *result;
            pthread_join(child_threads[i], (void **)&result);
            child_sums[i] = *result;
            total_sum += child_sums[i];
            free(result);
        }
        double average = (double)total_sum / NUM_CHILD_THREADS;
        FILE *output_file = fopen("output.txt","w");
        if (output_file){
            fprintf(output_file, "Average Sum: %.2f\n",average);
            fclose(output_file);
        }
        printf("Child Process: Average Sum = %.2f. Written to output.txt.\n",average);
        exit(0);
    } else {
        for (int i = 0; i < NUM_PARENT_THREADS; i++){
            parent_threads_ids[i] = i;
            pthread_create(&parent_threads[i],NULL,parent_generate_numbers, &parent_threads_ids[i]);
        }
        for (int i = 0; i < NUM_PARENT_THREADS; i++){
            pthread_join(parent_threads[i],NULL);
        }
        printf("The Parent Process: Sending signal to child to process. \n");
        kill(pid, SIGUSR1);
        wait(NULL);
        printf("The Parent Process: Execution complete. \n");
    }
    pthread_mutex_destroy(&pipe_mutex);
    return 0;

}