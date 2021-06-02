#ifndef TECROYALE_MY_THREAD_H
#define TECROYALE_MY_THREAD_H

#include <stdio.h>
#include <ucontext.h>
#include "thread_structure.h"
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "roundRobin_sched.h"
#include <time.h>
#include "realtime_sched.h"

typedef long my_thread_t;
void my_thread_init(long period);
int my_thread_create(my_thread_t *thread,void *(*start_routine)(void *),void *arg, int sched);
void my_thread_sleep(long seconds);
void my_thread_chsched(Thread_ptr thread, int sched);
void my_thread_yield();
void my_thread_end();

#endif //TECROYALE_MY_THREAD_H
