#include "main.h"

#include <stm32g4xx_hal.h>
#include <stdbool.h>

#include "VC/VC.h"
#include "CAN/driver_can.h"
#include "Inverters/Inverters.h"

#include "FreeRTOS.h"
#include "task.h"

void hardfault_error_handler();

void task_CAN_tx(void *pvParameters)
{
    (void) pvParameters;
    CAN_tx();
}

void task_CAN_rx(void *pvParameters)
{
    (void) pvParameters;
    while(true)
    {
        CAN_rx();
    }
}


void task_100Hz(void *pvParameters)
{
    (void) pvParameters;
    TickType_t next_wake_time = xTaskGetTickCount();
    while(true)
    {
        Inverters_100Hz();
        VehicleControllerState_100Hz();
        vTaskDelayUntil(&next_wake_time, 10);
    }
}

void task_heartbeat(void *pvParameters)
{
    (void) pvParameters;
    while(true)
    {
        toggle_heartbeat();
        vTaskDelay(100 * portTICK_PERIOD_MS);
    }
}

int main(void)
{
    HAL_Init();
    VC_init();

    int err = xTaskCreate(task_CAN_tx,
        "CAN_tx",
        1000,
        NULL,
        3,
        NULL);
    if (err != pdPASS) hardfault_error_handler();

    err = xTaskCreate(task_CAN_rx,
        "CAN_rx",
        1000,
        NULL,
        3,
        NULL);
    if (err != pdPASS) hardfault_error_handler();

    err = xTaskCreate(task_heartbeat,
        "heartbeat_task",
        1000,
        NULL,
        4,
        NULL);
    if (err != pdPASS) hardfault_error_handler();

    err = xTaskCreate(task_100Hz,
          "100hz_task",
          1000,
          NULL,
          4,
          NULL);
    if (err != pdPASS) hardfault_error_handler();

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