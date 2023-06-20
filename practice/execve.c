#include <stdio.h>
#include <unistd.h>

int main()
{
    __pid_t pid;
    char *argv[] = {"/usr/bin/ls", "-l", NULL};
    pid = fork();
    if (pid == -1)
    {
        return -1;
    }
    if (pid == 0)
    {
        int val = execve(argv[0], argv, NULL);

        if (val == -1)
        {
            perror("Error");
        }
    }
    else
    {
        wait(NULL);
        printf("Done with Execve\n");
    }

    return 0;
}