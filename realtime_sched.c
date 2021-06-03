#include "realtime_sched.h"
#include "my_thread.h"

ucontext_t notifierContext;
Thread_ptr GetCurrentThread_ptr();
bool select_sched = false;
extern Thread_Queue readyQueue;
extern Thread_Queue lotteryQueue;
int ticks = 0;

//notify that thread is completed
void notifyThreadCompleted()
{
    Thread_ptr currentNode = GetCurrentThread_ptr();
    currentNode->isCompleted = 1;
    raise(SIGPROF);
}

//Initializes context so it will call notifyThreadCompleted once thread is finished
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
        makecontext( &notifierContext, (void (*) (void))&notifyThreadCompleted, 0);
        hasContextCreated = 1;
    }
    return notifierContext;
}

//Temporarily disables scheduler on 'special' thread so it doesn't mess with count function on each scheduler
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

int getRR(){ //get the amount of threads running on round-robin scheduler
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

//For each 'special' thread in queue, find the best sched to place it in
void manageSpecialThread(){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->special == 1) {
            double rrCount = getRR() + 1;
            double tickets = getTickets();
            if (tickets == 1)
                tickets = 50;
            else
                tickets+=50;
            if (1/rrCount > 50/tickets){
                my_thread_chsched(nextNode, 0); //if the amount of round-robin threads is fewer than total tickets, then change sched to rr
                nextNode->tickets = 0;
            }else{
                my_thread_chsched(nextNode, 1); //else, lottery sched
                nextNode->tickets = 50;
            }
        }
        nextNode = nextNode->next;
        if (nextNode == head)
            break;
    }
}

int getSpecialThread(){//get count of 'special' thread
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

//clean queue from completed nodes, finishes once found node is not completed, it's from the same scheduler and not recently used
void cleanQueue(int sched, int origSched){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;

    while((nextNode!=null) && (nextNode->isCompleted || nextNode->scheduler == sched || nextNode->recently_used == 1))
    {
        if(nextNode->isCompleted)
        {
            Pop_Queue(readyQueue);
        }
        else
        {
            moveHead(readyQueue);
        }
        nextNode = GetCurrentThread(readyQueue);
        if (nextNode == head){//if node came back to head, it means all nodes were recently used
            setNotUsed(readyQueue, origSched);
            break;
        }
    }
}

//Function that swap schedulers each time a quantum is used
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

//add tickets to given thread
void setTicket(Thread_ptr thread, int tickets){
    thread->tickets = tickets;
}



