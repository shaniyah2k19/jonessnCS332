#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

//primeOrFactorial(n)
// check to see whether or not the number is prime.
// if the number is less or equal to 1 then it is consider not prime
bool isNumberPrime(int n){
    if (n <= 1){
        return false;
    }
    for (int i = 2; i * i <=n; i++){
        if ( n % i == 0){
            return false;
        }
    }           
    return true;
}

// function needed to calculate factorial
int factor(int n){
    if (n == 0 || n ==1){
        return 1;
    }
    return n * factor(n - 1);
}

//Function to print if the number is prime or factorial
void primeOrFactorial(int n){
    if ( n <= 1){
        printf("Invalid Input\n");
    } else if (isNumberPrime(n)){
        printf("Prime Number\n");
    } else {
        int fac = factor(n);
        printf("Factorial: %d\n",fac);
    }
}

//UABIndexSum(arr,size)
//calculates total of the items at index that match their values
int UABIndexSum(int arr[], int size){
    int sum = 0;
    for (int i = 0; i < size; i++){
        if (arr[i] == i){
            sum += arr[i];//Add to sum if value equals index
        }
    }
    return sum;
}

//replaceEvenWithZero(arr)
// This function with iterate through the array
// Check if the current element is even and replace with 0 if so
void replaceEvenWithZero(int arr[],int size){
    for (int i = 0; i < size; i++){
        if (arr[i] % 2 == 0){
            arr[i] = 0;//Replace even number with 0
        }
    }
}

//evenSquare(n)
// checks to if the number is a perfect square & square root even
bool evenSquare(int n){
    if (n < 0){
        return false;
    }
    int i = 0;
    while (i * i <= n){
        if (i * i == n){
            return(i % 2 == 0);
        }
        i++;
    }
    return false;
}

//countVowels(s)
// count the number of vowels in a string
int countVowels(const char* s) {
    int count = 0;
    while (*s){
        char cha = tolower(*s);// Convert to lowercase
        if (strchr("aeiou",cha)){
            count++;// Only increment if character is a vowel
        }
        s++;
    }
    return count;
}

int main(){

    //Testing primeOrFactorial
    printf("Result of primeOrFactorial(5):");
    primeOrFactorial(5);
    printf("Result of primeOrFactorial(4):");
    primeOrFactorial(4);
    printf("Result of primeOrFactorial(1):");
    primeOrFactorial(1);
    printf("Result of primeOrFactorial(7):");
    primeOrFactorial(7);
    printf("Result of primeOrFactorial(-3):");
    primeOrFactorial(-3);

    //Testing UABIndexSum 
    int arr1[] = {0,2,2,4};
    printf("UABIndexSum([0,2,2,4]): %d\n",UABIndexSum(arr1,4));
    int arr2[] = {3,1,0,4};
    printf("UABIndexSum([3,1,0,4]): %d\n",UABIndexSum(arr2,4));

    //Testing replaceEvenWithZero 
    int arr3[] = {1,2,3,4};
    replaceEvenWithZero(arr3,4);
    printf("replaceEvenWithZero([1,2,3,4]): [");

    //Printing the modified array 
    for(int i =0; i < 4; i++){
        printf("%d",arr3[i]);
        if (i < 3){
            printf(",");
        }
    }
    printf("]\n");

    //Testing evenSquare 
    printf("evenSquare(16): %d\n",evenSquare(16));
    printf("evenSquare(15): %d\n",evenSquare(15));
    printf("evenSquare(25): %d\n",evenSquare(25));
    printf("evenSquare(36): %d\n",evenSquare(36));

    //Testing countVowels
    printf("countVowels(\"Hello World\": %d\n", countVowels("Hello World"));
    printf("countVowels(\"UAB CS\": %d\n", countVowels("UAB CS"));
    printf("countVowels(\"Python\": %d\n", countVowels("Python"));
    printf("countVowels(\"aeiou\": %d\n", countVowels("aeiou"));
}