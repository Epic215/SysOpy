#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
void received(){
    printf("Otrzymalem potwierdzenie otrzymania, koniec dzialania\n");
}

int main(int argc, char *argv[]) {
    printf("Sender PID: %d\n", getpid());
    if (argc!=3){
        printf("Zla ilosc argumentow");
        return -1;
    }
    long target_pid=strtol(argv[1], NULL, 10);
    long choice=strtol(argv[2], NULL, 10);

    union sigval sv;
    signal(SIGUSR1,received);

    sv.sival_int = choice;
    sigqueue(target_pid, SIGUSR1, sv);

    sigset_t myset;
    sigfillset(&myset);
    sigdelset(&myset,SIGUSR1);
    sigdelset(&myset,SIGINT); // just for safety

    sigsuspend(&myset);
    return 0;


}
