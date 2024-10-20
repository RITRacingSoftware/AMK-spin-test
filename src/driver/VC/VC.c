#include "VC.h"
#include "core_config.h"

#include <stm32g4xx_hal.h>


#include "gpio.h"
#include "clock.h"
#include "can.h"

#define CAN FDCAN2
#define CAN_ID_1 3
#define CAN_ID_2 3

bool VC_init()
{
    // Drivers
    core_heartbeat_init(GPIOB, GPIO_PIN_9);
    core_GPIO_set_heartbeat(GPIO_PIN_RESET);

    if (!core_clock_init()) return false;
    if (!core_CAN_init(CAN)) return false;
    if (!core_CAN_add_filter(CAN, false,
                             CAN_ID_1, CAN_ID_2))
    {
        return false;
    }
    return true;
}

void toggle_heartbeat()
{
    core_GPIO_toggle_heartbeat();
}