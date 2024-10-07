#pragma once
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

int updateCounter(const char *thread_val, int *counter, SemaphoreHandle_t semaphore);

struct DeadlockArgs
{
    SemaphoreHandle_t a, b;
    int counter;
    char id;
};

void deadlock(void *);
int orphaned_lock(SemaphoreHandle_t semaphore, int *counter);