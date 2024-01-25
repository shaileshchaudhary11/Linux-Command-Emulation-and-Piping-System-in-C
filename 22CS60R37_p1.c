#include <stdio.h>
#include <unistd.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// How to run?
// gcc 22CS60R37_p1.C -o p1
// Provide file names as input
//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    // string variable to store filename from user input
    char filename[10];
    printf("Enter filename: ");
    scanf("%[^\n]s", filename);

    // condition to terminate program
    if (strcmp(filename, "exit") == 0)
    {
        return 0;
    }

    // execlp params:
    // 1: filename/path of command (char*)
    // 2: command syntax
    // rest: arguments to command(if any) and pointers to NULL-terminated character strings

    // Creating child process to open the file
    if (fork() == 0)
    {
        // opening the file using excelp command from unistd.h
        execlp("gedit", "gedit", filename, NULL);
    } 
    // Repeatatively calling the same program until user wants to exit the execution
    execlp("./p1", "./p1", NULL);
}