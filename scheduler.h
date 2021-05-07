
#ifndef SCHEDULER_H
#define SCHEDULER_H

void threadCompletedNotifier();
ucontext_t setupCompleteContext();
void scheduler(int  sigNum);

#endif //SCHEDULER_H
