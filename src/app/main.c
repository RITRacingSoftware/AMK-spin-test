#include "main.h"

#include <stm32g4xx_hal.h>
#include <stdbool.h>

#include "VC/VC.h"
#include "CAN/driver_can.h"
#include "Inverters/Inverters.h"
#include "GPIO/driver_GPIO.h"

#include "FreeRTOS.h"
#include "task.h"

#define VC_100HZ_PRIORITY (tskIDLE_PRIORITY + 1)
#define CAN_RX_PRIORITY (tskIDLE_PRIORITY + 4)
#define CAN_TX_PRIORITY (tskIDLE_PRIORITY + 4)

void hardfault_error_handler();

void task_CAN_tx(void *pvParameters)
{
    (void) pvParameters;
    if (!CAN_tx()) hardfault_error_handler();
}

void task_CAN_rx(void *pvParameters)
{
    (void) pvParameters;
    TickType_t next_wake_time = xTaskGetTickCount();
    while(true)
    {
        CAN_rx();
        vTaskDelayUntil(&next_wake_time, 1);
    }
}

void task_100Hz(void *pvParameters)
{
    (void) pvParameters;
    TickType_t next_wake_time = xTaskGetTickCount();
    while(true)
    {
        VC_100Hz();
        vTaskDelayUntil(&next_wake_time, 10);
    }
}

void task_heartbeat(void *pvParameters)
{
    (void) pvParameters;
    TickType_t next_wake_time = xTaskGetTickCount();
    while(true)
    {
//        toggle_heartbeat();
        vTaskDelayUntil(&next_wake_time, 500);
    }
}

int main(void)
{
    if (!VC_init()) hardfault_error_handler();

    int err;

    err = xTaskCreate(task_CAN_tx,
        "CAN_tx",
        1000,
        NULL,
        CAN_TX_PRIORITY,
        NULL);
    if (err != pdPASS) hardfault_error_handler();

    err = xTaskCreate(task_CAN_rx,
        "CAN_rx",
        5000,
        NULL,
        CAN_RX_PRIORITY,
        NULL);
    if (err != pdPASS) hardfault_error_handler();

    err = xTaskCreate(task_heartbeat,
        "heartbeat_task",
        1000,
        NULL,
        1,
        NULL);
    if (err != pdPASS) hardfault_error_handler();

    err = xTaskCreate(task_100Hz,
          "100hz_task",
          1000,
          NULL,
          VC_100HZ_PRIORITY,
          NULL);
    if (err != pdPASS) hardfault_error_handler();

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    // hand control over to FreeRTOS
    vTaskStartScheduler();

    // we should not get here ever
    hardfault_error_handler();
    return 1;
}

// Called when stack overflows from rtos
// Not needed in header, since included in FreeRTOS-Kernel/include/task.h
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{
    (void) xTask;
    (void) pcTaskName;

    hardfault_error_handler();
}

void hardfault_error_handler()
{
    while(1)
    {
        toggle_heartbeat();
        for (unsigned long long  i = 0; i < 200000; i++);
    }
}