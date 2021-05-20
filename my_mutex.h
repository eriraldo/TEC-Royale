//
// Created by ericksalazargranera on 5/19/21.
//

#ifndef TECROYALE_MY_MUTEX_H
#define TECROYALE_MY_MUTEX_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_thread.h"

void my_mutex_init(int *lock);

void my_mutex_destroy(int *lock);

void my_mutex_lock(int *lock);

void my_mutex_unlock(int *lock);

bool my_mutex_trylock(int *lock);


#endif //TECROYALE_MY_MUTEX_H
