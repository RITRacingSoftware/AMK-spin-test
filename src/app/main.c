#include "main.h"

#include <stm32g4xx_hal.h>
#include <stdbool.h>

#include "can.h"
#include "clock.h"
#include "gpio.h"
#include "error_handler.h"

#include "FreeRTOS.h"
#include "task.h"


#define TEST_CAN_ID1 3
#define TEST_CAN_ID2 3

CanMessage_s canMessage;

void task1(void *pvParameters)
{
    (void) pvParameters;
    while(true)
    {
        if (!core_CAN_add_message_to_tx_queue(FDCAN2, 3, 2, 0xfa55)) error_handler();
        if (core_CAN_send_from_tx_queue_task(FDCAN2))
        {
            core_GPIO_toggle_heartbeat();
        }
//        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void heartbeat_task(void *pvParameters)
{
    (void) pvParameters;
    while(true)
    {
        if (core_CAN_add_message_to_tx_queue(FDCAN2, 3, 2, 0xfa55)) core_GPIO_toggle_heartbeat();
        vTaskDelay(100 * portTICK_PERIOD_MS);
    }
}

int main(void)
{
    HAL_Init();

    // Drivers
    core_heartbeat_init(GPIOB, GPIO_PIN_9);
    core_GPIO_set_heartbeat(GPIO_PIN_RESET);

    if (!core_clock_init()) error_handler();
    if (!core_CAN_init(FDCAN2)) error_handler();
    if (!core_CAN_add_filter(FDCAN2, false,
                             TEST_CAN_ID1, TEST_CAN_ID2))
    {
        error_handler();
    }

    int err = xTaskCreate(task1,
                          "Task1",
                          1000,
                          NULL,
                          4,
                          NULL);
    if (err != pdPASS) {
        error_handler();
    }

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // hand control over to FreeRTOS
    vTaskStartScheduler();

    // we should not get here ever
    error_handler();
    return 1;
}

// Called when stack overflows from rtos
// Not needed in header, since included in FreeRTOS-Kernel/include/task.h
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{
    (void) xTask;
    (void) pcTaskName;

    error_handler();
}