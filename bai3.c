/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Da Vit, 23521802
# File: bai3.c
#
######################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

__pid_t pid;

void on_sigint(){
    printf("\ncount.sh has stopped!\n");
    if (pid != 0) kill(pid, SIGKILL);  
} 

int main()
{
    printf("Welcome to IT007, I am 23521802!\n");
    signal(SIGINT, on_sigint);
    pid = fork();
    if (pid == 0)
        system("./count.sh 120");
    else 
    if (pid > 0) wait(NULL);
    return 0;
}