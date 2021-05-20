
#ifndef ERRORCODES_H
#define	ERRORCODES_H

//These are the possible error codes returned
//from the functions returning integer
enum MFError
{
    SUCCESS =0,
    READY_QUEUE_NOT_INITIALIZED = -2,
    NOT_ENOUGH_MEMORY = -3,
    INVALID_OPERATION = -1,
    INVALID_MY_THREAD_ID = -4
};




#endif	/* ERRORCODES_H */

