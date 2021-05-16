#define STACKSIZE 1024*8

#ifndef DATASTRCUTURES_H
#define	DATASTRCUTURES_H

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
    int hasNoStackSpaceAllocated;
    int isCompleted;
    int isBlocked;
    long quantum;
    my_thread_t idThread;
    ucontext_t context;
    struct Node *next;
    struct WaitThreadList *waitingThreads;
    int tickets;
    int scheduler;
    int recently_used;
    int special;

} Thread,*Thread_ptr;


typedef struct DeadNode
{
    my_thread_t idThread;
    void **returnStatus;
    struct DeadNode *nextThread;
} *CompletedThread_ptr;

typedef struct Thread_Queue
{
    Thread_ptr head;
    Thread_ptr head_parent;
    long count;
} *Thread_Queue;

typedef struct WaitThreadList
{
    Thread_ptr node;
    struct WaitThreadList *next;
} *WaitThreadList;



typedef struct Completed_Thread_Queue
{
    struct DeadNode *node;
    long count;
} *Completed_Queue;

Thread_ptr GetCurrentThread(Thread_Queue queue);
int MoveForward(Thread_Queue queue);
int GetThreadCount(Thread_Queue queue);
int GetNextThreadId();
void FreeNode(Thread_ptr node);
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
Thread_Queue getNextThread(Thread_Queue queue, int sched);
Thread_ptr cloneThread(Thread_ptr oldThread);
void printThreads(Thread_Queue queue);
void setNotUsed(Thread_Queue queue, int sched);


#endif	/* DATASTRCUTURES_H */

