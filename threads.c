#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define SIDE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

SemaphoreHandle_t semaphore;

int counter;
int on;

void side_thread(void *params)
{
	while (1) {
        vTaskDelay(100);
        //count is incremented by side thread
        {
            counter += counter + 1;
        }
        //print statements are not thread safe
		printf("hello world from %s! Count %d\n", "thread", counter);
	}
}

void main_thread(void *params)
{
	while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        vTaskDelay(100);
        //print statements are not thread safe
		printf("hello world from %s! Count %d\n", "main", counter++);
        on = !on;
	}
}

int main(void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    on = false;
    //count is not thread safe
    counter = 0;
    TaskHandle_t main, side;
    //lock is here
    semaphore = xSemaphoreCreateCounting(1, 1);
    //creates a lock to run main thread without interrupts
    xTaskCreate(main_thread, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &main);
    //same, but with side thread so it doesnt share resources
    xTaskCreate(side_thread, "SideThread",
                SIDE_TASK_STACK_SIZE, NULL, SIDE_TASK_PRIORITY, &side);
    vTaskStartScheduler();
	return 0;
}