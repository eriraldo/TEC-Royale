#include <stdio.h>
#include <stdlib.h>
#include "my_thread.h"
#include <unistd.h>
#include "Warrior.h"
#include <ncurses.h>


my_thread_t threadInFunc;
int threadCreated = 0;
int arrived = 0;
int arrived2 = 0;
#define N 2000
static int list[N];
static int list2[N];
static int S ;

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void stoogesort(int arr[], int l, int h)
{

    // Base Case
    if (l >= h)
        return;

    // If first element is smaller than
    // last element, swap them
    if (arr[l] > arr[h]) {
        int temp = arr[l];
        arr[l] = arr[h];
        arr[h] = temp;
    }

    // If there are more than 2 elements
    // in the array
    if (h - l + 1 > 2) {
        int t = (h - l + 1) / 3;

        // Recursively sort the first
        // 2/3 elements
        stoogesort(arr, l, h - t);

        // Recursively sort the last
        // 2/3 elements
        stoogesort(arr, l + t, h);

        // Recursively sort the first
        // 2/3 elements again
        stoogesort(arr, l, h - t);
    }
}

void *Worker(void *threadid)
{
    int i=5;
    int *returnCode = (int*)malloc(sizeof(int));
    printf("In HILO 2: %d\n",*(int*)threadid);/*
    while(i > 0)
    {
        printf("In Worker:%d\n",i--);
        fflush(stdout);
    }*/

    //stoogesort(list2, 0, S-1);
    printf("Terminó HILO 2: %d\n", *(int*)threadid);
    arrived2 = 1;
    *returnCode = 783;
    return returnCode;
}

void *Worker1(void *threadid)
{
    int i=5;
    int *jjd = (int*)malloc(sizeof(int));
    printf("In HILO 1 :%d\n",*(int*)threadid);
    my_thread_create(&threadInFunc,Worker,(void*)&threadInFunc);

    while(i > 0)
    {
        printf("In Worker1:%d\n",i--);
        fflush(stdout);
    }


    //stoogesort(list, 0, S-1, (void*)&threadid);


    printf("ME SLEEP\n");
    my_thread_sleep(2);
    printf("REAL SHIT\n");
    stoogesort(list, 0, S-1);
    arrived = 1;
    printf("Terminó HILO 1: %d\n", *(int*)threadid);
    threadCreated = 1;
    *jjd = 234;
    return (void*)jjd;
}

void pairing(Warrior* const cmp, Warrior* const depart){
    int* stats1 [4] = {&cmp->vitality, &cmp->attack, &cmp->defense, &cmp->speed};
    int* stats2 [4] = {&depart->vitality, &depart->attack, &depart->defense, &depart->speed};

    int avg1 = *stats1[0]+ *stats1[1]+ *stats1[2]+ *stats1[3];
    int avg2 = *stats2[0]+ *stats2[1]+ *stats2[2]+ *stats2[3];

    if(avg2 < avg1){
        printf("CAN DO :D\n");
        srand(time(NULL));
        int select_attr = rand()%4;
        *stats2[select_attr] = *stats1[select_attr];
        int check_mut = rand()%100;
        if(check_mut<10){
            select_attr = rand()%4;
            printWarrior(depart);
            printf("BEFORE: %d\n", *stats2[select_attr]);
            mutation(stats2[select_attr]);
            printf("A MUTATION HAS OCCURRED\n");
        }
        getLevel(depart);
    }else {
        printf("CAN'T DO D:\n");
    }
}



int main(int argc, char** argv) {


    Warrior warrior;
    Warrior warrior2;
    int mutate = 3;
    mutation(&mutate);
    printf("CHECK: %d\n", mutate);
    int check = initValues(&warrior, 1, 2, 3, 4, "asd");
    if (!check){
        printf("Couldn't initialize warrior object\n");
    }

    check = initValues(&warrior2, 20, 25, 15, 20, "warrior2");
    if (!check){
        printf("Couldn't initialize warrior object\n");
    }

    printWarrior(&warrior);
    getLevel(&warrior);
    printWarrior(&warrior2);
    getLevel(&warrior2);

    pairing(&warrior2,&warrior);
    printf("/////////////////////////////////\n");
    printWarrior(&warrior);


    srand(time(NULL));
    for(size_t i=0; i<N; i++) {
        list[i] = rand() % 1000;
        list2[i] = rand()%1000;
    }

    S = sizeof(list) / sizeof(list[0]);



    //Just a test to check if Round-robin scheduler was working as expected
    int i=5;
    my_thread_t t1;
    my_thread_t t2;
    my_thread_t t3;
    void *status;

    my_thread_init(200);

    my_thread_create(&t1,Worker1,(void*)&t1);
    my_thread_create(&t2,Worker1,(void*)&t2);

    extern Thread_Queue readyQueue;
    Thread_ptr thread = GetThread(readyQueue, t1);
    Thread_ptr thread2 = GetThread(readyQueue, t2);
    my_thread_chsched(thread, 1);
    //Thread_Queue debug = readyQueue;

    my_thread_chsched(thread2, 0);



    while(i > 0)
    {
        printf("In Main:%d\n",i);
        fflush(stdout);
        i--;
    }

    while(!arrived)
    {
        //printf("WAITING\n");
    }

    /*while(!arrived2)
    {

    }*/

    int returnCode = 4;
    printf("\nReturn Code:%d",returnCode);
    printf("\nExiting Main\n");
}