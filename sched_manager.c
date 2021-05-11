#include "sched_manager.h"
ucontext_t notifierContext;
Thread_ptr GetCurrentThread_ptr();
bool select_sched = false;
extern Thread_Queue readyQueue;
extern Thread_Queue lotteryQueue;

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

void manage(int sigNum){
    select_sched = !select_sched;
    struct Thread_Queue debug = *readyQueue;
    if(select_sched && GetLotteryCount(readyQueue) > 0) {
        scheduler_lottery(0);
    }
    else {
        scheduler_rr(0);
    }
}

void swapSched(Thread_ptr thread, int sched){/*
    Thread_ptr threadClone = cloneThread(thread);
    PopNode_Queue(readyQueue, thread);
    PopNode_Queue(lotteryQueue, thread);*/

    switch (sched) {
        case 0:
            thread->tickets = 0;
           // Push_Queue(readyQueue, threadClone);
            thread->scheduler = 0;
            break;
        case 1:
            thread->tickets = 10;
            thread->scheduler = 1;
            //Push_Queue(lotteryQueue, threadClone);
            break;
        default:
            thread->tickets = 0;
            //Push_Queue(readyQueue, threadClone);
            thread->scheduler = 0;
            break;
    }
}

void setTicket(Thread_ptr thread, int tickets){
    thread->tickets = tickets;
}

Thread_ptr cloneThread(Thread_ptr oldThread){
    Thread_ptr newthread = (Thread_ptr)malloc(sizeof(Thread));
    newthread->idThread = oldThread->idThread;
    newthread->quantum = oldThread->quantum;
    newthread->context = oldThread->context;
    newthread->isBlocked = oldThread->isBlocked;
    newthread->isCompleted = oldThread->isCompleted;
    newthread->waitingThreads = oldThread->waitingThreads;
    newthread->hasNoStackSpaceAllocated = oldThread->hasNoStackSpaceAllocated;
    return newthread;

}
