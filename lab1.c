#include <stdio.h>
/*
Instructions to complie and run in GitHub Codespaces:
1. Save the file with .c at the end
2. Open a terminal up in GitHub Codespaces
3. Complile the code using gcc –o lab1 lab1.c
4. Run the file using ./lab1
5. Enter a number when prompt
*/
int main(){
    // Using the C function scan f to read the givennumber variable
    int given_number, c, prime = 1; // Here we are defining the variable 
    printf("Enter a number: ");
    scanf("%d", &given_number);

    // Using the if, else, and for to check whether the given number is prime or not
    if (given_number <= 1){
        prime = 0;
    } else {
        for (c = 2; c <= given_number / 2; c++){
            if (given_number % c == 0){
                prime = 0;
                break;
            }
        }
    }
    if(prime){
        printf("This number is prime.\n");
    } else {
        printf("This number is not prime. \n");
    }
    return 0;
}