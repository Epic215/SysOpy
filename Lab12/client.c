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
#include <stdbool.h>
#include "utils.h"

int sfd_c;
pthread_t sending, receiving;
char* client_id;
socklen_t addr_len;
struct sockaddr_in addr_srv;

void *sending_thread(){
    pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while(1){
        char buffer[512];

        size_t x = read(STDIN_FILENO, &buffer, 512);
        buffer[x] = '\0';

        const char delim[] = " \t\n";

        int idx = 0;

        char *token;
        token = strtok(buffer, delim);
        MessageType type = -1;

        char other_nickname[MESSAGE_LEN] = {};
        char text[MESSAGE_LEN] = {};

        int broke=0;

        while (token) {
            switch(idx)
            {
                case 0:
                    if (strcmp(token, "LIST") == 0) {type = LIST; idx+=3;}
                    else if (strcmp(token, "2ALL") == 0) {type = ALL2; idx+=2;}
                    else if (strcmp(token, "2ONE") == 0) {type = ONE2;idx++;}
                    else if (strcmp(token, "STOP") == 0) {type = STOP; idx+=3;}
                    else {
                        broke = 1;
                    }
                    break;
                case 1:
                    strcpy(other_nickname, token);
                    other_nickname[strlen(token)] = 0;
                    idx++;
                    break;
                case 2:
                    strcat(text, token);
                    strcat(text, " ");
                    break;
                default:
                    broke = 1;
                    break;
            }

            if (broke) break;
            token = strtok(NULL, delim);
        }
        if (type == ALL2 && strlen(text)==0) broke=1;
        if (type == ONE2 && (strlen(text)==0 || strlen(other_nickname)==0)) broke=1;

        if (broke) {
            printf("You used command in a wrong way\n");
            if (type == LIST) printf("List should only have command LIST\n");
            else if (type == ALL2) printf("2ALL should look like this: 2ALL string\n");
            else if (type == ONE2) printf("2ONE should look like this: 2ONE client_id string\n");
            else if (type == STOP) printf("Stop should only have command STOP\n");
            else {
                broke = 1;
                printf("Invalid command\n");
            }
            continue;
        }



        message msg;
        msg.type = type;
        strcpy(msg.to, other_nickname);
        strcpy(msg.text, text);
        strcpy(msg.from, client_id);

        sendto(sfd_c, &msg, sizeof msg,0,(struct sockaddr *) &addr_srv,addr_len);

    }




}

void *receiving_thread(){
    pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1){
        message msg;

        recvfrom(sfd_c, &msg, sizeof msg,0,(struct sockaddr *) &addr_srv,&addr_len);
        switch(msg.type)
        {
            case ALIVE:
                strcpy(msg.from, client_id);
                sendto(sfd_c, &msg, sizeof msg,0,(struct sockaddr *) &addr_srv,addr_len);
                break;
            case MESSAGE:
                printf("Message: %s, from user: %s\n",msg.text,msg.from);
                break;
            case LIST:
                printf("%s\n",msg.text);
                break;
            case STOP:
                printf("DISCONNECTED\n");
                raise(SIGINT);
                break;

            case FULL:
                printf("DENIED: FULL SERWER\n");
                raise(SIGINT);
                break;
            default:
                printf("Unknown type: ignored\n");
                break;
        }

    }



}

void handle_signal(){
    message msg;
    msg.type = STOP;
    strcpy(msg.from,client_id);

    sendto(sfd_c, &msg, sizeof msg,0,(struct sockaddr *) &addr_srv,addr_len);
    pthread_cancel(sending);
    pthread_cancel(receiving);
    close(sfd_c);
    exit(0);

}

int main(int argc, char* argv[]) {

    if (argc != 4){
        printf("Za mala ilosc danych\n");
        exit(-1);
    }
    // handle ctrl+c
    signal(SIGINT, handle_signal);


    //connect serwer
    sfd_c = socket(AF_INET,SOCK_DGRAM,0);
    if (sfd_c== -1) {
        perror("socket");
    }

    client_id = argv[1];
    char* server_ip = argv[2];
    int port = atoi(argv[3]);


    memset(&addr_srv, 0, sizeof(struct sockaddr_in));
    struct in_addr in;
    if(inet_aton(server_ip,&in)==0){
        perror("aton");
    }
    addr_srv.sin_family=AF_INET;
    addr_srv.sin_port=htons(port);
    addr_srv.sin_addr=in;
    addr_len = sizeof(addr_srv);

    message msg;
    strcpy(msg.from,client_id);
    msg.type=INIT;



    sendto(sfd_c,&msg, sizeof msg,0,(struct sockaddr *) &addr_srv,addr_len);

    pthread_create(&sending, NULL, sending_thread, NULL);
    pthread_create(&receiving, NULL, receiving_thread, NULL);
    pthread_detach(sending);
    pthread_detach(receiving);

    pthread_join(sending, NULL);
    pthread_join(receiving, NULL);


    pthread_exit(NULL);
}
