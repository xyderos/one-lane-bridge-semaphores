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

sem_t N,S,counter,counterN,counterS;

int counter1;

int tPass = MAXCAREACHTIME;

int cNorth, cSouth;

void *car(void *arg) {

    printf("car %ld\n", (long) arg);

    int trips = TRIPS;

    int value = (long) arg;

    while (trips >= 0) {

        if (value % 2 == 0) {

            sem_wait(&counterN);

            cNorth++;

            sem_post(&counterN);

            sem_wait(&N);

            int col;

            sem_getvalue(&N, &col);

            printf("%d\n", col);

            printf("Car %ld passed the bridge from north to south & with %d trips left\n", (long) arg, trips);

            sem_wait(&counter);

            counter1++;

            if (cNorth < MAXCAREACHTIME)tPass = cNorth;

            if (counter1 == tPass) {

                printf("Car counter north %d\n", cNorth);

                counter1 = 0;

                for (int i = 0; i < tPass; i++) {
                    sem_post(&counter);
                    sem_post(&S);
                }

            } else sem_post(&counter);

            sem_wait(&counterN);

            cNorth--;

            tPass = MAXCAREACHTIME;

            sem_post(&counterN);
        } else {
            sem_wait(&counterS);

            cSouth++;

            sem_post(&counterS);

            sem_wait(&S);

            printf("Car %ld passed the bridge from south to north with %d trips left\n", (long) arg, trips);

            sem_wait(&counter);

            counter1++;

            if (cSouth < MAXCAREACHTIME) tPass = cSouth;

            if (counter1 == tPass) {
                printf("Car counter south %d\n", cSouth);
                counter1 = 0;
                for (int i = 0; i < tPass; i++) {
                    sem_post(&counter);
                    sem_post(&N);
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

int main(int argc, char *argv[]){

    pthread_attr_t attr;

    pthread_t workerid[CARS];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&N, 0, MAXCAREACHTIME);

    sem_init(&S, 0, 0);

    sem_init(&counter,0,1);

    sem_init(&counterN,0,1);

    sem_init(&counterS,0,1);

    int cars = CARS;

    long i = 0;

    for(i = 1; i <= cars; i++)pthread_create(&workerid[i], NULL, car, (void *) i);

    for(i = 1; i<= cars; i++)pthread_join(workerid[i], NULL);

}