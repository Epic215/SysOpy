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





int handler(message msg,struct sockaddr_in client_addr){
    int pos_array=-1;

    if(msg.type==INIT){

        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) {
                clients[i].addr = client_addr;
                strcpy(clients[i].name, msg.from);
                clients[i].active = 1;
                clients[i].ping_status = 1;
                pos_array=i;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
        if (pos_array==-1){
            message mess;
            mess.type=FULL;
            strcpy(mess.text,"DENIED");
            sendto(sfd_s,&mess,sizeof mess,0,(struct sockaddr *) &client_addr,sizeof(client_addr));
        }
        return 0;
    }




    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && memcmp(&clients[i].addr, &client_addr, sizeof(struct sockaddr_in))==0) {
            pos_array=i;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    if (pos_array==-1){
        return -1;
    }

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
                        sendto(sfd_s, &msg, sizeof msg,0,(struct sockaddr *) &clients[i].addr,sizeof(clients[i].addr));
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                break;
            case ONE2:
                msg.type=MESSAGE;

                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!strcmp(msg.to,clients[i].name)) {
                        sendto(sfd_s, &msg, sizeof msg,0,(struct sockaddr *) &clients[i].addr,sizeof(clients[i].addr));
                        break;
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                break;

            case LIST:
                strcpy(msg.text, "Users list: ");
                sendto(sfd_s, &msg, sizeof msg,0,(struct sockaddr *) &client_addr,sizeof(client_addr));
                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].active) {
                        strcpy(msg.text, clients[i].name);
                        sendto(sfd_s, &msg, sizeof msg,0,(struct sockaddr *) &client_addr,sizeof(client_addr));
                    }
                }
                pthread_mutex_unlock(&clients_mutex);

                break;
            case STOP:
                clients[pos_array].active=0;
                sendto(sfd_s, &msg,sizeof msg,0,(struct sockaddr*) &client_addr,sizeof(client_addr));
                printf("User %s disconnected\n",msg.from);
                break;
            default:
                printf("Unknown type: ignored\n");
                break;


    }
    return 0;

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
                sendto(sfd_s,&msg_term,sizeof msg_term,0,(struct sockaddr*) &clients[i].addr,sizeof(clients[i].addr)); // upewnienie sie usuniecia uzytkownika (mozliwe ze niepotrzebne)

            }
            if (clients[i].active && clients[i].ping_status) {
                clients[i].ping_status=0;
                sendto(sfd_s,&msg_ping,sizeof msg_ping,0,(struct sockaddr*) &clients[i].addr,sizeof(clients[i].addr));
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
            sendto(sfd_s, &msg, sizeof msg,0,(struct sockaddr*) &clients[i].addr,sizeof(clients[i].addr));
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

    if ((sfd_s = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket");
        exit(-1);
    }

    int port;
    port=atoi(argv[1]);

    struct sockaddr_in addr_s, addr_c;
    memset(&addr_s, 0, sizeof(struct sockaddr_in));
    memset(&addr_c, 0, sizeof(struct sockaddr_in));
    socklen_t addr_len = sizeof(addr_c);


    addr_s.sin_family=AF_INET;
    addr_s.sin_port=htons(port);
    addr_s.sin_addr.s_addr = INADDR_ANY;



    if (bind(sfd_s, (struct sockaddr *) &addr_s, sizeof(struct sockaddr_in)) == -1){
        close(sfd_s);
        perror("bind");
        exit(-1);
    }

    pthread_t ping;
    pthread_create(&ping, NULL, pinging_thread, NULL);
    pthread_detach(ping);

    for(;;){
        message msg;
        recvfrom(sfd_s,&msg,sizeof(msg),0,(struct sockaddr*) &addr_c,&addr_len);
        handler(msg,addr_c);


    }

}
