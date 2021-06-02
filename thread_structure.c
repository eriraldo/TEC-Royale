#include "thread_structure.h"

//Add thread node to completed queue
int PushToCompleted(Completed_Queue queue,CompletedThread_ptr node)
{
    int failure = -1;
    if(queue != null && node != null)
    {
        node->nextThread=queue->node;
        queue->node=node;
        failure=0;
    }
    return failure;
}
//This will take the current warrior of a warrior list
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
        failure = -1;
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
            free(head_node);
            queue->count--;
        }
        else
        {
            //Queue Is Empty
            failure = -1;
        }
    }
    return failure;
}

//Moves head of queue
int moveHead(Thread_Queue queue)
{
    int failure = -1;
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

//Get thread given id
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

//Set each thread that belongs to the same sched as not recently used
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

//copies each attribute in a new thread
Thread_ptr cloneThread(Thread_ptr oldThread){
    Thread_ptr newthread = (Thread_ptr)malloc(sizeof(Thread));
    newthread->idThread = oldThread->idThread;
    newthread->quantum = oldThread->quantum;
    newthread->context = oldThread->context;
    newthread->isCompleted = oldThread->isCompleted;
    newthread->scheduler = oldThread->scheduler;
    newthread->tickets = oldThread->tickets;
    newthread->special = 0;
    return newthread;

}

//prints id of each thread
void printThreads(Thread_Queue queue){
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;
    printf("\n PRINTING \n ");
    while((nextNode!=null))
    {
        printf("%ld: ", nextNode->idThread);
        nextNode = nextNode->next;
        if (head == nextNode)
            break;
    }
    printf("\n DONE PRINTING");
}

int GetThreadCount(Thread_Queue queue)
{
    if(queue == null)
    {
        return 0;
    }
    return queue->count;
}

//Gets the amount of lottery sched used
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
    return -1;

}

//Increment id each time a new thread is created
int GetNextThreadId()
{
    static long currentThreadID=0;
    return ++currentThreadID;
}

//Add thread node to queue
int Push_Queue(Thread_Queue queue,Thread_ptr node)
{
    int failure = 0;
    if(queue ==null || node== null)
    {
        failure = -1;
    }
    else
    {
        if(queue->head == null){
            node->next = node;
            queue->head_parent = node;
            queue->head = node;
        }
        else{
            //Add the node before head so that it will be at the end of the queue
            node->next= queue->head;
            queue->head_parent->next = node;
            queue->head_parent = node;
        }
        queue->count++;
    }

    return failure;
}

//Given a node, moves queue head until the node is found, then pop
void PopNode_Queue(Thread_Queue queue, Thread_ptr node)
{
    Thread_ptr nextNode = GetCurrentThread(queue);
    Thread_ptr head = nextNode;
    while((nextNode!=null))
    {
        if(nextNode->idThread == node->idThread)
            break;
        else{
            moveHead(queue);
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

//Initialize a new thread pointer
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
    newThread->idThread = GetNextThreadId();
    newThread->isCompleted = 0;
    newThread->recently_used = 0;
    newThread->scheduler = 0;
    newThread->tickets = 0;
    return newThread;
}

//Initialize new thread queue
Thread_Queue GetThreadQueue()
{
    Thread_Queue newQueue = (Thread_Queue)malloc(sizeof(struct Thread_Queue));
    if(newQueue == null)
    {
        return null;
    }
    newQueue->count = 0;
    newQueue->head = newQueue->head_parent = null;
    return newQueue;
}
//Initialize new completed thread node
CompletedThread_ptr GetCompletedNode()
{
    CompletedThread_ptr newNode = (CompletedThread_ptr)malloc(sizeof(struct completedNode));
    if(newNode == null)
    {
        return null;
    }
    newNode->returnStatus = (void**)malloc(sizeof(void*));
    if(newNode->returnStatus == null)
    {
        free(newNode);
        return null;
    }
    newNode->idThread=-1;
    *(newNode->returnStatus) = null;
    newNode->nextThread = null;
    return newNode;
}

//Initialize new completed thread queue
Completed_Queue GetCompletedQueue()
{
    Completed_Queue newQueue = (Completed_Queue)malloc(sizeof(struct Completed_Thread_Queue));
    if(newQueue == null)
    {
        return null;
    }
    newQueue->count = 0;
    newQueue->node = null;
    return newQueue;
}
