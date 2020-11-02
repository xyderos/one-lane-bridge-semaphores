#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#define CARS 10
#define TRIPS 20
#define MAXCAREACHTIME 5

sem_t north,south,counter,counterN,counterS;

int cnt;

int tPass = MAXCAREACHTIME;

int cNorth, cSouth;

void *Car(void *arg){

    printf("car %ld\n",(long)arg );

    int trips = TRIPS;

    int value = (long)arg;

    while(trips >= 0) {
        if (value % 2 == 0) {

            sem_wait(&north);

            printf("Car %ld passed the bridge from north to south & with %d trips left\n", (long) arg, trips);

            sem_wait(&counter);

            int getvalue;

            sem_getvalue(&north, &getvalue);

            if (getvalue >= 0)tPass = getvalue;

            cnt++;

            if (cnt == tPass) {
                printf("Car counter north %d\n", cNorth);
                cnt = 0;
                for (int i = 0; i < tPass; i++) {

                    sem_post(&counter);

                    sem_post(&south);
                }
                tPass = MAXCAREACHTIME;

            } else sem_post(&counter);


        } else {

            sem_wait(&south);

            printf("Car %ld passed the bridge from south to north with %d trips left\n", (long) arg, trips);

            sem_wait(&counter);

            int getvalue;

            sem_getvalue(&south, &getvalue);

            if (getvalue >= 0) tPass = getvalue;

            cnt++;

            if (cnt == tPass) {
                printf("Car counter south %d\n", cSouth);
                cnt = 0;
                for (int i = 0; i < tPass; i++) {
                    sem_post(&counter);
                    sem_post(&north);
                }
            } else sem_post(&counter);

            sem_wait(&counterS);

            cSouth--;

            tPass = MAXCAREACHTIME;

            sem_post(&counterS);
        }
        trips--;

        value++;
    }
}
void *Bridge(void *arg){

}

int main(int argc, char *argv[]){

    pthread_attr_t attr;

    pthread_t workerid[CARS];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&north,0,MAXCAREACHTIME);

    sem_init(&south,0,0);

    sem_init(&counter,0,1);

    sem_init(&counterN,0,1);

    sem_init(&counterS,0,1);

    int cars = CARS;

    long i = 0;

    pthread_create(&workerid[0],NULL ,Bridge, (void *) i);

    for(i = 1; i <= cars; i++) pthread_create(&workerid[i],NULL, Car, (void *) i);

    for(i = 1; i<= cars; i++)pthread_join(workerid[i], NULL);
}