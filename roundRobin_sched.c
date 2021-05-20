#include "roundRobin_sched.h"
extern Thread_Queue readyQueue;
extern struct itimerval timeQuantum;
extern long quantum;
Thread_ptr GetCurrentThread_ptr();
extern sigset_t sigProcMask;

void scheduler_rr(int  sigNum)
{
    Thread_Queue debug = readyQueue;
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(GetThreadCount(readyQueue) == 1)
    {
        Thread_ptr currentNode = GetCurrentThread(readyQueue);
        if(currentNode->isCompleted)
        {
            Pop_Queue(readyQueue);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            exit(0);
        }
    }
    else if(GetThreadCount(readyQueue) > 0)
    {
        Thread_ptr currentNode= GetCurrentThread(readyQueue);
        int currentNodeCompleted = 0;
        if(currentNode != null)
        {
            Thread_ptr nextNode;
            if(currentNode->isCompleted)
            {
                currentNodeCompleted = 1;
                Pop_Queue(readyQueue);
            }
            else
            {
                MoveForward(readyQueue);
            }

            cleanQueue(1);
            nextNode = GetCurrentThread(readyQueue);

            if(nextNode == null)
            {
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                exit(0);
            }
            if(nextNode != currentNode)
            {
                timeQuantum.it_value.tv_usec = quantum;
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                if(currentNodeCompleted)
                {
                    nextNode->recently_used = 1;
                    setcontext(&(nextNode->context));
                }
                else
                {
                    nextNode->recently_used = 1;
                    swapcontext(&(currentNode->context),&(nextNode->context));
                }
            }
        }
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
}