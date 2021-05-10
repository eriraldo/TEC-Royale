#include "DataStructures.h"
#include "scheduler.h"
extern Thread_Queue readyQueue;
ucontext_t notifierContext;
extern struct itimerval timeQuantum;
extern long quantum;
Thread_ptr GetCurrentThread_ptr();
extern sigset_t sigProcMask;
int ignoreSignal = 0;

void scheduler(int  sigNum)
{
    if(!ignoreSignal){
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
            nextNode = GetCurrentThread(readyQueue);

            while((nextNode!=null) && (nextNode->isBlocked || nextNode->isCompleted))
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
                    setcontext(&(nextNode->context));
                }
                else
                {
                    swapcontext(&(currentNode->context),&(nextNode->context));
                }
            }
        }
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    }
}

void threadCompletedNotifier()
{
    Thread_ptr currentNode = GetCurrentThread_ptr();
    WaitThreadList blockedNode = currentNode->waitingThreads;
    while(blockedNode != null)
    {
        blockedNode->node->isBlocked = 0;
        blockedNode = blockedNode->next;
    }
    printf("Thread Completed:%ld\n",currentNode->idThread);
    currentNode->isCompleted = 1;

    raise(SIGPROF);

}

ucontext_t setupCompleteContext()
{
    static int hasContextCreated;
    
    if(!hasContextCreated)
    {
        getcontext(&notifierContext);
        notifierContext.uc_link = 0;
        notifierContext.uc_stack.ss_sp = malloc(STACKSIZE);
        notifierContext.uc_stack.ss_size = STACKSIZE;
        notifierContext.uc_stack.ss_flags= 0;
        makecontext( &notifierContext, (void (*) (void))&threadCompletedNotifier, 0);
        hasContextCreated = 1;
    }
    return notifierContext;
}
