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
   int result = updateCounter(1,&count,semaphore);

     printf("\n The incremented count is %d\n",count);
   TEST_ASSERT_EQUAL_INT16_MESSAGE(pdTRUE,result, "The updateCounter func did not run");
   TEST_ASSERT_EQUAL_INT16_MESSAGE(1,count,"The counter did not increment");
}

void update_cnt_test_unavailable()
{
   SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
   int count = 0;
    printf("\n The previous count is %d\n",count);
   int result = updateCounter(1,&count,semaphore);

     printf("\n The incremented count is %d\n",count);
   TEST_ASSERT_EQUAL_INT16_MESSAGE(pdFALSE,result, "The updateCounter func did run");
   TEST_ASSERT_EQUAL_INT16_MESSAGE(0,count,"The counter did increment");
}

void testLock(void)
{
    
}

int main (void)
{
    stdio_init_all();
    while(true)
    {
        
        //sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(update_cnt_test_available);
        RUN_TEST(update_cnt_test_unavailable);
        //sleep_ms(5000);
        UNITY_END();
    }
    return 0;
}
