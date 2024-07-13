#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

volatile int choice=-1;
volatile int changes=0;

void handle_message(int sig, siginfo_t *sip, void *ptr){
    printf("Potwierdzenie odbioru\n");
    choice=sip->si_value.sival_int;
    changes++;
    kill(sip->si_pid,SIGUSR1);

}

int main() {

    printf("Catcher PID: %d\n", getpid());

    struct sigaction sa;
    sa.sa_sigaction=handle_message;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    while(1){
        switch(choice){
            case 1:
                for (int i = 0 ; i <=100; i++){
                    printf("%d ",i);
                }
                printf("\n");
                choice=-1;
                break;
            case 2:
                printf("Liczba otrzymanych zadan zmiany trybu pracy od początku dzialania programu: %d\n",changes);
                choice=-1;
                break;
            case 3:
                printf("Opcja 3 zakonczenie dzialania catchera\n");
                return 0;
            default:
                break;
        }
    }
}
