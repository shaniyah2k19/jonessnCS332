##Destination
This program copies the contents of source file to a destination file. If the
destination file does not exist, it will be created. If it does exist , the
contents of the source file will be appended to it.

##Compilation
To compile the run, use the following command:
Replace 'lab4.c' with the name of your C source file. 

## Execution
To run the program, use the following command:

Replace 'input.txt' with the path to your source file 
Replace 'output.txt' with the path to your destination file

**Please Note**
- 'input.txt' : The source file must already exist
- 'output.txt' : If it exists, the contents of 'input.txt' will append to it.
If it doesn't exist then it will be created.

##Testing
Create 'input.txt' and 'output.txt' with some content

Run the program
gcc lab4.c -o lab4
./lab4
./lab4 input.txt output.txt
cat output.txt
