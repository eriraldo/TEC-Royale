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

int GetThreadCount(Thread_Queue queue)
{
    if(queue == null)
    {
        return 0;
    }
    return queue->count;
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
    newNode->idThread=INVALID_my_thread_T;
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
