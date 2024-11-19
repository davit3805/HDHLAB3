/*######################################
# University of Information Technology
# IT007 Operating System
#
# Nguyen Da Vit, 23521802
# File: bai4.c
######################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10
#define SHM_KEY 12345

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int total;
    int stop;
} SharedMemory;

void producer(SharedMemory *shared) {
    while (1) {
        if (shared->stop) break;

        // Kiểm tra xem buffer có chỗ trống không
        if ((shared->in + 1) % BUFFER_SIZE == shared->out) continue;

        // Tạo một số ngẫu nhiên từ 10 đến 20
        int item = rand() % 11 + 10;
        shared->buffer[shared->in] = item;
        printf("Producer: Produced %d\n", item);
        shared->in = (shared->in + 1) % BUFFER_SIZE;

        sleep(1);  // Thời gian sản xuất
    }
}

void consumer(SharedMemory *shared) {
    while (1) {
        if (shared->stop) break;

        // Kiểm tra xem buffer có dữ liệu không
        if (shared->in == shared->out) continue; 

        // Đọc dữ liệu từ buffer
        int item = shared->buffer[shared->out];
        shared->out = (shared->out + 1) % BUFFER_SIZE;
        shared->total += item;
        printf("Consumer: Consumed %d, Total: %d\n", item, shared->total);

        // Nếu tổng lớn hơn 100, dừng cả hai tiến trình
        if (shared->total > 100) shared->stop = 1;
        
        sleep(1);  // Thời gian tiêu thụ
    }
}

int main() 
{
    srand(time(NULL));  // Khởi tạo seed cho số ngẫu nhiên

    // Tạo shared memory
    int shm_id = shmget(SHM_KEY, sizeof(SharedMemory), IPC_CREAT | 0666);

    // Gắn shared memory
    SharedMemory *shared = (SharedMemory *)shmat(shm_id, NULL, 0);

    // Khởi tạo 
    shared->in = 0;
    shared->out = 0;
    shared->total = 0;
    shared->stop = 0;

    __pid_t pid = fork();

    if (pid == 0) consumer(shared); 
    else if (pid > 0) 
    {
        producer(shared);
        wait(NULL);

        // Giải phóng bộ nhớ
        shmdt(shared);
        shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}
