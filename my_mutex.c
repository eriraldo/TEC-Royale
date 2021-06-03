#include "my_mutex.h"
//Initializes mutex lock
void my_mutex_init(int *lock){
    lock = 0;
}

void my_mutex_lock(int *lock) {//acquires lock
    while (*lock){//If lock can't be acquired, then yield
        my_thread_yield();
    }
    __sync_val_compare_and_swap (lock, 0, 1); //strong atomic compare exchange
}

void my_mutex_unlock(int *lock) { //loses lock
    __sync_val_compare_and_swap (lock, 1, 0);
}

bool my_mutex_trylock(int *lock) { //returns true if lock is acquired, false otherwise.
    if(__sync_val_compare_and_swap (lock, 0, 1) != 0)
        return false;
    return true;
}

void my_mutex_destroy(int *lock) { //frees lock
    free(lock);
}

