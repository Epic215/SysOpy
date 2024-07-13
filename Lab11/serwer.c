#include <stdio.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include "utils.h"



int sfd_s;
client_t clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;



void *handler_thread(void *arg){
    int cfd_c=*(int*)arg;
    int pos_array=-1;
    char client_name[NAME_SIZE]="";
    ssize_t read_size;
    if ((read_size = read(cfd_c, client_name, sizeof(client_name))) <= 0) {
        perror("recv");
        close(cfd_c);
        return NULL;
    }



    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].cfd = cfd_c;
            strcpy(clients[i].name, client_name);
            clients[i].active = 1;
            clients[i].ping_status = 1;
            pos_array=i;
            break;
        }
    }
    if (pos_array==-1){
        message msg;
        msg.type=FULL;
        strcpy(msg.text,"DENIED");
        write(cfd_c,&msg,sizeof msg);
    }
    pthread_mutex_unlock(&clients_mutex);

    while(1){
        message msg;

        read(cfd_c, &msg, sizeof msg);
        switch(msg.type)
        {
            case ALIVE:
                printf("User %s responded to ping\n",msg.from);
                clients[pos_array].ping_status=1;
                break;
            case ALL2:
                msg.type=MESSAGE;
                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].active && i!=pos_array) {
                        write(clients[i].cfd, &msg, sizeof msg);
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                break;
            case ONE2:
                msg.type=MESSAGE;

                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!strcmp(msg.to,clients[i].name)) {
                        write(clients[i].cfd, &msg, sizeof msg);
                        break;
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                break;

            case LIST:
                strcpy(msg.text, "Users list: ");
                write(cfd_c, &msg, sizeof msg);
                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].active) {
                        strcpy(msg.text, clients[i].name);
                        write(cfd_c, &msg, sizeof msg);
                    }
                }
                pthread_mutex_unlock(&clients_mutex);

                break;
            case STOP:
                clients[pos_array].active=0;
                clients[pos_array].cfd=-1;
                write(cfd_c, &msg,sizeof msg);
                printf("User %s disconnected\n",msg.from);
                break;
            default:
                printf("Unknown type: ignored\n");
                break;
        }
        if(msg.type==STOP){
            break;
        }

    }

    pthread_exit(0);
}

void *pinging_thread(){
    message msg_term, msg_ping;
    msg_term.type=STOP;
    strcpy(msg_term.text,"no response from you");
    strcpy(msg_term.from,"serwer");
    msg_ping.type=ALIVE;
    printf("Sending pings\n");
    while(1){
        sleep(PING_FREQUENCY);
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active && !clients[i].ping_status) {
                printf("User %s not responded to ping - user will be deleted from serwer\n",clients[i].name);
                clients[i].active=0;
                clients[i].cfd=-1;
                write(clients[i].cfd,&msg_term,sizeof msg_term); // upewnienie sie usuniecia uzytkownika (mozliwe ze niepotrzebne)
            }
            if (clients[i].active && clients[i].ping_status) {
                clients[i].ping_status=0;
                write(clients[i].cfd,&msg_ping,sizeof msg_ping);
            }

        }
        pthread_mutex_unlock(&clients_mutex);

    }

}


void handle_signal(){
    printf("\nShutting down serwer...\n");
    message msg;
    msg.type=STOP;
    strcpy(msg.text, "Serwer shut down");


    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            write(clients[i].cfd, &msg, sizeof msg);
            close(clients[i].cfd);
        }
    }
    pthread_mutex_unlock(&clients_mutex);


    close(sfd_s);
    exit(0);

}


int main(int argc, char *argv[]) {

    if (argc != 2){
        printf("Za mala ilosc danych\n");
        exit(-1);
    }
    // handle ctrl+c
    signal(SIGINT, handle_signal);


    //start serwer

    if ((sfd_s = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        perror("socket");
        exit(-1);
    }

    int port;
    port=atoi(argv[1]);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;



    if (bind(sfd_s, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1){
        close(sfd_s);
        perror("bind");
        exit(-1);
    }

    if (listen(sfd_s,10) == -1){
        close(sfd_s);
        perror("listen");
        exit(-1);
    }
    pthread_t ping;
    pthread_create(&ping, NULL, pinging_thread, NULL);
    pthread_detach(ping);

    for(;;){
        int cfd = accept(sfd_s,NULL,NULL);

        if (cfd==-1){
            perror("accept");
            continue;
        }
        pthread_t handler;
        pthread_create(&handler, NULL, handler_thread, &cfd);

        pthread_detach(handler);

    }

}
