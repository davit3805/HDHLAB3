#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_KEY 1234 // Khóa để tạo shared memory
#define MAX_SIZE 100 // Kích thước tối đa của chuỗi Collatz

void collatz(int n, int *buffer, int *length) {
    *length = 0;
    while (n > 1) {
        buffer[(*length)++] = n;
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }
    buffer[(*length)++] = 1; 
}

int main(int argc, char *argv[]) {
    
    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("n phải là số nguyên dương.\n");
        return 1;
    }

    // Tạo shared memory
    int shmid = shmget(SHM_KEY, sizeof(int) * (MAX_SIZE + 1), IPC_CREAT | 0666);

    // Gắn shared memory vào không gian địa chỉ
    int *shared_mem = (int *)shmat(shmid, NULL, 0);

    __pid_t pid = fork();

    if (pid == 0) 
    {
        // Tính chuỗi Collatz và lưu vào shared memory
        int *buffer = shared_mem + 1; // Lưu chuỗi Collatz
        int length = 0;
        collatz(n, buffer, &length);

        // Lưu độ dài chuỗi Collatz vào shared memory
        shared_mem[0] = length;

        shmdt(shared_mem); // Hủy gắn shared memory
        exit(0);
    } else {
        wait(NULL);

        // Đọc dữ liệu từ shared memory
        int length = shared_mem[0];
        int *buffer = shared_mem + 1;

        // In chuỗi Collatz
        printf("Collatz sequence: ");
        for (int i = 0; i < length; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\n");

        //Giải phóng bộ nhớ
        shmdt(shared_mem);      
        shmctl(shmid, IPC_RMID, NULL); 
    }

    return 0;
}
