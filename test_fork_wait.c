/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Da Vit, 23521802
# File: test_fork_wait.c
######################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    printf("Child process, pid=%d\n",pid);
    else {
        wait(NULL);
        printf("Parent Proces, pid=%d\n",pid);
    }
    exit(0);
}