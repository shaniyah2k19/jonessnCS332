#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    double *a;
    int N;
    int size;
    long tid;
} ThreadData;

double sum = 0.0;

void *compute(void *arg){
    ThreadData *data = (ThreadData *)arg;
    int myStart, myEnd, myN, i;

    myN = data->N / data->size;
    myStart = data->tid * myN;
    myEnd = myStart + myN;
    if (data->tid == (data->size - 1)) myEnd = data->N;

    double mysum = 0.0;
    for (i = myStart; i < myEnd; i++){
        mysum += data->a[i];
    }
    pthread_mutex_lock(&mutex);
    sum += mysum;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(int argc, char **argv){
    if (argc != 3){
        printf("Usage: %s <# of elements> <# of threads>\n",argv[0]);
        exit(-1);
    }
    int N = atoi(argv[1]);
    int size = atoi(argv[2]);
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * size);
    ThreadData *threadData = (ThreadData *)malloc(sizeof(ThreadData)* size);
    
    double *a = (double *)malloc(sizeof(double) * N);
    for (int i = 0; i < N; i++){
        a[i] = (double)(i + 1);
    }
    

    for(long i = 0; i < size; i++){
        threadData[i].a = a;
        threadData[i].N = N;
        threadData[i].size = size;
        threadData[i].tid = i;

        pthread_create(&tid[i],NULL, compute, &threadData[i]);
    }
    for (int i = 0; i < size; i++){
        pthread_join(tid[i], NULL);
    }
    printf("The total is %g, it should be equal to %g\n", sum, ((double)N * (N+1)) /2);
    free(a);
    free(tid);
    free(threadData);
    return 0;

}

