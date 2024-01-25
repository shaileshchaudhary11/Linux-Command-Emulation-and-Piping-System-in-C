#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

//////////////////////////////////////////////////////////////////////
// How to run?
// gcc 22CS60R37_p2.C -o p2
// Provide commands as an input
//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    // string to store input command
    char *command;
    command = (char *)malloc(100 * sizeof(char));
    // file pointer to store parent pid
    FILE *pf;
    pf = fopen("ppid.txt", "a");
    fprintf(pf, "%d\n", getpid());
    // prompt text
    printf("file>");
    // reading command from user
    scanf("%[^\n]s", command);
    // token: string to store splitted command
    char *token = NULL;
    // token will have the command
    token = strtok(command, " ");
    pid_t wpid;
    int status = 0;
    // // // // open command // // // //
    if (strcmp(token, "open") == 0)
    {
        while (token != NULL)
        {
            // getting the filenames from command args
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                // forking child process to open file using execlp and gedit
                if (fork() == 0)
                {
                    // file pointer to store pid of child for future use
                    FILE *fptr;
                    fptr = fopen("pid.txt", "a");
                    // writing child pid and filename in pid.txt
                    fprintf(fptr, "%d %s\n", getpid(), token);
                    fclose(fptr);
                    // execlp: Function to execute system calls in Linux
                    // args:
                    // Path to command
                    // syntax of command
                    // args to command (, separated)
                    execlp("gedit", "gedit", "-s", token, NULL);
                }
            }
        }
    }
    // // // // quit command // // // //
    else if (strcmp(command, "quit") == 0)
    {
        // file pointer to open pid.txt
        FILE *fptr, *pptr;
        fptr = fopen("pid.txt", "w");
        // clearing up the pids of opened files
        fprintf(fptr, "%s", "");
        fclose(fptr);
        kill(getpid(), SIGINT);
    }
    // // // // close command // // // //
    else if (strcmp(token, "close") == 0)
    {
        while (token != NULL)
        {
            // getting the filenames from command args
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                // file pointer to read pid.txt
                FILE *fptr;
                // string to store a line from file
                char str[25];
                // string to store spiltted strings
                char *t = NULL;
                fptr = fopen("pid.txt", "r");
                int id;
                // reading a line from file
                int a = fscanf(fptr, "%d %s\n", &id, str);
                while (a != EOF)
                {
                    // reading id from scanned string
                    // t = strtok(str, " ");
                    // id = atoi(t);
                    // // reading the filename with pid=id
                    // t = strtok(NULL, "\n");
                    // it filename is same as filename from command hen kill the process with pid=id
                    if (strcmp(str, token) == 0)
                    {
                        // forking child process to close file
                        if (fork() == 0)
                        {
                            kill(id, SIGSEGV);
                        }
                        break;
                    }
                    a = fscanf(fptr, "%d %s\n", &id, str);
                }
            }
        } 
    }
    else
    {
        printf("Invalid command!");
    }
    execlp("./p2", "./p2", NULL);
}
