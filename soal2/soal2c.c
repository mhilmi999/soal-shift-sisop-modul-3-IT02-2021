#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{

    int pipe1[2];  // Used to store two ends of first pipe
    int pipe2[2]; // Used to store two ends of second pipe
    // int fd3[2];
    pid_t pid1;
    if (pipe(pipe1) == -1)
    {
        fprintf(stderr, "Gagal Pipe");
        return 1;
    }
    pid_t pid2;
    if (pipe(pipe2) == -1)
    {
        fprintf(stderr, "Gagal Pipe");
        return 1;
    }

    pid1 = fork();
    if (pid1 < 0)
    {
        fprintf(stderr, "Gagal fork");
        return 1;
    }

    if (pid1 == 0)
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            close(pipe1[0]);
            close(pipe2[1]);
            close(pipe2[0]);
            printf("1");
            dup2(pipe1[1], STDOUT_FILENO);

            close(pipe1[1]);
            char *argv[] = {"ps", "aux", NULL};
            execv("/usr/bin/ps", argv);
        }
        else
        {
            printf("2");
            close(pipe1[1]);
            close(pipe2[0]);
            dup2(pipe1[0], STDIN_FILENO);
            dup2(pipe2[1], STDOUT_FILENO);
            close(pipe2[1]);
            close(pipe1[0]);

            char *argv[] = {"sort", "-nrk", "3,3", NULL};
            execv("/usr/bin/sort", argv);
        }
    }
    else
    {
        printf("3");
        close(pipe1[1]);
        close(pipe1[0]);
        dup2(pipe2[0], STDIN_FILENO);
        // dup2(pipe2[1], STDOUT_FILENO);

        close(pipe2[0]);
        close(pipe2[1]);
        // close(pipe2[1]);
        char *argv[] = {"head", "-5", NULL};
        execv("/usr/bin/head", argv);
    }
    // exit(1);

    return 0;
}
