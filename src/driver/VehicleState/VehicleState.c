#include "VehicleState.h"
#include "config.h"

#include "gpio.h"
#include "can.h"
#include "Inverters/Inverters.h"
#include "CAN/driver_can.h"
#include "GPIO/driver_GPIO.h"


#define TIME_DELAY 0
#define INV INV_RL

static VehicleState_e state;
static int timer;

static bool sent_enable = false;

static void new_state(VehicleState_e new)
{
    state = new;
}

void VehicleState_init()
{
    // Set default values
    state = VehicleState_VC_NOT_READY;
    timer = 0;
}

void VehicleState_100Hz()
{
    GPIO_toggle_precharge_relay();
    GPIO_toggle_interlock_relay();
    switch(state)
    {
        case VehicleState_VC_NOT_READY:
            // If the button is pressed, move to next state
            core_GPIO_set_heartbeat(true);
            if (GPIO_start_button_pressed())
            {
                new_state(VehicleState_INVERTERS_POWERED);
                timer = 0;
            }
            break;

        case VehicleState_INVERTERS_POWERED:
            // If all inverters are ready move to next state
            core_GPIO_set_heartbeat(false);

            // AMK_bSystemReady = 1
            if (Inverters_get_ready(INV_RR) &&
                Inverters_get_ready(INV_RL) &&
                Inverters_get_ready(INV_FR) &&
                Inverters_get_ready(INV_FL))
//            if (Inverters_get_ready(INV_RL) &&
//            Inverters_get_ready(INV_FL) &&
//            Inverters_get_ready(INV_FR))
            {
                new_state(VehicleState_INVERTERS_READY);
            }
            break;

        case VehicleState_INVERTERS_READY:
            core_GPIO_set_heartbeat(true);
            // Wait TIME_DELAY
            if (timer < TIME_DELAY) break;

            // If precharge button is pressed, enable precharge relay
            if (GPIO_precharge_button_pressed())
            {
//                GPIO_set_precharge_relay(true);
                new_state(VehicleState_PRECHARGING);
                timer = 0;
            }
            break;

        case VehicleState_PRECHARGING:
            // Wait TIME_DELAY
            core_GPIO_set_heartbeat(false);
//            if (timer < TIME_DELAY) break;

            // If precharge is confirmed finished, send it over CAN
            if (GPIO_precharge_done_button_pressed())
            {
                Inverters_set_dc_on(true); // AMK_bDcOn = 1
            }

            // Receive echo for confirmation of precharge finishing
            // AMK_bDcOn = 1 MIRROR
            if (!(Inverters_get_dc_on_echo(INV_RR) &&
                  Inverters_get_dc_on_echo(INV_RL) &&
                  Inverters_get_dc_on_echo(INV_FR) &&
                  Inverters_get_dc_on_echo(INV_FL))) break;
//            if (!Inverters_get_dc_on_echo(INV)) break;

            // Receive confirmation from inverters that they have been precharged
            // AMK_bQuitDcOn = 1
            if (!(Inverters_get_dc_on(INV_RR) &&
                  Inverters_get_dc_on(INV_RL) &&
                  Inverters_get_dc_on(INV_FR) &&
                  Inverters_get_dc_on(INV_FL))) break;
//            if (!Inverters_get_dc_on(INV)) break;

            // Complete interlock from VC side, allow full HV to go through
//            GPIO_set_interlock_relay(true);
            new_state(VehicleState_WAIT);
            timer = 0;
            break;

        case VehicleState_WAIT:
            core_GPIO_set_heartbeat(true);
            Inverters_set_torque_request(INV_RR, 0, 0, 0);
            Inverters_set_torque_request(INV_RL, 0, 0, 0);
            Inverters_set_torque_request(INV_FR, 0, 0, 0);
            Inverters_set_torque_request(INV_FL, 0, 0, 0);
            if (GPIO_enable_button_pressed())
            {
                new_state(VehicleState_STANDBY);
            }
            break;

        case VehicleState_STANDBY:
            core_GPIO_set_heartbeat(false);
            // Set inverter enable and on

            if (!sent_enable)
            {
                Inverters_set_enable(true); // AMK_bEnable = 1
//                Inverters_send_setpoints(INV_RR);
                Inverters_send_setpoints(INV_RL);
                Inverters_send_setpoints(INV_FR);
                Inverters_send_setpoints(INV_FL);

                Inverters_set_enable(false);
                Inverters_send_setpoints(INV_RL);
                Inverters_send_setpoints(INV_FL);
                Inverters_send_setpoints(INV_FR);
                sent_enable = true;
            }
            else Inverters_set_enable(true); // AMK_bEnable = 1
            Inverters_set_inv_on(true); // AMLK_bInverterOn = 1

            // Receive echo for inverters commanded on
            // AMK_bInverterOn = 1 MIRROR
            if (!(Inverters_get_inv_on_echo(INV_RR) &&
                  Inverters_get_inv_on_echo(INV_RL) &&
                  Inverters_get_inv_on_echo(INV_FR) &&
                  Inverters_get_inv_on_echo(INV_FL))) break;
//            if (!(Inverters_get_inv_on_echo(INV_RL) && Inverters_get_inv_on_echo(INV_FL) &&
//                    Inverters_get_inv_on_echo(INV_FR))) break;
//            if (!Inverters_get_inv_on_echo(INV_FL)) break;

            // Receive confirmation that inverters are on
            // AMK_bQuitInverterOn = 1
            if (!(Inverters_get_inv_on(INV_RR) &&
                  Inverters_get_inv_on(INV_RL) &&
                  Inverters_get_inv_on(INV_FR) &&
                  Inverters_get_inv_on(INV_FL))) break;
//            if (!(Inverters_get_inv_on(INV_RL) && Inverters_get_inv_on(INV_FL) &&
//                    Inverters_get_inv_on(INV_FR))) break;
//            if (!Inverters_get_inv_on(INV_FL)) break;

            // Switch relay allowing inverters to read real torque requests
            // X140 binary input BE2 = 1
            GPIO_set_activate_inv_relays(true);

            new_state(VehicleState_RUNNING);
            timer = 0;
            break;

        case VehicleState_RUNNING:
            core_GPIO_set_heartbeat(true);
            Inverters_set_torque_request(INV_RR, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);
            Inverters_set_torque_request(INV_RL, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);
            Inverters_set_torque_request(INV_FR, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);
            Inverters_set_torque_request(INV_FL, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);

            // If the start button is pressed again, shutdown
            if (GPIO_start_button_pressed())
            {
                new_state(VehicleState_SHUTDOWN);
            }
            break;

        case VehicleState_SHUTDOWN:
            core_GPIO_set_heartbeat(false);
            // Send zeroes for torque requests, turn off activation relay, send inverter off message
            Inverters_set_torque_request(INV_RL,  0, 0, 0);
            Inverters_set_torque_request(INV_FL,  0, 0, 0);
            Inverters_set_torque_request(INV_RR, 0, 0, 0);
            Inverters_set_torque_request(INV_FR, 0, 0, 0);
            GPIO_set_activate_inv_relays(false); // X140 binary input BE2 = 0
            Inverters_set_inv_on(false); // AMK_bInverterOn = 0

            // Receive echo for inverters being commanded off
            // AMK_bInverterOn = 0 MIRROR
            if (Inverters_get_inv_on_echo(INV_RR) ||
                Inverters_get_inv_on_echo(INV_RL) ||
                Inverters_get_inv_on_echo(INV_FR) ||
                Inverters_get_inv_on_echo(INV_FL)) break;
//            if (Inverters_get_inv_on_echo(INV_RL) || Inverters_get_inv_on_echo(INV_FL) ||
//            Inverters_get_inv_on_echo(INV_FR)) break;
//            if (Inverters_get_inv_on_echo(INV_FL)) break;

            // Set inverter enables off
            Inverters_set_enable(false); // AMK_bEnable = 0

            // Receive confirmation that inverters are off
            //AMK_bQuitInverterOn = 0
            if (Inverters_get_inv_on(INV_RR) ||
                Inverters_get_inv_on(INV_RL) ||
                Inverters_get_inv_on(INV_FR) ||
                Inverters_get_inv_on(INV_FL)) break;
//            if (Inverters_get_inv_on(INV_RL) || Inverters_get_inv_on(INV_FL) ||
//            Inverters_get_inv_on(INV_FR)) break;
//            if (Inverters_get_inv_on(INV_FL)) break;

            // Send DC bus off message
            Inverters_set_dc_on(false); // AMK_bDcOn = 0

            // Receive echo for DC bus being off
            // AMK_bDcOn = 0 MIRROR
            if (Inverters_get_dc_on_echo(INV_RR) ||
                Inverters_get_dc_on_echo(INV_RL) ||
                Inverters_get_dc_on_echo(INV_FR) ||
                Inverters_get_dc_on_echo(INV_FL)) break;
//            if (Inverters_get_dc_on_echo(INV)) break;

            // Receive confirmation that DC bus is off
            // AMK_bQitDcOn = 0
            if (Inverters_get_dc_on(INV_RR) ||
                Inverters_get_dc_on(INV_RL) ||
                Inverters_get_dc_on(INV_FR) ||
                Inverters_get_dc_on(INV_FL)) break;
//            if (Inverters_get_dc_on(INV)) break;

            // Kill interlock
//            GPIO_set_interlock_relay(false);
            break;
    }

    timer += 10;
}
