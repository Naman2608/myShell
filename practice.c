#include <stdio.h>
#include <unistd.h>

int main()
{

    // __pid_t pid;
    // pid = getpid();
    // printf("%u", pid);

    __pid_t ppid;
    // ppid = getppid();
    // printf("\n%u", ppid);

    __pid_t fpid;
    printf("\n Before fork 1");
    fpid = fork();
    if (fpid == -1)
    {
        perror("unsucessful \n");
        return 1;
    }
    // child process
    if (fpid == 0)
    {
        sleep(10);
        printf("\nI am the child\n");
    }
    // parent process
    else
    {
        wait(NULL);
        ppid = getppid();
        printf("\nI am the Parent Ppid = %u ", ppid);
    }
    printf("\n After fork 2\n");
    return 0;
}