#include "DataStructures.h"
#include "errorCodes.h"


//This will add teh given Thread to the last of the queue


int PushToCompleted(Completed_Queue queue,CompletedThread_ptr node)
{
    int failure = INVALID_OPERATION;
    if(queue != null && node != null)
    {
        node->nextThread=queue->node;
        queue->node=node;
        failure=0;
    }
    return failure;
}

Thread_ptr GetCurrentThread(Thread_Queue queue)
{
    if(queue == null)
    {
        return null;
    }
    return queue->head;
}

//This will remove the Thread which is pointed by head of the given queue
int Pop_Queue(Thread_Queue queue)
{
    int failure = 0;
    Thread_ptr next_node,prev_node,head_node;
    if(queue == null)
    {
        failure = INVALID_OPERATION;
    }
    else
    {
        head_node = queue->head;
        prev_node = queue->head_parent;
        if(head_node != null)
        {

            next_node = head_node->next;
            if(queue->count == 1)
            {
                //There is only one node in the queue
                //clear the pointers
                queue->head = queue->head_parent = null;
            }
            else
            {
                //More Than one node in queue
                //Make the parent node point to next_node
                queue->head = next_node;
                prev_node->next = queue->head;
            }
            FreeNode(head_node);
            queue->count--;
        }
        else
        {
            //Queue Is Empty
            failure = INVALID_OPERATION;
        }
    }
    return failure;
}


int MoveForward(Thread_Queue queue)
{
    int failure = INVALID_OPERATION;
    if(queue != null)
    {
        Thread_ptr current_head = queue->head;
        if(current_head != null)
        {
            queue->head_parent = current_head;
            queue->head = current_head->next;
            failure = 0;
        }
    }
    return failure;
}

Thread_ptr GetThread(Thread_Queue queue, long idThread){
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;

    while((nextNode!=null))
    {
        if(nextNode->idThread == idThread)
            break;
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    return nextNode;
}

void setNotUsed(Thread_Queue queue, int sched){
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;

    while((nextNode!=null))
    {
        if(nextNode->scheduler == sched)
            nextNode->recently_used = 0;
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
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
    newthread->scheduler = oldThread->scheduler;
    newthread->tickets = oldThread->tickets;
    newthread->special = 0;
    return newthread;

}

void printThreads(Thread_Queue queue){
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;
    printf("\n PRINTING \n ");
    while((nextNode!=null))
    {
        printf("%d: ", nextNode->idThread);
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    printf("\n DONE PRINTING");
}

Thread_Queue getNextThread(Thread_Queue queue, int sched){
    Thread_Queue target_sched = GetThreadQueue();
    Thread_Queue leftovers = GetThreadQueue();
    Thread_Queue debug = queue;
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;

    while((nextNode!=null))
    {
        Thread_ptr nodeClone = cloneThread(nextNode);
        if(nextNode->scheduler == sched)
            Push_Queue(target_sched, nodeClone);
        else
            Push_Queue(leftovers, nodeClone);
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    MoveForward(target_sched);

    nextNode = GetCurrentThread(leftovers);
    head = nextNode;

    while((nextNode!=null))
    {
        Thread_ptr nodeClone = cloneThread(nextNode);
        Push_Queue(target_sched, nodeClone);
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    return target_sched;
}


int GetThreadCount(Thread_Queue queue)
{
    if(queue == null)
    {
        return 0;
    }
    return queue->count;
}

int GetLotteryCount(Thread_Queue queue){
    int count = 0;
    if(queue!= null){
        Thread_ptr nextNode = GetCurrentThread(queue);
        Thread_ptr head = nextNode;

        while((nextNode!=null))
        {
            if (nextNode->scheduler == 1)
                count+=1 ;
            nextNode = nextNode->next;
            if (head == nextNode)
                break;
        }
        return count;
    }
    printf("ERROR COLA\n");
    return -1;

}

//HELPER FUNCTIONS

int GetNextThreadId()
{
    static long currentThreadID=0;
    return ++currentThreadID;
}

void FreeNode(Thread_ptr node)
{
    //if we had alloceted stack space to this node
    //first free it before freeing the node
    if(node != null)
    {
        WaitThreadList blockedThreadNode = node->waitingThreads;
        if(!(node->hasNoStackSpaceAllocated))
        {
            free(node->context.uc_stack.ss_sp);
        }
        if(blockedThreadNode != null)
        {
            WaitThreadList nextNode = blockedThreadNode->next;
            free(blockedThreadNode);
            blockedThreadNode = nextNode;
        }
        free(node);
    }
}

int Push_Queue(Thread_Queue queue,Thread_ptr node)
{
    int failure = 0;
    if(queue ==null || node== null)
    {
        failure = INVALID_OPERATION;
    }
    else
    {
        if(queue->head == null)
        {
            //first node
            //make next of it point to itself as we are implementing circular list
            node->next = node;
            queue->head_parent = node;
            queue->head = node;
        }
        else
        {
            //Add the node before head so that it will be at the end of the queue
            node->next= queue->head;
            queue->head_parent->next = node;
            queue->head_parent = node;
        }
        queue->count++;
    }

    return failure;
}

void PopNode_Queue(Thread_Queue queue, Thread_ptr node)
{
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->idThread == node->idThread)
            break;
        else{
            MoveForward(queue);
        }
        nextNode = GetCurrentThread(queue);
        if (nextNode == head){
            nextNode = null;
            break;
        }
    }
    if (nextNode!=null){
        Pop_Queue(queue);
    }

}

Thread_ptr NewThread()
{
    Thread_ptr newThread = (Thread_ptr)malloc(sizeof(Thread));
    if(newThread == null)
    {
        return null;
    }
    newThread->context.uc_link = 0;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->next = null;
    newThread->waitingThreads = null;
    newThread->isBlocked = 0;
    newThread->idThread = GetNextThreadId();
    newThread->hasNoStackSpaceAllocated = 1;
    newThread->isCompleted = 0;
    newThread->recently_used = 0;
    newThread->scheduler = 0;
    newThread->tickets = 10;
    return newThread;
}

Thread_Queue GetThreadQueue()
{
    Thread_Queue newQueue = (Thread_Queue)malloc(sizeof(struct Thread_Queue));
    if(newQueue == null)
    {
        //Memory Exhaustion
        return null;
    }
    newQueue->count = 0;
    newQueue->head = newQueue->head_parent = null;
    return newQueue;
}

CompletedThread_ptr GetCompletedNode()
{
    CompletedThread_ptr newNode = (CompletedThread_ptr)malloc(sizeof(struct DeadNode));
    if(newNode == null)
    {
        //Memory Exhaustion
        return null;
    }
    newNode->returnStatus = (void**)malloc(sizeof(void*));
    if(newNode->returnStatus == null)
    {
        //Memory Exhaustion
        free(newNode);
        return null;
    }
    newNode->idThread=INVALID_MY_THREAD_ID;
    *(newNode->returnStatus) = null;
    newNode->nextThread = null;
    return newNode;
}

Completed_Queue GetCompletedQueue()
{
    Completed_Queue newQueue = (Completed_Queue)malloc(sizeof(struct Completed_Thread_Queue));
    if(newQueue == null)
    {
        //Memory Exhaustion
        return null;
    }
    newQueue->count = 0;
    newQueue->node = null;
    return newQueue;
}
