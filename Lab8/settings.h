#ifndef SETTINGS_H
#define SETTINGS_H
#include <semaphore.h>

#define SHARED_MEMORY_ "printer_shared_memory"
#define MAX_PRINTERS 10
#define MAX_TEXT_LEN 256

typedef enum {
    READY=0,
    WORKING=1
} state_t;


typedef struct {
    sem_t sem;
    char text[MAX_TEXT_LEN];
    size_t text_len;
    state_t state;
} printer_buf;

typedef struct {
    printer_buf printers[MAX_PRINTERS];
    size_t printers_len;
} printers;

#endif