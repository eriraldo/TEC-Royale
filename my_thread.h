
#ifndef MYTHREAD_H
#define	MYTHREAD_H
#include "errorCodes.h"
#include <stdio.h>
#include <ucontext.h>
#include "DataStructures.h"
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"

typedef long my_thread_t;
void my_thread_init(long period);
int my_thread_create(my_thread_t *thread,void *(*start_routine)(void *),void *arg);



#endif	/* MYTHREAD_H */

