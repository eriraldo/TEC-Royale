#include "DataStructures.h"
#include "scheduler.h"
extern Thread_Queue readyQueue;
ucontext_t notifierContext;
extern struct itimerval timeQuantum;
extern long timeIntervalInMSec;
Thread_ptr GetCurrentThread_ptr();
extern sigset_t sigProcMask;
int ignoreSignal = 0;

void scheduler(int  sigNum)
{
    //Check if only element on queue is main()
    if(!ignoreSignal){
      sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(GetThreadCount(readyQueue) == 1)
    {
        //only one node nothing to do
        Thread_ptr currentNode = GetCurrentThread(readyQueue);
        if(currentNode->isCompleted)
        {
            Pop_Queue(readyQueue);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            exit(0);
        }
        //END CONDITION OF USER THREADS
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
                //Remove the Node if it has completed Execution
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
                    //This means the thread is not completed but blocked
                    //we need to move forward
                    MoveForward(readyQueue);
                }
                nextNode = GetCurrentThread(readyQueue);
            }
            if(nextNode == null)
            {
                //All Threads Completed
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                exit(0);
            }
            if(nextNode != currentNode)
            {
                //if After removing all the completed Threads
                //of there are more then one node remaining
                //then Context Switch
                //Swap the Context between currentNode and nextNode


                //DEBUGGING

                fflush(stdout);
                //ENDDEBUGGING'
                timeQuantum.it_value.tv_usec = timeIntervalInMSec;
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                if(currentNodeCompleted)
                {
                    //printf("Seeting Context as the current thread completed  %ld\n",nextNode->idThread);
                    setcontext(&(nextNode->context));
                }
                else
                {
                    //printf("Swaping %ld with %ld\n",currentNode->idThread,nextNode->idThread);
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
    //Set the isCompleted Node of the current node as 1
    Thread_ptr currentNode = GetCurrentThread_ptr();
    WaitThreadList blockedNode = currentNode->waitingThreads;

    //we will first unblock all the other threads waiting for this
    //thread to complete
    while(blockedNode != null)
    {
        blockedNode->node->isBlocked = 0;
        blockedNode = blockedNode->next;
    }

    //DEBUGGING
    printf("Thread Completed:%ld\n",currentNode->idThread);
    //ENDDEBUGGING
    

    //then we mark this thread as complete
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
