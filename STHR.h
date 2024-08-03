#pragma once

/**
 * STHR is for Simple Threading Utility
 */
 
#include <stdlib.h>

// types
typedef enum
{
    STHR_OK = 0,
    STHR_ERR_GENERIC = -1
} STHR_result_t;

typedef void(STHR_function_t)(void);

#ifdef _WIN32

#include <windows.h>
typedef HANDLE STHR_thread_t;
typedef HANDLE STHR_mutex_t;

#else

#include <pthread.h>
typedef pthread_t STHR_thread_t;
typedef pthread_mutex_t STHR_mutex_t;

#endif

// threads
STHR_result_t STHR_thread_create(STHR_thread_t *thread, STHR_function_t *function);
STHR_result_t STHR_thread_destroy(STHR_thread_t *thread);
STHR_result_t STHR_thread_wait_end(STHR_thread_t *thread);

// mutexes
STHR_result_t STHR_mutex_create(STHR_mutex_t *mutex);
STHR_result_t STHR_mutex_destroy(STHR_mutex_t *mutex);
STHR_result_t STHR_mutex_lock(STHR_mutex_t *mutex);
STHR_result_t STHR_mutex_unlock(STHR_mutex_t *mutex);
