#include <stdio.h>
#include <unistd.h>

int main()
{

    __pid_t pid;
    pid = getpid();
    printf("%u", pid);

    __pid_t ppid;
    ppid = getppid();
    printf("\n%u", ppid);

    __pid_t fpid;
    printf("\n Before fork 1");
    fpid = fork();
    if (fpid == -1)
    {
        perror("unsucessful \n");
        return 1;
    }
    printf("\n After fork 2\n");
    return 0;
}