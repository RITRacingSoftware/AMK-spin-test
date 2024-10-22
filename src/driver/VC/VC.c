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

#define CAN_ID_1 3
#define CAN_ID_2 3

#define TIME_DELAY 2000
#define TORQUE_SETPOINT 0
#define POS_TORQUE_LIMIT 0
#define NEG_TORQUE_LIMIT 0

bool VC_init()
{

    // Inits
    core_heartbeat_init(GPIOB, GPIO_PIN_9);
    core_GPIO_set_heartbeat(GPIO_PIN_RESET);

    if (!core_clock_init()) return false;
    if (!core_CAN_init(FDCAN2)) return false;
    if (!core_CAN_add_filter(FDCAN2, false,
                             CAN_ID_1, CAN_ID_2))
    {
        return false;
    }

    GPIO_init();
    VehicleState_init();

    return true;
}


void toggle_heartbeat()
{
    core_GPIO_toggle_heartbeat();
}