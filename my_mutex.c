//
// Created by ericksalazargranera on 5/19/21.
//

#include "my_mutex.h"

void my_mutex_init(int *lock){
    lock = 0;
}

void my_mutex_lock(int *lock) {
    while (*lock){
        my_thread_yield();
    }
    __sync_val_compare_and_swap (lock, 0, 1);
}

void my_mutex_unlock(int *lock) {
    __sync_val_compare_and_swap (lock, 1, 0);
}

bool my_mutex_trylock(int *lock) {
    if(__sync_val_compare_and_swap (lock, 0, 1) != 0)
        return false;
    return true;
}

void my_mutex_destroy(int *lock) {
    free(lock);
}

