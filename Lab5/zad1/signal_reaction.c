#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <string.h>
#include <signal.h>
void handler(int sig_no) {
    printf("Otrzymany sygnal SIGUSR1 numer: %d.\n", sig_no);
}
int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Zla ilosc argumentow\n");
        return -1;
    }
    if (strcmp("handler",argv[1]) == 0){
        signal(SIGUSR1,handler);
        raise(SIGUSR1);
    }
    else if (strcmp("mask",argv[1])==0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_SETMASK, &mask, NULL);
        raise(SIGUSR1);

        sigset_t pend;
        sigpending(&pend);

        printf("Czy sygnal oczekuje? : %i\n", sigismember(&pend, SIGUSR1));
    }else if (strcmp("ignore",argv[1])==0){
        signal(SIGUSR1,SIG_IGN);
        raise(SIGUSR1);
    } else {
        signal(SIGUSR1,SIG_DFL);
        raise(SIGUSR1);
    }
    return 0;
}
