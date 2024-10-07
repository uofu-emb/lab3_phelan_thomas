#include <FreeRTOS.h>
#include <semphr.h>
#include <threadfunc.h>


int updateCounter(const char *thread_val, int *counter, SemaphoreHandle_t semaphore)
{
    if (xSemaphoreTake(semaphore, 10) == pdFALSE) 
        {
        return pdFALSE;
        }

    (*counter)++;

    printf("hello world from %s! Count %d\n", thread_val, *counter);

    xSemaphoreGive(semaphore);
    return pdTRUE;
}


void deadlock(void *args)
{
    struct DeadlockArgs *dargs = (struct DeadlockArgs *)args;

    printf("\tBegin deadlock %c\n", dargs->id);
    dargs->counter++;
    xSemaphoreTake(dargs->f, portMAX_DELAY);
    {
        dargs->counter++;
        printf("\tinside first lock %c\n", dargs->id);
        vTaskDelay(100);
        printf("\tpost-delay %c\n", dargs->id);
        xSemaphoreTake(dargs->s, portMAX_DELAY);
        {
            printf("\tinside second lock %c\n", dargs->id);
            dargs->counter++;
        }
        xSemaphoreGive(dargs->s);
    }
    xSemaphoreGive(dargs->f);
    vTaskSuspend(NULL);
}

int orphaned_lock(SemaphoreHandle_t semaphore, int *counter)
{
    if (xSemaphoreTake(semaphore, 500) == pdFALSE) 
    {
        return pdFALSE;
    }

    (*counter)++;

    if ((*counter) % 2) 
    {
        return pdFALSE;
    }
    printf("Count %d\n", counter);
    xSemaphoreGive(semaphore);
    return pdTRUE;  
}       

int unorphaned_lock(SemaphoreHandle_t semaphore, int *counter)
{
    if (xSemaphoreTake(semaphore, 500) == pdFALSE) 
    {
        return pdFALSE;
    }

    (*counter)++;

    if ((*counter) % 2) 
    {
        xSemaphoreGive(semaphore);  //change so it alway releases
        return pdFALSE;
    }
    printf("Count %d\n", counter);
    xSemaphoreGive(semaphore);
    return pdTRUE;  
}       