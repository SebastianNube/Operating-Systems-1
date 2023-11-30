#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 5

int myCount = 0;
int count = 0;


pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t myCond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t myCond2 = PTHREAD_COND_INITIALIZER;

void * producer(){
    //for(int i = 0; i < 10; i++){
    while(myCount < 9){
        pthread_mutex_lock(&myMutex);
        printf("PRODUCER: myMutex locked\n");
        while(count == SIZE){
            printf("PRODUCER: waiting on myCond1\n");
            pthread_cond_wait(&myCond1, &myMutex);
        }
        count = SIZE;
        printf("PRODUCER: signaling myCond2\n");
        pthread_cond_signal(&myCond2);
        pthread_mutex_unlock(&myMutex);
        printf("PRODUCER: myMutex unlocked\n");
        if(myCount == 10){
            return NULL;
        }
    }
    return NULL;
}

void * consumer(){
    int value = myCount;
    while(value != 10){
        pthread_mutex_lock(&myMutex);
        printf("CONSUMER: myMutex locked\n");
        while(count == 0){
            printf("CONSUMER: waiting on myCond2\n");
            pthread_cond_wait(&myCond2, &myMutex);
        }
        count = 0;
        myCount = myCount + 1;
        printf("myCount: %d -> %d\n", myCount - 1, myCount);
        printf("CONSUMER: signaling myCond1\n");
        pthread_cond_signal(&myCond1);
        pthread_mutex_unlock(&myMutex);
        printf("CONSUMER: myMutex unlocked\n");
        if(myCount == 10){
            return NULL;
        }
    }
}


int main(){
    printf("PROGRAM START\n");
    pthread_t p,c;

    
    pthread_create(&p, NULL, producer, NULL);
    sleep(1);
    pthread_create(&c, NULL, consumer, NULL);
    printf("CONSUMER THREAD CREATED\n");

    pthread_join(p, NULL);
    pthread_join(c, NULL);
    printf("PROGRAM END\n");
}