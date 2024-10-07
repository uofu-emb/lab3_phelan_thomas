#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "threadfunc.h"

void setUp(void) {}

void tearDown(void) {}

void update_cnt_test_available()
{
   SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
   int count = 0;
    printf("\n The previous count is %d\n",count);
   int result = updateCounter("test",&count,semaphore);

     printf("\n The incremented count is %d\n",count);
   TEST_ASSERT_EQUAL_INT16_MESSAGE(pdTRUE,result, "The updateCounter func did not run");
   TEST_ASSERT_EQUAL_INT16_MESSAGE(1,count,"The counter did not increment");
}

void update_cnt_test_unavailable()
{
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    int count = 0;
    printf("\n The previous count is %d\n",count);
    xSemaphoreTake(semaphore, portMAX_DELAY);
    int result = updateCounter("test",&count,semaphore);

     printf("\n The incremented count is %d\n",count);
     TEST_ASSERT_EQUAL_INT16_MESSAGE(pdFALSE,result, "The updateCounter func did run");
     TEST_ASSERT_EQUAL_INT16_MESSAGE(0,count,"The counter did increment");
}

#define FIRST_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define FIRST_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define SEC_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SEC_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void testLock(void)
{
  printf("intestlock");
  SemaphoreHandle_t first_semaphore = xSemaphoreCreateCounting(1, 1);
  SemaphoreHandle_t sec_semaphore = xSemaphoreCreateCounting(1, 1);
  TaskHandle_t first_handle, sec_handle;
  struct DeadlockArgs first_args= {first_semaphore,sec_semaphore,0,'f'};
  struct DeadlockArgs sec_args= {sec_semaphore,first_semaphore,10,'s'};

  xTaskCreate(deadlock,"f",FIRST_TASK_STACK_SIZE,&first_args,FIRST_TASK_PRIORITY,&first_handle);
  xTaskCreate(deadlock,"s",SEC_TASK_STACK_SIZE,&sec_args,SEC_TASK_PRIORITY,&sec_handle);

  printf("creating delay for deadlock");
  vTaskDelay(500);
  printf("waited for 500ms");

  TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(first_semaphore), 0);
  TEST_ASSERT_EQUAL_INT(uxSemaphoreGetCount(sec_semaphore), 0);
  TEST_ASSERT_EQUAL_INT(2, first_args.counter);
  TEST_ASSERT_EQUAL_INT(12, sec_args.counter);

  vTaskDelete(first_handle);
  vTaskDelete(sec_handle);
  printf("killed both threads");
}

void orphaned_test()
{
  printf("Starting orphaned test\n");
  int counter = 1;
  printf("Value of counter was %d", counter);
  SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1,1);
  int result = orphaned_lock(semaphore, &counter);
  TEST_ASSERT_EQUAL_INT(2,counter);
  printf("New Value of counter is %d\n", counter);

  TEST_ASSERT_EQUAL_INT(pdTRUE,result);
  TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semaphore));

  xSemaphoreGive(semaphore);

  result = orphaned_lock(semaphore,&counter);
  TEST_ASSERT_EQUAL_INT(3, counter);
  printf("New Value of counter is %d\n", counter);
  TEST_ASSERT_EQUAL_INT(pdTRUE, result);
  TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semaphore));

  result = orphaned_lock(semaphore,&counter);
  TEST_ASSERT_EQUAL_INT(3, counter);
  printf("New Value of counter was %d\n", counter);
  TEST_ASSERT_EQUAL_INT(pdTRUE, result);
  TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semaphore));

}

void unorphaned_test()
{
  printf("Starting unorphaned test");
  int counter = 1;
  SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1,1);
  int result = orphaned_lock(semaphore, &counter);
  TEST_ASSERT_EQUAL_INT(2,counter);
  TEST_ASSERT_EQUAL_INT(pdTRUE,result);
  TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(semaphore));

  result = orphaned_lock(semaphore,&counter);
  TEST_ASSERT_EQUAL_INT(3, counter);
  TEST_ASSERT_EQUAL_INT(pdTRUE, result);
  TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(semaphore));

}

int main (void)
{
    stdio_init_all();
    while(true)
    {
        sleep_ms(10000); // Give time for TTY to attach.
        //sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(update_cnt_test_available);
        RUN_TEST(update_cnt_test_unavailable);
        //RUN_TEST(testLock);
        RUN_TEST(orphaned_test);
        RUN_TEST(unorphaned_test);
        //sleep_ms(5000);
        UNITY_END();
    }
    return 0;
}
