#include "roundRobin_sched.h"
extern Thread_Queue readyQueue;
extern struct itimerval timeQuantum;
extern long quantum;
Thread_ptr GetCurrentThread_ptr();
extern sigset_t sigProcMask;
int ignoreSignal2 = 0;

void scheduler_rr(int  sigNum)
{
    Thread_Queue debug = readyQueue;
    if(!ignoreSignal2){
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
                //readyQueue = getNextThread(readyQueue, 0);
                MoveForward(readyQueue);
            }
            int lotteryCount = GetLotteryCount(readyQueue);/*
            for(int i = 0; i < lotteryCount; i++){
                MoveForward(readyQueue);
            }*/
            /*if (GetLotteryCount(readyQueue) > 0)
                MoveForward(readyQueue);*/
            //currentNode = GetThread(readyQueue, currentNode->idThread);
            nextNode = GetCurrentThread(readyQueue);
            Thread_ptr head = nextNode;

            while((nextNode!=null) && (nextNode->isBlocked || nextNode->isCompleted || nextNode->scheduler == 1 || nextNode->recently_used == 1))
            {
                if(nextNode->isCompleted)
                {
                    Pop_Queue(readyQueue);
                }
                else
                {
                    MoveForward(readyQueue);
                }
                nextNode = GetCurrentThread(readyQueue);
                if (nextNode == head){
                    setNotUsed(readyQueue, 0);
                    break;
                }
            }
            if(nextNode == null)
            {
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                exit(0);
            }
            if(nextNode != currentNode)
            {
                fflush(stdout);
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
}