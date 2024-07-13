#define PING_FREQUENCY 30
#define MAX_CLIENTS 10
#define MESSAGE_LEN 512
#define NAME_SIZE 64

typedef struct {
    struct sockaddr_in addr;
    int active;
    int ping_status;
    char name[NAME_SIZE];
} client_t;

typedef enum {
    INIT,
    LIST,
    ALL2,
    ONE2,
    STOP,
    ALIVE,
    MESSAGE,
    FULL
} MessageType;

typedef struct {
    MessageType type;
    char text[MESSAGE_LEN];
    char from[NAME_SIZE];
    char to[NAME_SIZE];
} message;
