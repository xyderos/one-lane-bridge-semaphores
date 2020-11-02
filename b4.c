#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#define CARS 100
#define TRIPS 20
#define MAXCAREACHTIME 100

sem_t north,south,counter;

int totalCars = CARS;

int totalPass = MAXCAREACHTIME;

int counterNorth, counterSouth;

int totalCarsNorth = (CARS / 2);

int totalCarsSouth = ((CARS + 1) / 2);

void *Car(void *arg){

    int trips = TRIPS;

    int value = (long)arg;

    int tmp1;

    int tmp2;

    while(trips > 0){
        if(value%2 == 0){

            sem_getvalue(&north,&tmp1);

            if(totalCarsNorth == 0) sem_post(&south);

            else{
                sem_wait(&north);

                counterNorth++;

                trips--;

                value++;

                if(totalCarsNorth < totalPass) totalPass = totalCarsNorth;

                totalCarsNorth--;

                if(trips==0) totalCarsSouth--;

                printf("Car%ld FROM NORTH, trips =  %d left  ,counterNorth = %d \n",(long)arg, trips, counterNorth);

                if(counterNorth >= totalPass){

                    counterNorth = 0;

                    totalCarsSouth++;

                    totalPass = MAXCAREACHTIME;

                    sem_post(&south);
                }
                else{

                    totalCarsSouth++;

                    sem_post(&north);
                }
            }
        }else{
            sem_getvalue(&north,&tmp2);

            if(totalCarsSouth == 0) sem_post(&north);

            else{
                sem_wait(&south);

                counterSouth++;

                trips--;

                value++;

                if(totalCarsSouth < totalPass) totalPass = totalCarsSouth;

                totalCarsSouth--;

                if(trips==0)totalCarsNorth--;

                printf("Car%ld FROM SOUTH , trips =  %d left  ,counterSouth = %d\n",(long)arg, trips, counterSouth);

                if(counterSouth >= totalPass){

                    printf("Antalet bilar som fick åka = %d\n", totalPass);

                    counterSouth = 0;

                    totalCarsNorth++;

                    totalPass = MAXCAREACHTIME;

                    sem_post(&north);
                }else{
                    totalCarsNorth++;
                    sem_post(&south);
                }
            }
        }
    }
    sem_wait(&counter);

    totalCars--;

    sem_getvalue(&north,&tmp1);

    sem_getvalue(&south,&tmp2);

    printf("\nCar%d FINISHED AND EXIT, There are %d cars left, {N=%d, S=%d},{CarN = %d, CarS = %d}\n\n", arg, totalCars, tmp1, tmp2, totalCarsNorth, totalCarsSouth);

    if(totalCarsNorth == 0) sem_post(&south);

    if(totalCarsSouth == 0) sem_post(&north);

    sem_post(&counter);
}

int main(int argc, char *argv[]){

    pthread_attr_t attr;

    pthread_t workerid[CARS];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&north,0,1);

    sem_init(&south,0,0);

    sem_init(&counter,0,1);

    printf("%d, %d\n", totalCarsSouth, totalCarsNorth );

    int cars = CARS;

    long i = 0;

    for(i = 1; i <= cars; i++) pthread_create(&workerid[i],NULL, Car, (void *) i);

    for(i = 1; i<= cars; i++) pthread_join(workerid[i], NULL);
}