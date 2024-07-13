#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int count=0;
pthread_mutex_t santa = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reindeers[9]={
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER
};
pthread_mutex_t waiting = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;
pthread_t threads[10]; // reindeers 0-8, santa 9


void* reindeer_t(void* arg){
    int id = *(int*)arg;

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_mutex_lock(&reindeers[id]);
    while(1){
        sleep(rand() % 6 + 5);

        pthread_mutex_lock(&waiting);

        printf("Renifer: czeka %d reniferów na Mikołaja, %d\n",count,id);

        count++;
        if (count==9){

            printf("Renifer: budzę Mikołaja, %d\n", id);
            pthread_cond_broadcast(&cond);
            count=0;
        }


        pthread_mutex_unlock(&waiting);



        pthread_mutex_lock(&reindeers[id]);
        printf("Renifer: lecę na wakacje %d\n", id);
    }


}

void* santa_t(){
    for(int i=0; i<4;i++){
        pthread_cond_wait(&cond,&santa);
        printf("Mikołaj: budzę się\n");

        printf("Mikołaj: dostarczam zabawki\n");
        sleep(rand() % 3 + 2);

        for (int j=0;j<9;j++){
            pthread_mutex_unlock(&reindeers[j]);
        }

        printf("Mikołaj: zasypiam\n");


    }
    for(int i=0; i<9;i++){
        pthread_cancel(threads[i]);
    }
    return NULL;


}

int main() {

    int id[9];

    pthread_create(&threads[9],NULL,santa_t,NULL);
    for (int i=0; i<9;i++){
        id[i]=i;
        pthread_create(&threads[i],NULL,reindeer_t,&id[i]);
    }

    for (int i=0; i<10;i++){
        pthread_join(threads[i],NULL);
    }

    return 0;
}
