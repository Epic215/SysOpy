#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "settings.h"


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Zla liczba argumentow\n");
        return -1;
    }
    long M = atoi(argv[1]);

    if (M > MAX_PRINTERS) {
        printf("Too much, maximum number %d\n", MAX_PRINTERS);
        return -1;
    }


    int shm_fd = shm_open(SHARED_MEMORY_, O_RDWR | O_CREAT ,  S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(printers)) == -1) {
        perror("ftruncate");
        return 1;
    }

    printers * printers_ = mmap(NULL, sizeof(printers), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    memset(printers_->printers, 0, sizeof(printers));
    printers_->printers_len = M;

    for(int i=0;i<M;i++){
        pid_t pid = fork();
        sem_init(&printers_->printers[i].sem, 1, 1);
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        if(pid == 0){
            while(1){
                if(printers_->printers[i].state == WORKING){
                    printf("Drukarka %d dziala\n", i);
                    printf("Drukarka %d: ", i);
                    for(int j=0;j<printers_->printers[i].text_len;j++){
                        printf("%c", printers_->printers[i].text[j]);
                        sleep(1);
                    }
                    printf("\n");
                    fflush(stdout);
                    printers_->printers[i].state = READY;
                    sem_post(&printers_->printers[i].sem);
                }

            }
            exit(0);

        }
    }

    while(wait(NULL) > 0) {}

    munmap(printers_, sizeof(printers));
    shm_unlink(SHARED_MEMORY_);
    return 0;

}