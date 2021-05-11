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



void setTicket(Thread_ptr thread, int tickets){
    thread->tickets = tickets;
}


