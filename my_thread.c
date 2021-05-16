#include "DataStructures.h"
#include "my_thread.h"

Thread_Queue readyQueue = null;
Thread_Queue lotteryQueue = null;
Completed_Queue deadQueue = null;
struct sigaction schedulerHandle;
extern ucontext_t notifierContext;
struct itimerval timeQuantum;
long quantum;
sigset_t sigProcMask;
void *wrapperFunction(void *(*start_routine)(void *), void *arg);
Thread_ptr GetCurrentThread_ptr();
extern int ignoreSignal;


void my_thread_chsched(Thread_ptr thread, int sched){
    switch (sched) {
        case 0:
            thread->tickets = 0;
            thread->scheduler = 0;
            break;
        case 1:
            thread->tickets = 10;
            thread->scheduler = 1;
            break;
        case 2:
            thread->special = 1;
            break;
        default:
            thread->tickets = 0;
            thread->scheduler = 0;
            break;
    }
}

void my_thread_init(long period) {
    if (readyQueue == null && deadQueue == null) {
        sigemptyset(&sigProcMask);
        sigaddset(&sigProcMask, SIGPROF);
        ignoreSignal = 0;
        deadQueue = GetCompletedQueue();
        if (deadQueue == null) {
            return;
        }
        readyQueue = GetThreadQueue();
        lotteryQueue = GetThreadQueue();
        if (readyQueue != null) {
            quantum = period;

            Thread_ptr ThreadOfMain = NewThread();
            getcontext(&(ThreadOfMain->context));
            setupCompleteContext();
            ThreadOfMain->context.uc_link = &notifierContext;

            Push_Queue(readyQueue, ThreadOfMain);
            memset(&schedulerHandle, 0, sizeof (schedulerHandle));
            schedulerHandle.sa_handler = &manage;
            sigaction(SIGPROF, &schedulerHandle, NULL);

            printf("\nmy_thread library initialized\n");
            timeQuantum.it_value.tv_sec = 0;
            timeQuantum.it_value.tv_usec = quantum;
            timeQuantum.it_interval.tv_sec = 0;
            timeQuantum.it_interval.tv_usec = quantum;
            setitimer(ITIMER_PROF, &timeQuantum, NULL);
        }
    }
}

void asd(){
    Thread_ptr main = GetThread(readyQueue, 1);
    notifierContext.uc_link = &main->context;
}

int my_thread_create(my_thread_t *thread, void *(*start_routine)(void *), void *arg, int sched) {

    if (readyQueue != null) {
        sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
        Thread_ptr newThread = NewThread();
        getcontext(&(newThread->context));
        newThread->quantum = quantum;

        newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);
        if (newThread->context.uc_stack.ss_sp == null) {
            FreeNode(newThread);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return NOT_ENOUGH_MEMORY;
        }
        newThread->context.uc_stack.ss_size = STACKSIZE;
        newThread->context.uc_stack.ss_flags = 0;
        newThread->hasNoStackSpaceAllocated = 0;

        newThread->context.uc_link = &notifierContext;
        makecontext(&(newThread->context), wrapperFunction, 2, start_routine, arg);
        *thread = newThread->idThread;
        my_thread_chsched(newThread, sched);

        printf("Created Thread:%ld\n", *thread);
        Push_Queue(readyQueue, newThread);
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        return 0;
    }

    return READY_QUEUE_NOT_INITIALIZED;
}

void my_thread_sleep(long seconds){
    time_t t1;
    t1 = time(0) + seconds;
    while(  time(0) < t1)
        ;
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

