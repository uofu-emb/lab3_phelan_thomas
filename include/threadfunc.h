#pragma once
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

int updateCounter(const char thread_val, int *counter, SemaphoreHandle_t semaphore);