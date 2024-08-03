#include "STHR.h"

/**
 * THREADS
 */

#ifdef _WIN32

STHR_result_t STHR_thread_create(STHR_thread_t *thread, STHR_function_t *function)
{
    *thread = CreateThread(
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)function,
        NULL,
        0,
        NULL);

    if (*thread == NULL)
        return STHR_ERR_GENERIC;
    return STHR_OK;
}

STHR_result_t STHR_thread_destroy(STHR_thread_t *thread)
{
    if (*thread == NULL)
        return STHR_ERR_GENERIC;

    CloseHandle(*thread);

    return STHR_OK;
}

STHR_result_t STHR_thread_wait_end(STHR_thread_t *thread)
{
    if (*thread == NULL)
        return STHR_ERR_GENERIC;

    WaitForSingleObject(*thread, INFINITE);
    CloseHandle(*thread);

    return STHR_OK;
}

#else

STHR_result_t STHR_thread_create(STHR_thread_t *thread, STHR_function_t *function)
{
    if (pthread_create(thread, NULL, (void *(*)(void *))function, NULL) != 0)
        return STHR_ERR_GENERIC;
    return STHR_OK;
}

STHR_result_t STHR_thread_destroy(STHR_thread_t *thread)
{
    if (pthread_cancel(*thread) != 0)
        return STHR_ERR_GENERIC;
    return STHR_OK;
}

STHR_result_t STHR_thread_wait_end(STHR_thread_t *thread)
{
    if (pthread_join(*thread, NULL) != 0)
        return STHR_ERR_GENERIC;
    return STHR_OK;
}

#endif

/**
 * MUTEXES
 */

#ifdef _WIN32

STHR_result_t STHR_mutex_create(STHR_mutex_t *mutex)
{
    *mutex = CreateMutex(NULL, FALSE, NULL);
    if (*mutex == NULL)
        return STHR_ERR_GENERIC;
    return STHR_OK;
}

STHR_result_t STHR_mutex_destroy(STHR_mutex_t *mutex)
{
    if (CloseHandle(*mutex) != 0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

STHR_result_t STHR_mutex_lock(STHR_mutex_t *mutex)
{
    DWORD result = WaitForSingleObject(*mutex, INFINITE);
    if (result == WAIT_OBJECT_0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

STHR_result_t STHR_mutex_unlock(STHR_mutex_t *mutex)
{
    if (ReleaseMutex(*mutex) != 0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

#else

STHR_result_t STHR_mutex_create(STHR_mutex_t *mutex)
{
    if (pthread_mutex_init(mutex, NULL) == 0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

STHR_result_t STHR_mutex_destroy(STHR_mutex_t *mutex)
{
    if (pthread_mutex_destroy(mutex) == 0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

STHR_result_t STHR_mutex_lock(STHR_mutex_t *mutex)
{
    if (pthread_mutex_lock(mutex) == 0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

STHR_result_t STHR_mutex_unlock(STHR_mutex_t *mutex)
{
    if (pthread_mutex_unlock(mutex) == 0)
        return STHR_OK;
    return STHR_ERR_GENERIC;
}

#endif
