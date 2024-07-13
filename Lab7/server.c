#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_KEY_PATHNAME 12345
#define MAX_MSG_SIZE 256
#define MAX_CLIENT_COUNT 10

struct message {
    long msg_type;
    int qid;
    char buf [MAX_MSG_SIZE];
};

struct client_info {
    int client_queue_id;
    int client_id;
};

struct client_info clients[MAX_CLIENT_COUNT];
int count_clients=0;

int main(int argc, char **argv) {

    int server_qid;
    struct message message;

    if ((server_qid = msgget (SERVER_KEY_PATHNAME, IPC_CREAT | 0666)) == -1) {
        perror ("Server: msgget");
        exit (1);
    }

    while (1) {
        // read an incoming message
        if (msgrcv (server_qid, &message, sizeof(message) - sizeof(long), 0, 0) == -1) {
            perror ("Server: msgrcv");
            exit (1);
        }

        if (strcmp(message.buf, "INIT") == 0){

            if (count_clients>=MAX_CLIENT_COUNT) {
                key_t client_key = ftok(".", message.qid);
                int client_queue_id = msgget(client_key, 0666);
                sprintf(message.buf, "SERWER_FULL");
                // send reply message to client
                if (msgsnd(client_queue_id, &message, sizeof(message) - sizeof(long), 0) == -1) {
                    perror("Server: msgsnd INIT response");
                    exit(1);
                }
                printf("Brak miejsca\n ");

            }
            else{
                key_t client_key = ftok(".", message.qid);
                clients[count_clients].client_queue_id = msgget(client_key, 0666);
                clients[count_clients].client_id = message.qid;
                message.msg_type=1;
                sprintf(message.buf, "Przyjeto, identyfikator klienta: %d", clients[count_clients].client_id);
                // send reply message to client
                if (msgsnd(clients[count_clients].client_queue_id, &message, sizeof(message) - sizeof(long), 0) == -1) {
                    perror("Server: msgsnd INIT response");
                    exit(1);
                }

                count_clients++;
            }


        }
        else{
            printf("Dostalem komunikat od klienta %d: %s\n", message.qid, message.buf);
            for (int i = 0; i < count_clients; ++i) {
                if (clients[i].client_queue_id != -1 && clients[i].client_id != message.qid) {
                    if (msgsnd(clients[i].client_queue_id, &message, sizeof(message) - sizeof(long), 0) == -1) {
                        perror("Server: msgsnd to client");
                    }
                }
            }
        }

    }
    msgctl(server_qid, IPC_RMID, NULL);




    return 0;
}