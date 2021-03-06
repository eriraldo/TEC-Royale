//
// Created by ericksalazargranera on 5/19/21.
//
#define STACKSIZE 1024*8
#ifndef TECROYALE_DATASTRUCTURES_H
#define TECROYALE_DATASTRUCTURES_H



#define null NULL

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef long my_thread_t;

typedef struct Node
{
    int isCompleted;
    long quantum;
    my_thread_t idThread;
    ucontext_t context;
    struct Node *next;
    int tickets;
    int scheduler;
    int recently_used;
    int special;

} Thread,*Thread_ptr;


typedef struct completedNode
{
    my_thread_t idThread;
    void **returnStatus;
    struct completedNode *nextThread;
} *CompletedThread_ptr;

typedef struct Thread_Queue
{
    Thread_ptr head;
    Thread_ptr head_parent;
    long count;
} *Thread_Queue;

typedef struct Completed_Thread_Queue
{
    struct completedNode *node;
    long count;
} *Completed_Queue;

Thread_ptr GetCurrentThread(Thread_Queue queue);
int moveHead(Thread_Queue queue);
int GetThreadCount(Thread_Queue queue);
int GetNextThreadId();
Thread_ptr NewThread();
Thread_Queue GetThreadQueue();
int Pop_Queue(Thread_Queue queue);
int Push_Queue(Thread_Queue queue,Thread_ptr node);
int PushToCompleted(Completed_Queue queue,CompletedThread_ptr node);
Completed_Queue GetCompletedQueue();
CompletedThread_ptr GetCompletedNode();
void PopNode_Queue(Thread_Queue queue, Thread_ptr node);
Thread_ptr GetThread(Thread_Queue queue, long idThread);
int GetLotteryCount(Thread_Queue queue);
Thread_ptr cloneThread(Thread_ptr oldThread);
void printThreads(Thread_Queue queue);
void setNotUsed(Thread_Queue queue, int sched);






#endif //TECROYALE_DATASTRUCTURES_H
