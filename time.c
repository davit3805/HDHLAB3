/*######################################
# University of Information Technology #
# IT007 Operating System
#
# Nguyen Da Vit, 23521802 #
# File: time.c
#
######################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    __pid_t pid;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    pid = fork();
    if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", argv[1], (char *)NULL);
        return 0;
    } else if (pid > 0)
    {
        wait(NULL);
        gettimeofday(&end, NULL);
        double t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("Thời gian thực thi: %f giây\n", t);
    }
    return 0;
}