#include "VC.h"
#include "core_config.h"

#include <stm32g4xx_hal.h>
#include <stdint.h>

#include "gpio.h"
#include "clock.h"
#include "can.h"
#include "Inverters/Inverters.h"
#include "CAN/driver_can.h"
#include "GPIO/driver_GPIO.h"
#include "VehicleState/VehicleState.h"
#include "Accel/Accel.h"
#include "error_handler.h"
#include "usart.h"

bool state;

bool VC_init()
{
    HAL_Init();
    // Inits
    if (!core_clock_init()) return false;
    if (!core_USART_init(USART3, 500000)) return false;
    if (!core_CAN_init(CAN_INV)) return false;
    if (!core_CAN_init(CAN_MAIN)) return false;
    if (!CAN_add_filters()) return false;
//    uprintf(USART3, "test\n");

    state = true;
    GPIO_init();
    VehicleState_init();
    Inverters_init();
    Accel_init();

    return true;
}

void VC_100Hz()
{
//    if (!Accel_update_task()) error_handler();
    VehicleState_100Hz();
    Inverters_100Hz();


    core_CAN_add_message_to_tx_queue(FDCAN3, 3, 2, 0xfa55);
//    core_GPIO_digital_write(RR_ACTIVATE_RELAY_PORT, RR_ACTIVATE_RELAY_PIN, state);
//    state = !state;
//    uprintf(USART3, "Before: CCCR: %08x, PSR: %08x, ECR: %08x\n", FDCAN3->CCCR, FDCAN3->PSR, FDCAN3->ECR);
//    core_CAN_send_message(FDCAN3, 3, 2, 0xfa55);
//    uprintf(USART3, "After: CCCR: %08x, PSR: %08x, ECR: %08x\n", FDCAN3->CCCR, FDCAN3->PSR, FDCAN3->ECR);
//    core_GPIO_toggle_heartbeat();
}

void toggle_heartbeat()
{
//    core_GPIO_digital_write(MAIN_LED_PORT, MAIN_LED_PIN, true);
//    core_GPIO_toggle_heartbeat();
}

void set_heartbeat(bool on)
{
    core_GPIO_set_heartbeat(on);
}
