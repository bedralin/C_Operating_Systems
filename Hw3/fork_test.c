#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>


int main() {
    int pid;
    int status;

    pid = fork();
    printf("pid #1 is %d\n",pid);
    wait(&status);    

    pid = fork();
    printf("pid #2 is %d\n",pid);
    wait(&status);

    pid = fork();
    printf("pid #3 is %d\n",pid);
    wait(&status);

}
