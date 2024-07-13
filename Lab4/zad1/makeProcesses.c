#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Zla ilosc argumentow");
        return -1;
    }
    long il=strtol(argv[1], NULL, 10);
    for (int i=0; i<il;i++){
        int id=fork();
        if(id==0){
            printf("Identyfikator procesu macierzystego: %d\n",getppid());
            printf("Identyfikator procesu wlasnego: %d\n",getpid());
            return 0;
        }
    }
    while(wait(NULL)!=-1);
    printf("argv[1]: %s\n",argv[1]);
    return 0;
}
