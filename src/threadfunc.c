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

    printf("\tinside deadlock %c\n", dargs->id);
    dargs->counter++;
    xSemaphoreTake(dargs->a, portMAX_DELAY);
    {
        dargs->counter++;
        printf("\tinside first lock %c\n", dargs->id);
        vTaskDelay(100);
        printf("\tpost-delay %c\n", dargs->id);
        xSemaphoreTake(dargs->b, portMAX_DELAY);
        {
            printf("\tinside second lock %c\n", dargs->id);
            dargs->counter++;
        }
        xSemaphoreGive(dargs->b);
    }
    xSemaphoreGive(dargs->a);
    vTaskSuspend(NULL);
}