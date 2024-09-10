#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readStrings(char **arr, int N);
void insertionSort(char **arr, int N);
void printStrings(char **arr, int N);

int main(){
    int N;
    // Prompting the user for number of strings
    printf("Enter the number of strings: ");
    scanf("%d", &N);
    // Allocating the memory for an aray of pointers to strings
    char **arr = (char **) malloc(N * sizeof(char *));
    if (arr == NULL){
        printf("Memory allocation has failed. \n");
        return 1;
    }
    // Reading strings from user
    readStrings(arr,N);

    //Displaying strings before insertionsort
    printf("The strings are: \n");
    printStrings(arr,N);

    //Sorting the strings using insertion sort
    insertionSort(arr,N);

    //Displaying strings after sorting
    printf("Sorted strings: \n");
    printStrings(arr,N);
    
    // Free allocated memory
    for (int i = 0; i < N; i++){
        free(arr[i]);
    }
    free(arr);
    return 0;
}
//Function to read strings from user
void readStrings(char **arr, int N){
    for (int i = 0; i < N; i++){
        arr[i] = (char *)malloc(100 * sizeof(char));
        if (arr[i] == NULL){
            printf("Memory allocation failed. \n");
            exit(1);
        }
        printf("Enter string %d: ", i + 1);
        scanf("%s", arr[i]); // Reading the strings into allocated memory
       
    }
}
// Function to sort to the array of string using insertion sort 
void insertionSort(char **arr, int N){
    char *temp;
    for (int i = 1; i < N; i++){
        temp = arr[i];
        int j = i - 1;
        //Moving the elements of arr[0..1-1] that greater than temp to one ahead
        while ( j >= 0 && strcmp(arr[j],temp) > 0){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp; // Placing temp in the right position
    }
}
// Function to print array of strings
void printStrings(char **arr, int N){
    for (int i = 0; i < N; i++){
        printf("%s\n", arr[i]); // Printing each string
    }
}