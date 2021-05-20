//
// Created by ericksalazargranera on 5/19/21.
//
#include "realtime_sched.h"
#include "my_thread.h"

ucontext_t notifierContext;
Thread_ptr GetCurrentThread_ptr();
bool select_sched = false;
extern Thread_Queue readyQueue;
extern Thread_Queue lotteryQueue;
int ticks = 0;
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

void disableSpecialThreads(){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->special == 1){
            nextNode->scheduler = -1;
        }
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
}

int getRR(){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;
    int rrCount = 0;
    while((nextNode!=null))
    {
        if(nextNode->scheduler == 0 && nextNode->recently_used != 1)
            rrCount+= 1;
        nextNode = nextNode->next;
        if (nextNode == head)
            break;
    }
    return rrCount;
}

void manageSpecialThread(){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->special == 1) {
            double rrCount = getRR() + 1;
            double tickets = getTickets() + 50;
            if (1/rrCount > 50/tickets){
                my_thread_chsched(nextNode, 0);
            }else{
                my_thread_chsched(nextNode, 1);
                nextNode->tickets = 50;
            }
        }
        nextNode = nextNode->next;
        if (nextNode == head)
            break;
    }
}

int getSpecialThread(){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;
    int specialCount = 0;
    while((nextNode!=null))
    {
        if(nextNode->special == 1)
            specialCount+= 1;
        nextNode = nextNode->next;
        if (nextNode == head)
            break;
    }
    return specialCount;
}

void cleanQueue(int sched){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;

    while((nextNode!=null) && (nextNode->isBlocked || nextNode->isCompleted || nextNode->scheduler == sched || nextNode->recently_used == 1))
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
}

void manage(int sigNum){
    int special = getSpecialThread();
    int test = ticks;
    if (special > 0 && ticks == 0){
        disableSpecialThreads();
        manageSpecialThread();
        ticks = 51; //added so it doesn't change sched to priority threads all the time
    }

    select_sched = !select_sched;
    struct Thread_Queue debug = *readyQueue;
    if(select_sched && GetLotteryCount(readyQueue) > 0) {
        ticks -=1;
        scheduler_lottery(0);
    }
    else {
        ticks -=1;
        scheduler_rr(0);
    }

}



void setTicket(Thread_ptr thread, int tickets){
    thread->tickets = tickets;
}



