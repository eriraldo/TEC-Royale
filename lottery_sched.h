//
// Created by ericksalazargranera on 5/19/21.
//

#ifndef TECROYALE_LOTTERY_SCHED_H
#define TECROYALE_LOTTERY_SCHED_H

#include "thread_structure.h"
#include "realtime_sched.h"

void scheduler_lottery(int  sigNum);
Thread_ptr getWinnerThread(Thread_ptr current);
int getTickets();


#endif //TECROYALE_LOTTERY_SCHED_H
