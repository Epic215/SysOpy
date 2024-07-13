#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_KEY_PATHNAME 12345
#define MAX_MSG_SIZE 256

struct message {
    long msg_type;
    int qid;
    char buf [MAX_MSG_SIZE];
};

struct client_info {
    int client_queue_id;
    int client_id;
};


int main(int argc, char **argv) {

    int server_qid, client_qid;
    int id = getpid();
    struct message message = {1, id, "INIT"};


    key_t client_key = ftok(".", id);

    client_qid = msgget(client_key, IPC_CREAT | 0666);

    if ((server_qid = msgget(SERVER_KEY_PATHNAME, 0666)) == -1) {
        perror ("Client : msgget server_qid");
        exit (1);
    }


    if (msgsnd (server_qid, &message, sizeof(message) - sizeof(long), 0) == -1) {
        perror ("client: msgsnd");
        exit (1);
    }


    if (msgrcv (client_qid, &message, sizeof(message) - sizeof(long), 0, 0) == -1) {
        perror ("client: msgrcv");
        exit (1);
    }
    printf("%s\n",message.buf);


    int pid=fork();
    if (strcmp(message.buf,"SERWER_FULL")!=0){


        if (pid!=0){
            while(1){
                // send message to server
                fgets (message.buf, 198, stdin);
                if (msgsnd (server_qid, &message, sizeof(message) - sizeof(long), 0) == -1) {
                    perror ("client: msgsnd");
                    exit (1);
                }
            }
        }
        else{
            while(1){
                // read response from server
                if (msgrcv (client_qid, &message, sizeof(message) - sizeof(long), 0, 0) == -1) {
                    perror ("client: msgrcv");
                    exit (1);
                }
                printf("%s\n",message.buf);
            }
        }
    }
    else{
        printf("Niestety nie można się polaczyc\n");
    }


}
