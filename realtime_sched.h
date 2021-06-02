//
// Created by ericksalazargranera on 5/19/21.
//

#ifndef TECROYALE_REALTIME_SCHED_H
#define TECROYALE_REALTIME_SCHED_H

#include "thread_structure.h"
#include "roundRobin_sched.h"
#include "lottery_sched.h"
#include <stdbool.h>

void notifyThreadCompleted();
ucontext_t setupCompleteContext();
void manage(int sigNum);
void setTicket(Thread_ptr thread, int tickets);
void disableSpecialThreads();
int getRR();
void manageSpecialThread();
int getSpecialThread();
void cleanQueue(int sched, int origSched);



#endif //TECROYALE_REALTIME_SCHED_H
