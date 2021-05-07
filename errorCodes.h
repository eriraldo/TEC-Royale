
#ifndef ERRORCODES_H
#define	ERRORCODES_H

//These are the possible error codes returned
//from the functions returning integer
enum MFError
{
    //this will be returned if the requested function is sucessfull
    SUCCESS =0,
    //this will be returned if any my_thread functions will be called
    //without calling my_thread_init
    READY_QUEUE_NOT_INITIALIZED = -2,
    //this will be returned if there is no enough memeory available
    //to perform the requested function
    NOT_ENOUGH_MEMORY = -3,
    //this will be returned if the requested operation
    //cannot be performed with the required parameters
    INVALID_OPERATION = -1,
    //this will be returned if the provided
    //my_thread_t is invalid or not initialized
    INVALID_my_thread_T = -4
};




#endif	/* ERRORCODES_H */

