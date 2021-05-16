#ifndef SCHED_MANAGER_H
#define SCHED_MANAGER_H

#include "DataStructures.h"
#include "roundRobin_sched.h"
#include "lottery_sched.h"
#include <stdbool.h>

void threadCompletedNotifier();
ucontext_t setupCompleteContext();
void manage(int sigNum);
void setTicket(Thread_ptr thread, int tickets);
void disableSpecialThreads();
int getRR();
void manageSpecialThread();
int getSpecialThread();


#endif //SCHED_MANAGER_H
