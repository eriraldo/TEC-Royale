#include "lottery_sched.h"
extern Thread_Queue lotteryQueue;
extern Thread_Queue readyQueue;
extern struct itimerval timeQuantum;
extern long quantum;
Thread_ptr GetCurrentThread_ptr();
extern sigset_t sigProcMask;
int totalTickets = 0;

//function that get a random thread node based on its ticket count
//most of it is just validations though
void scheduler_lottery(int  sigNum)
{
    Thread_Queue debug = readyQueue;
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(GetThreadCount(readyQueue) == 1){ //means that only the main thread is running
        Thread_ptr currentNode = GetCurrentThread(readyQueue);
        if(currentNode->isCompleted){
            Pop_Queue(readyQueue);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            exit(0);
        }
    }
    else if(GetThreadCount(readyQueue) > 0){
        Thread_ptr currentNode= GetCurrentThread(readyQueue);
        int currentNodeCompleted = 0;
        if(currentNode != null){
            Thread_ptr nextNode;
            if(currentNode->isCompleted){//validations
                currentNodeCompleted = 1;
                Pop_Queue(readyQueue);
            }
            else{
                moveHead(readyQueue);
            }
            cleanQueue(0, 1);
            nextNode = GetCurrentThread(readyQueue);
            if(nextNode == null){//means that there are no threads on queue
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                exit(0);
            }
            nextNode = getWinnerThread(currentNode);//randomly select thread node
            if(nextNode != currentNode){
                fflush(stdout);
                timeQuantum.it_value.tv_usec = quantum; //assign its quantum
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                if(currentNodeCompleted){ //if completed, it's not necessary to do a swap context, since we don't want to save its context
                    nextNode->recently_used = 1;
                    setcontext(&(nextNode->context));
                }
                else{
                    totalTickets = 0; //reset amount of tickets to 0 once a thread node is found
                    nextNode->recently_used = 1;
                    swapcontext(&(currentNode->context),&(nextNode->context));//saves currentNode context and executes nextNode context
                }
            }
        }
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
}


int getTickets(){//get total tickets from each thread that is using lottery scheduler
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
    if(tickets ==0)//avoids division with zero
        tickets=1;
    return tickets;
}

//Its job is to get a random thread node with lottery scheduler
Thread_ptr getWinnerThread(Thread_ptr current){
    totalTickets = getTickets();
    Thread_ptr nextNode = GetCurrentThread(readyQueue);
    srand(time(NULL));
    int test = totalTickets;
    int win_ticket = rand() % totalTickets; //randomly selects winner ticket
    int sumTicket = 0;


    Thread_ptr head = nextNode;
    while((nextNode!=null)){
        if (nextNode->scheduler == 1 && nextNode->recently_used !=1){//only threads that is using lottery sched and is not recently used are eligible to win
            if(win_ticket >= sumTicket && win_ticket < sumTicket+nextNode->tickets)//breaks if winner ticket is in range of node's tickets
                break;
            sumTicket += nextNode->tickets;
        }
        moveHead(readyQueue);
        nextNode = GetCurrentThread(readyQueue);
        if (head == nextNode)
            break;
    }
    return nextNode;

}

