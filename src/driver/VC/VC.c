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

#define CAN_ID_1 3
#define CAN_ID_2 3

#define TIME_DELAY 2000
#define TORQUE_SETPOINT 0
#define POS_TORQUE_LIMIT 0
#define NEG_TORQUE_LIMIT 0

VehicleState_e state;
int timer;
bool sent_inverter_enable_on;
bool sent_inverter_zeroes_shutdown;
bool sent_inverter_enable_off_shutdown;
bool sent_hv_off_shutdown;

static void new_state(VehicleState_e new)
{
    state = new;
}

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

    // Set default values
    state = VehicleState_VC_NOT_READY;
    sent_inverter_enable_on = false;
    sent_inverter_zeroes_shutdown = false;
    sent_inverter_enable_off_shutdown = false;
    timer = 0;
    return true;
}

void VC_state_100Hz()
{
    switch(state)
    {
        case VehicleState_VC_NOT_READY:
            // If the button is pressed, move to next state
            if (start_button_pressed())
            {
                new_state(VehicleState_INVERTERS_POWERED);
                timer = 0;
            }
            break;

        case VehicleState_INVERTERS_POWERED:
            // If all inverters are ready move to next state

            // AMK_bSystemReady = 1
            if (get_ready(INV_RR) &&
                get_ready(INV_RL) &&
                get_ready(INV_FR) &&
                get_ready(INV_RL))
            {
                new_state(VehicleState_INVERTERS_READY);
            }
            break;

        case VehicleState_INVERTERS_READY:
            // Wait TIME_DELAY
            if (timer < TIME_DELAY) break;

            // If precharge button is pressed, enable precharge relay
            if (precharge_button_pressed())
            {
                set_precharge_relay(true);
                new_state(VehicleState_PRECHARGING);
                timer = 0;
            }
            break;

        case VehicleState_PRECHARGING:
            // Wait TIME_DELAY
            if (timer < TIME_DELAY) break;

            // If precharge is confirmed finished, send it over CAN
            if (precharge_done_button_pressed())
            {
                CAN_tx_hv_on(true); // AMK_bDcOn = 1
            }

            // Receive echo for confirmation of precharge finishing
            // AMK_bDcOn = 1 MIRROR
            if (!(get_hv_on_echo(INV_RR) &&
                  get_hv_on_echo(INV_RL) &&
                  get_hv_on_echo(INV_FR) &&
                  get_hv_on_echo(INV_FL))) break;

            // Receive confirmation from inverters that they have been precharged
            // AMK_bQuitDcOn = 1
            if (!(get_hv_on(INV_RR) &&
                  get_hv_on(INV_RL) &&
                  get_hv_on(INV_FR) &&
                  get_hv_on(INV_FL))) break;

            // Complete interlock from VC side, allow full HV to go through
            set_interlock_relay(true);
            new_state(VehicleState_WAIT);
            timer = 0;
            break;

        case VehicleState_WAIT:

            // Every 200ms send 0 on torque requests
            if (timer >= 200)
            {
                CAN_tx_actual_speed_value(0);
                CAN_tx_torque_request(0, 0, 0);
                timer = 0;
            }

            if (enable_button_pressed())
            {
                new_state(VehicleState_STANDBY);
            }
            break;

        case VehicleState_STANDBY:
            // Send inverter enable and on
            if (!sent_inverter_enable_on)
            {
                CAN_tx_inv_enable(true); // AMK_bEnable = 1
                CAN_tx_inv_on(true); // AMLK_bInverterOn = 1
                sent_inverter_enable_on = true;
            }

            // Receive echo for inverters commanded on
            // AMK_bInverterOn = 1 MIRROR
            if (!(get_inv_on_echo(INV_RR) &&
                  get_inv_on_echo(INV_RL) &&
                  get_inv_on_echo(INV_FR) &&
                  get_inv_on_echo(INV_FL))) break;

            // Receive confirmation that inverters are on
            // AMK_bQuitInverterOn = 1
            if (!(get_inv_on(INV_RR) &&
                  get_inv_on(INV_RL) &&
                  get_inv_on(INV_FR) &&
                  get_inv_on(INV_FL))) break;

            // Switch relay allowing inverters to read real torque requests
            // X140 binary input BE2 = 1
            set_activate_inv_relays(true);

            new_state(VehicleState_RUNNING);
            timer = 0;
            break;

        case VehicleState_RUNNING:
            // Every 200ms send torque requests at set values
            if (timer >= 200)
            {
                CAN_tx_torque_request(TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);
                timer = 0;
            }

            // If the start button is pressed again, shutdown
            if (start_button_pressed())
            {
                new_state(VehicleState_SHUTDOWN);
            }

        case VehicleState_SHUTDOWN:
            // Send zeroes for torque requests, turn off activation relay, send inverter off message
            if (!sent_inverter_zeroes_shutdown)
            {
                CAN_tx_torque_request(0, 0, 0);
                set_activate_inv_relays(false); // X140 binary input BE2 = 0
                CAN_tx_inv_on(false); // AMK_bInverterOn = 0
                sent_inverter_zeroes_shutdown = true;
            }

            // Receive echo for inverters being commanded off
            // AMK_bInverterOn = 0 MIRROR
            if (get_inv_on_echo(INV_RR) ||
                get_inv_on_echo(INV_RL) ||
                get_inv_on_echo(INV_FR) ||
                get_inv_on_echo(INV_FL)) break;

            // Turn off inverter enables
            if (!sent_inverter_enable_off_shutdown)
            {
                CAN_tx_inv_enable(false); // AMK_bEnable = 0
                sent_inverter_enable_off_shutdown = true;
            }

            // Receive confirmation that inverters are off
            //AMK_bQuitInverterOn = 0
            if (get_inv_on(INV_RR) ||
                get_inv_on(INV_RL) ||
                get_inv_on(INV_FR) ||
                get_inv_on(INV_FL)) break;

            // Send HV bus off message
            if (!sent_hv_off_shutdown)
            {
                CAN_tx_hv_on(false); // AMK_bDcOn = 0
                sent_hv_off_shutdown = true;
            }

            // Receive echo for HV bus being off
            // AMK_bDcOn = 0 MIRROR
            if (get_hv_on_echo(INV_RR) ||
                get_hv_on_echo(INV_RL) ||
                get_hv_on_echo(INV_FR) ||
                get_hv_on_echo(INV_FL)) break;

            // Receive confirmation that HV bus is off
            // AMK_bQitDcOn = 0
            if (get_hv_on(INV_RR) ||
                get_hv_on(INV_RL) ||
                get_hv_on(INV_FR) ||
                get_hv_on(INV_FL)) break;

            // Kill interlock
            set_interlock_relay(false);
            break;
    }

    timer += 10;
}

void toggle_heartbeat()
{
    core_GPIO_toggle_heartbeat();
}