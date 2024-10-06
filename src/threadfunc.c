#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include <threadfunc.h>


int updateCounter(const char thread_val, int *counter, SemaphoreHandle_t semaphore)
{
    if (xSemaphoreTake(semaphore, 250) == pdFALSE) 
        {
        return pdFALSE;
        }

    (*counter)++;

    printf("hello world from %s! Count %d\n", thread_val, *counter);

    xSemaphoreGive(semaphore);
    return pdTRUE;
}