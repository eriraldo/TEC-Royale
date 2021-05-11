#ifndef SCHED_MANAGER_H
#define SCHED_MANAGER_H

#include "DataStructures.h"
#include "roundRobin_sched.h"
#include "lottery_sched.h"
#include <stdbool.h>

void threadCompletedNotifier();
ucontext_t setupCompleteContext();
void manage(int sigNum);
void swapSched(Thread_ptr thread, int sched);
void setTicket(Thread_ptr thread, int tickets);
Thread_ptr cloneThread(Thread_ptr oldThread);

#endif //SCHED_MANAGER_H
