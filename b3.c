#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#define CARS 4
#define TRIPS 20
#define MAXCAREACHTIME 3

sem_t north,south,counter,counterN,counterS;

int totalpass = MAXCAREACHTIME;

int totalCars = 1;

int totalCarsNorth = (CARS / 2) - 1;

int totalCarsSouth = ((CARS + 1) / 2) - 1;//-1;

void *Car(void *arg){

    int trips = TRIPS;

    int value = (long)arg;

    while(trips > 0) {
        if (value % 2 == 0) {

            sem_wait(&north);

            totalCarsNorth++;

            if (totalCarsNorth < totalpass)totalpass = totalCarsNorth;

            if (totalCars != totalpass) {

                totalCarsNorth--;

                sem_post(&north);
            } else {
                totalpass = MAXCAREACHTIME;

                totalCars = 1;

                totalCarsNorth--;

                sem_post(&south);
            }
        } else {

            sem_wait(&south);

            totalCarsSouth++;

            printf("Car %ld passed the bridge from south to north with %d trips left and value = %d\n", (long) arg,
                   trips, totalCarsSouth);

            if (totalCarsSouth < totalpass) totalpass = totalCarsSouth;

            if (totalCars != totalpass) {
                totalCars++;

                totalCarsSouth--;

                sem_post(&south);
            } else {
                totalpass = MAXCAREACHTIME;

                totalCars = 1;

                totalCarsSouth--;

                sem_post(&north);
            }
        }
        trips--;
        value++;
    }
    printf("car %d finished and EXIT\n",arg);
}

int main(int argc, char *argv[]){

    pthread_attr_t attr;

    pthread_t workerid[CARS];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&north,0,1);

    sem_init(&south,0,0);

    sem_init(&counter,0,1);

    sem_init(&counterN,0,1);

    sem_init(&counterS,0,1);

    printf("%d, %d\n", totalCarsSouth, totalCarsNorth );

    int cars = CARS;

    long i = 0;

    for(i = 1; i <= cars; i++)pthread_create(&workerid[i],NULL, Car, (void *) i);

    for(i = 1; i<= cars; i++) pthread_join(workerid[i], NULL);
}
