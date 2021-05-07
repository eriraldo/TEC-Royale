#include "DataStructures.h"
#include "my_thread.h"

Thread_Queue readyQueue = null;
Completed_Queue deadQueue = null;
struct sigaction schedulerHandle;
extern ucontext_t notifierContext;
struct itimerval timeQuantum;
long timeIntervalInMSec;
sigset_t sigProcMask;
void *wrapperFunction(void *(*start_routine)(void *), void *arg);
Thread_ptr GetCurrentThread_ptr();
extern int ignoreSignal;

void my_thread_init(long period) {
    //Initialize Ready Queue
    if (readyQueue == null && deadQueue == null) {
        sigemptyset(&sigProcMask);
        sigaddset(&sigProcMask, SIGPROF);
        ignoreSignal = 0;
        deadQueue = GetCompletedQueue();
        if (deadQueue == null) {
            return;
        }
        readyQueue = GetThreadQueue();
        if (readyQueue != null) {
            //Get Thread for main
            timeIntervalInMSec = period;

            Thread_ptr ThreadOfMain = NewThread();
            getcontext(&(ThreadOfMain->context));
            setupCompleteContext();
            ThreadOfMain->context.uc_link = &notifierContext;

            //Put the Thread in ready Queue
            Push_Queue(readyQueue, ThreadOfMain);

            //Set Signal Handler to Call Scheduler
            memset(&schedulerHandle, 0, sizeof (schedulerHandle));
            schedulerHandle.sa_handler = &scheduler;
            sigaction(SIGPROF, &schedulerHandle, NULL);

            printf("\nmy_thread library initialized\n");
            timeQuantum.it_value.tv_sec = 0;
            timeQuantum.it_value.tv_usec = timeIntervalInMSec;
            timeQuantum.it_interval.tv_sec = 0;
            timeQuantum.it_interval.tv_usec = timeIntervalInMSec;
            setitimer(ITIMER_PROF, &timeQuantum, NULL);
        }
    }
}

int my_thread_create(my_thread_t *thread, void *(*start_routine)(void *), void *arg) {

    if (readyQueue != null) {
        sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
        //Create a New Thread with stack
        Thread_ptr newThread = NewThread();
        getcontext(&(newThread->context));

        newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);
        if (newThread->context.uc_stack.ss_sp == null) {
            FreeNode(newThread);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return NOT_ENOUGH_MEMORY;
        }
        newThread->context.uc_stack.ss_size = STACKSIZE;
        newThread->context.uc_stack.ss_flags = 0;
        newThread->hasNoStackSpaceAllocated = 0;

        //Set the ulink to CompleteHandlerContext
        newThread->context.uc_link = &notifierContext;

        //Make Context by calling the wrapper function
        //called with the given function pointer and arguments
        makecontext(&(newThread->context), wrapperFunction, 2, start_routine, arg);



        //Copy the value of threadid to *thread
        *thread = newThread->idThread;

        //DEBUGGING
        printf("Created Thread:%ld\n", *thread);
        //ENDDEBUGGING

        //Enqueue the item to the ready queue
        Push_Queue(readyQueue, newThread);
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        return 0;
    }

    return READY_QUEUE_NOT_INITIALIZED;
}

void *wrapperFunction(void *(*start_routine)(void *), void *arg) {
    void *returnValue;
    Thread_ptr currentThread = GetCurrentThread_ptr();
    returnValue = (*start_routine)(arg);
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    CompletedThread_ptr completedNode = GetCompletedNode();
    if (completedNode != null) {
        *(completedNode->returnStatus) = returnValue;
        completedNode->idThread = currentThread->idThread;
        PushToCompleted(deadQueue, completedNode);
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return returnValue;
}

Thread_ptr GetCurrentThread_ptr() {
    Thread_ptr currentThread = null;
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    currentThread = GetCurrentThread(readyQueue);
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return currentThread;
}

