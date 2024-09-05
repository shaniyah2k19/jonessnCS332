#include <stdio.h>
// Function for insertion sort
void insetionSort(int arr[], int N){
    int i, currLoc, temp;
    // looping through each of the elements starting from the second element
    for (i = 1; i < N; i++){
        currLoc = i;
        // Comparing the current elements with the ones before 
        while(currLoc > 0 && arr[currLoc - 1] > arr[currLoc]){
            // Swapping the arr[currLoc] and arr[cuuLoc-1]
            temp = arr[currLoc];
            arr[currLoc] = arr[currLoc - 1];
            arr[currLoc - 1] = temp;
            // Moving back 1
            currLoc--;
        }
    }
} 
int main(){
    int N;
    //Prompting the user to enter the number of elements
    printf("Enter number of elements in the array: ");
    scanf("%d",&N);
    // Declaring array size N
    int arr[N];
    // Going over each element of the array
    for (int i = 0; i < N; i++){
        printf("Enter the element %d of array: ",i+1);
        scanf("%d", &arr[i]);// Storing input
    }
    // Displaying array before stroing
    printf("The array is: ");
    for(int i = 0; i < N - 1; i++){
        printf("%d,", arr[i]); //Printing without a comma
    }
    printf("%d\n", arr[N - 1]);
    insetionSort(arr,N); // Sorting array with the insertionSort function
    printf("The sorted array is: ");
    
    for(int i = 0; i < N - 1; i++){
        printf("%d, ", arr[i]);
    }
    printf("%d\n", arr[N-1]); 
    return 0;
}
