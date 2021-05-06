#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{

    int fd[2];  // Used to store two ends of first pipe
    int fd2[2]; // Used to store two ends of second pipe
    // int fd3[2];
    pid_t p, p2;
    int status4;
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    p = fork();
    if (p < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }

    if (p == 0)
    {
        p2 = fork();
        if (p2 == 0)
        {
            close(fd[0]);
            close(fd2[1]);
            close(fd2[0]);
            printf("1");
            dup2(fd[1], STDOUT_FILENO);

            close(fd[1]);
            char *argv[] = {"ps", "aux", NULL};
            execv("/usr/bin/ps", argv);
        }
        else
        {
            printf("2");
            close(fd[1]);
            close(fd2[0]);
            dup2(fd[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);
            close(fd2[1]);
            close(fd[0]);

            char *argv[] = {"sort", "-nrk", "3,3", NULL};
            execv("/usr/bin/sort", argv);
        }
    }
    else
    {
        printf("3");
        close(fd[1]);
        close(fd[0]);
        dup2(fd2[0], STDIN_FILENO);
        // dup2(fd2[1], STDOUT_FILENO);

        close(fd2[0]);
        close(fd2[1]);
        // close(fd2[1]);
        char *argv[] = {"head", "-5", NULL};
        execv("/usr/bin/head", argv);
    }
    // exit(1);

    return 0;
}
