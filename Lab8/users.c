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


#include "settings.h"

void generate_random_string(char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Zla liczba argumentow\n");
        return -1;
    }
    long N = atoi(argv[1]);

    int shm_fd = shm_open(SHARED_MEMORY_, O_RDWR, S_IRUSR | S_IWUSR);
    if(shm_fd < 0){
        perror("shm_open");
        return -1;
    }
    printers* printers_ = mmap(NULL, sizeof(printers), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    char text_buffer[MAX_TEXT_LEN] = {0};
    int seed = 0;
    for(int i=0;i<N;i++){
        seed += 10;
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        if(pid == 0){
            srand(seed);
            while(1){
                generate_random_string(text_buffer, 10);

                int printer_index = -1;
                for (int j = 0; j < printers_->printers_len; j++) {
                    int val;
                    sem_getvalue(&printers_->printers[j].sem, &val);
                    if(val > 0) {
                        printer_index = j;
                        break;
                    }
                }
                if(printer_index == -1){
                    printer_index = rand() % printers_->printers_len;
                }
                if(sem_wait(&printers_->printers[printer_index].sem) == -1){
                    perror("sem_wait");
                    return -1;
                }
                memcpy(printers_->printers[printer_index].text, text_buffer, 256);
                printers_->printers[printer_index].text_len = strlen(text_buffer);

                printers_->printers[printer_index].state = WORKING;

                printf("Uzytkownik %d wyslal: %s\n", i, printers_->printers[printer_index].text);
                fflush(stdout);
                sleep(rand()%3+1);
            }
            exit(0);
        }
    }

    while(wait(NULL) > 0) {};

    munmap(printers_, sizeof(printers));
}