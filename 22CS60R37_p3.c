#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

//////////////////////////////////////////////////////////////////////
// How to run?
// gcc 22CS60R37_p3.C -o p3
// Provide commands as an input
//////////////////////////////////////////////////////////////////////

int main()
{
    // string to store input command
    char *command;
    command = (char *)malloc(INT_MAX * sizeof(char));
    // wpid: store parent pid
    pid_t wpid;
    int status = 0;

    // forking a child process to run command
    if (fork() == 0)
    {
        // prompt text
        printf("shell>");
        // reading command from terminal
        scanf("%[^\n]s", command);
        char *token = NULL;
        // splitting command
        token = strtok(command, " ");
        // // // // pwd command // // // //
        // checking whether the input contains pwd or not and also whether it has & in it or not
        if ((strstr(token, "pwd") != NULL && strstr(token, "&") != NULL) || strcmp(token, "pwd") == 0)
        {
            // string to store oresent working directory
            char cwd[PATH_MAX];
            // getting present working directory
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                printf("Current working dir: %s\n", cwd);
            }
            else
            {
                perror("getcwd() error");
            }
        }
        // // // // ls command // // // //
        else if ((strstr(token, "ls") != NULL && strstr(token, "&") != NULL) || strcmp(token, "ls") == 0)
        {

            pid_t wpid;
            int status = 0;
            // DIR pointer to store directory
            DIR *mydir;
            // pointer to store file/dir
            struct dirent *myfile;
            struct stat mystat;
            // string to store pwd
            char cwd[PATH_MAX];
            // string to store file/dir name one by one
            char buf[512];
            // opening the pwd usinf opendir()
            mydir = opendir(getcwd(cwd, sizeof(cwd)));
            // reading filrname from dir stored in mydir
            while ((myfile = readdir(mydir)) != NULL)
            {
                // printing the files and dir in pwd
                printf(buf, "%s/%s", cwd, myfile->d_name);
                stat(buf, &mystat);
                printf(" %s\n", myfile->d_name);
            }
        }
        // // // // cd command // // // //
        else if ((strstr(token, "cd") != NULL && strstr(token, "&") != NULL) || strcmp(token, "cd") == 0)
        {
            // reading the dir name to change
            token = strtok(NULL, "\n");
            pid_t wpid;
            int status = 0;
            // string to store pwd
            char cwd[4096];
            if (chdir(token) == -1) // change directory
            {
                fputs("No directory found!!", stdout);
            }
            else
            {
                // current dir path after changing dir
                printf("Current directory:");
                fputs(getcwd(cwd, 4096), stdout);
                printf("\n");
            }
        }
        // // // // mkdir command // // // //
        else if ((strstr(token, "mkdir") != NULL && strstr(token, "&") != NULL) || strcmp(token, "mkdir") == 0)
        {
            // reading the dir name to create
            token = strtok(NULL, "\n");
            // creating new dir in pwd
            if (mkdir(token, 0777) < 0)
            {
                printf("Not able to create directory!");
            }
        }
        // // // // exit command // // // //
        else if (strstr(command, "exit") != NULL)
        {
            kill(getpid(), SIGINT);
        }
        else
        {
            printf("Invalid command!!\n");
        }
    }
    // if commnd contains & in it then parent process wait until all childs complete their execution
    if (strstr(command, "&") == NULL && strstr(command, "exit") == NULL)
    {
        while ((wpid = wait(&status)) > 0)
            ;
    }
    // iteratively running the program
    execlp("./p3", "./p3", NULL);
}
