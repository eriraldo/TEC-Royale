#include "lottery_sched.h"
extern Thread_Queue lotteryQueue;
extern Thread_Queue readyQueue;
extern struct itimerval timeQuantum;
extern long quantum;
Thread_ptr GetCurrentThread_ptr();
extern sigset_t sigProcMask;
int ignoreSignal = 0;
int totalTickets = 0;

void scheduler_lottery(int  sigNum)
{
    Thread_Queue debug = readyQueue;
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
                    //readyQueue = getNextThread(readyQueue, 1);
                    MoveForward(readyQueue);
                }
                Thread_Queue debug2 =readyQueue;
                //currentNode = GetThread(readyQueue, currentNode->idThread);
                nextNode = GetCurrentThread(readyQueue);
                Thread_ptr head = nextNode;

                while((nextNode!=null) && (nextNode->isBlocked || nextNode->isCompleted || nextNode->scheduler == 0 || nextNode->recently_used == 1))
                {
                    if(nextNode->isCompleted)
                    {
                        Pop_Queue(readyQueue);
                    }
                    else{
                        MoveForward(readyQueue);
                    }

                    nextNode = GetCurrentThread(readyQueue);
                    if (nextNode == head){
                        setNotUsed(readyQueue, 1);
                        break;
                    }
                }
                if(nextNode == null)
                {
                    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                    exit(0);
                }
                nextNode = getWinnerThread(currentNode);

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
                        totalTickets = 0;
                        swapcontext(&(currentNode->context),&(nextNode->context));
                    }
                }
            }
        }
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    }
}


int getTickets(){
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    Thread_ptr head = nextNode;
    int tickets = 0;
    while((nextNode!=null))
    {
        if(nextNode->scheduler == 1 && nextNode->recently_used != 1)
            tickets+= nextNode->tickets;
        nextNode = nextNode->next;
        if (nextNode == head)
            break;
    }
    if(tickets ==0)
        tickets=1;
    return tickets;
}

Thread_ptr getWinnerThread(Thread_ptr current){
    totalTickets = getTickets();
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    srand(time(NULL));
    int test = totalTickets;
    int win_ticket = rand() % totalTickets;
    int sumTicket = 0;


    Thread_ptr head = nextNode;
    while((nextNode!=null))
    {
        if (nextNode->scheduler == 1 && nextNode != current){
            if(win_ticket >= sumTicket && win_ticket < sumTicket+nextNode->tickets)
                break;
            sumTicket += nextNode->tickets;
        }
        MoveForward(readyQueue);
        nextNode = GetCurrentThread(readyQueue);
        if (head == nextNode)
            break;
    }
    return nextNode;

}
