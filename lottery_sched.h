

#ifndef LOTTERY_SCHED_H
#define LOTTERY_SCHED_H

#include "DataStructures.h"

void scheduler_lottery(int  sigNum);
Thread_ptr getWinnerThread();
void getTickets();

#endif //LOTTERY_SCHED_H
