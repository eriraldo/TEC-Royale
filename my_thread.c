//
// Created by ericksalazargranera on 5/19/21.
//
#include "thread_structure.h"
#include "my_thread.h"

Thread_Queue readyQueue = null;
Thread_Queue lotteryQueue = null;
Completed_Queue completedQueue = null;
struct sigaction schedulerHandle;
extern ucontext_t notifierContext;
struct itimerval timeQuantum;
long quantum;
sigset_t sigProcMask;
void *wrapperFunction(void *(*start_routine)(void *), void *arg);
Thread_ptr GetCurrentThread_ptr();

//function to change scheduler, it just modifies an attribute and gives tickets if it's lottery sched
void my_thread_chsched(Thread_ptr thread, int sched){
    switch (sched) {
        case 0: //round-robin sched
            thread->tickets = 0;
            thread->scheduler = 0;
            break;
        case 1: //lottery sched
            thread->tickets = 10;
            thread->scheduler = 1;
            break;
        case 2: //priority sched (for bombWarrior)
            thread->special = 1;
            break;
        default: //round-robin sched
            thread->tickets = 0;
            thread->scheduler = 0;
            break;
    }
}
//initializes thread library so we can use all its functions
void my_thread_init(long period) {
    if (readyQueue == null && completedQueue == null) {
        sigemptyset(&sigProcMask); //Initializes the signal set given by set to empty, with all signals excluded from the set.
        sigaddset(&sigProcMask, SIGPROF); //add signal signum from set.
        completedQueue = GetCompletedQueue();
        if (completedQueue == null) {
            return;
        }
        readyQueue = GetThreadQueue();
        lotteryQueue = GetThreadQueue();
        if (readyQueue != null) {
            quantum = period;
            Thread_ptr ThreadOfMain = NewThread();
            getcontext(&(ThreadOfMain->context));//gives context to thread
            setupCompleteContext();
            ThreadOfMain->context.uc_link = &notifierContext; //after finishing its target function, it will call notifyThreadCompleted

            Push_Queue(readyQueue, ThreadOfMain);
            memset(&schedulerHandle, 0, sizeof (schedulerHandle));
            schedulerHandle.sa_handler = &manage; //Function that manages both round-robin and lottery scheduler.
            sigaction(SIGPROF, &schedulerHandle, NULL);// This signal typically indicates expiration of a timer that measures both CPU time used by the current process, and CPU time expended on behalf of the process by the system.
            timeQuantum.it_value.tv_sec = 0;
            timeQuantum.it_value.tv_usec = quantum; // Time until next expiration im microseconds
            timeQuantum.it_interval.tv_sec = 0;
            timeQuantum.it_interval.tv_usec = quantum; // Interval for periodic timer in microseconds
            setitimer(ITIMER_PROF, &timeQuantum, NULL);
        }
    }
}

//thread is willing to relinquish cpu time
void my_thread_yield(){
    raise(SIGPROF);
}

//Given target function, its argument and its scheduler, creates a new thread node
int my_thread_create(my_thread_t *thread, void *(*start_routine)(void *), void *arg, int sched) {

    if (readyQueue != null) {
        sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
        Thread_ptr newThread = NewThread();
        getcontext(&(newThread->context));
        newThread->quantum = quantum; //saves quantum on attribute

        newThread->context.uc_stack.ss_sp = malloc(STACKSIZE); //allocates memory to stack
        if (newThread->context.uc_stack.ss_sp == null) {
            free(newThread);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return -1;
        }
        newThread->context.uc_stack.ss_size = STACKSIZE;
        newThread->context.uc_stack.ss_flags = 0;
        newThread->context.uc_link = &notifierContext;
        makecontext(&(newThread->context), (void (*)(void)) wrapperFunction, 2, start_routine, arg); //modifies context so it'll work on target function
        *thread = newThread->idThread;
        my_thread_chsched(newThread, sched); //changes sched based on sched argument
        Push_Queue(readyQueue, newThread);
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        return 0;
    }

    return -1;
}

//Thread is asleep given certain amount of seconds
void my_thread_sleep(long seconds){
    time_t t1;
    t1 = time(0) + seconds;
    while(  time(0) < t1)
        ;
}

//used to get the return value of a function that is called in the context
void *wrapperFunction(void *(*start_routine)(void *), void *arg) {
    void *returnValue;
    Thread_ptr currentThread = GetCurrentThread_ptr();
    returnValue = (*start_routine)(arg);
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    CompletedThread_ptr completedNode = GetCompletedNode();
    if (completedNode != null) {
        *(completedNode->returnStatus) = returnValue;
        completedNode->idThread = currentThread->idThread;
        PushToCompleted(completedQueue, completedNode);
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return returnValue;
}

//Get head thread node from queue
Thread_ptr GetCurrentThread_ptr() {
    Thread_ptr currentThread = null;
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    currentThread = GetCurrentThread(readyQueue);
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return currentThread;
}

//Ends thread
void my_thread_end() {
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);//blocks signal so it can end properly
    Thread_ptr thread = GetCurrentThread(readyQueue);
    thread->scheduler = -1;
    CompletedThread_ptr completedNode = GetCompletedNode();
    if (completedNode != null && thread!= null) {
        completedNode->idThread = thread->idThread;
        PushToCompleted(completedQueue, completedNode);//add current thread to completed queue
    }
    notifyThreadCompleted();
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    raise(SIGPROF);
}

