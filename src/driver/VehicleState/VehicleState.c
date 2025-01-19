#include "VehicleState.h"
#include "config.h"

#include "gpio.h"
#include "can.h"
#include "Inverters/Inverters.h"
#include "CAN/driver_can.h"
#include "GPIO/driver_GPIO.h"
#include "Accel/Accel.h"


#define TIME_DELAY 2000

static VehicleState_e state;
static int timer;

unsigned long heartbeatTimes = 0;

static bool forward;

static uint8_t rtd_debounce_count;

static void new_state(VehicleState_e new)
{
    state = new;
}

void VehicleState_init()
{
    // Set default values
    state = VehicleState_VC_NOT_READY;
    timer = 0;
    rtd_debounce_count = 0;
    forward = true;
}

void VehicleState_100Hz()
{
    if (heartbeatTimes%10 == 0) core_GPIO_toggle_heartbeat();
    switch(state)
    {
        case VehicleState_VC_NOT_READY:
            // If the button is pressed, move to next state
            if (core_GPIO_digital_read(TSMS_PORT, TSMS_PIN))
            {
                new_state(VehicleState_INVERTERS_POWERED);
                timer = 0;
            }
            break;

        case VehicleState_INVERTERS_POWERED:
            // If all inverters are ready move to next state
            core_GPIO_digital_write(MAIN_LED_PORT, MAIN_LED_PIN, true);

            core_GPIO_digital_write(FL_STATUS_PORT, FL_STATUS_PIN, Inverters_get_ready(INV_FL));
            core_GPIO_digital_write(FR_STATUS_PORT, FR_STATUS_PIN, Inverters_get_ready(INV_FR));
            core_GPIO_digital_write(RL_STATUS_PORT, RL_STATUS_PIN, Inverters_get_ready(INV_RL));
            // AMK_bSystemReady = 1
            if (/*Inverters_get_ready(INV_RR) &&*/
                Inverters_get_ready(INV_RL) &&
                Inverters_get_ready(INV_FR) &&
                Inverters_get_ready(INV_FL))
            {
                new_state(VehicleState_PRECHARGING);
            }
            timer = 0;
            break;

        case VehicleState_PRECHARGING:
            core_GPIO_digital_write(AMK_LED_PORT, AMK_LED_PIN, true);
            GPIO_set_precharge_relay(true);

            // Wait TIME_DELAY
//            if (timer < TIME_DELAY) break;

            // If precharge is finished with all 4, confirm precharge done
            if (!(/*Inverters_get_precharged(INV_RR) &&*/
                  Inverters_get_precharged(INV_RL) &&
                  Inverters_get_precharged(INV_FR) &&
                  Inverters_get_precharged(INV_FL))) break;

//            core_GPIO_digital_write(FL_STATUS_PORT, FL_STATUS_PIN, true);

            Inverters_set_dc_on(true); // AMK_bDcOn = 1

            // Receive echo for confirmation of precharge finishing
            // AMK_bDcOn = 1 MIRROR
            if (!(/*Inverters_get_dc_on_echo(INV_RR) &&*/
                  Inverters_get_dc_on_echo(INV_RL) &&
                  Inverters_get_dc_on_echo(INV_FR) &&
                  Inverters_get_dc_on_echo(INV_FL))) break;

            // Receive confirmation from inverters that they have been precharged
            // AMK_bQuitDcOn = 1
            if (!(/*Inverters_get_dc_on(INV_RR) &&*/
                  Inverters_get_dc_on(INV_RL) &&
                  Inverters_get_dc_on(INV_FR) &&
                  Inverters_get_dc_on(INV_FL))) break;

            // Complete interlock from VC side, allow full HV to go through
            GPIO_set_interlock_relay(true);
            GPIO_set_precharge_relay(false);
            new_state(VehicleState_WAIT);
            timer = 0;
            break;

        case VehicleState_WAIT:
            core_GPIO_digital_write(SENSOR_LED_PORT, SENSOR_LED_PIN, true);
            Inverters_set_torque_request(INV_RR, 0, 0, 0);
            Inverters_set_torque_request(INV_RL, 0, 0, 0);
            Inverters_set_torque_request(INV_FR, 0, 0, 0);
            Inverters_set_torque_request(INV_FL, 0, 0, 0);
            if (GPIO_get_RTD())
            {
                new_state(VehicleState_STANDBY);
            }
            break;

        case VehicleState_STANDBY:
            core_GPIO_digital_write(MAIN_LED_PORT, MAIN_LED_PIN, false);

            // Set inverter enable and on
            Inverters_set_enable(true); // AMK_bEnable = 1
            Inverters_set_inv_on(true); // AMLK_bInverterOn = 1

            // Receive echo for inverters commanded on
            // AMK_bInverterOn = 1 MIRROR
            if (!(/*Inverters_get_inv_on_echo(INV_RR) &&*/
                  /*Inverters_get_inv_on_echo(INV_RL) &&
                  Inverters_get_inv_on_echo(INV_FR) &&*/
                  Inverters_get_inv_on_echo(INV_FL))) break;

            // Receive confirmation that inverters are on
            // AMK_bQuitInverterOn = 1
            if (!(/*Inverters_get_inv_on(INV_RR) &&*/
                  /*Inverters_get_inv_on(INV_RL) &&
                  Inverters_get_inv_on(INV_FR) &&*/
                  Inverters_get_inv_on(INV_FL))) break;

            // Switch relay allowing inverters to read real torque requests
            // X140 binary input BE2 = 1
            GPIO_set_activate_inv_relays(true);

            new_state(VehicleState_RUNNING);
            timer = 0;
            break;

        case VehicleState_RUNNING:
            core_GPIO_digital_write(AMK_LED_PORT, AMK_LED_PIN, false);
//            Inverters_set_torque_request(INV_RR, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);
//            Inverters_set_torque_request(INV_RL, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);
//            Inverters_set_torque_request(INV_FR, TORQUE_SETPOINT, NEG_TORQUE_LIMIT, POS_TORQUE_LIMIT);


            if (!GPIO_get_RTD()) rtd_debounce_count = 0;
            else if (rtd_debounce_count >= 50)
            {
                if (rtd_debounce_count == 50)
                    forward = !forward;
                rtd_debounce_count  = 51;
            }
            else rtd_debounce_count += 1;

            Inverters_set_torque_request(INV_FL, (int)(Accel_get_pos() * MAX_TORQUE) * (forward? 1 : -1)
                                         , -(MAX_TORQUE), MAX_TORQUE);

            // If the start button is pressed again, shutdown
            if (!GPIO_get_TSMS())
            {
                new_state(VehicleState_SHUTDOWN);
            }
            break;

        case VehicleState_SHUTDOWN:
            core_GPIO_digital_write(SENSOR_LED_PORT, SENSOR_LED_PIN, false);
            // Send zeroes for torque requests, turn off activation relay, send inverter off message
            Inverters_set_torque_request(INV_RL,  0, 0, 0);
            Inverters_set_torque_request(INV_FL,  0, 0, 0);
            Inverters_set_torque_request(INV_RR, 0, 0, 0);
            Inverters_set_torque_request(INV_FR, 0, 0, 0);
            GPIO_set_activate_inv_relays(false); // X140 binary input BE2 = 0
            Inverters_set_inv_on(false); // AMK_bInverterOn = 0

            // Receive echo for inverters being commanded off
            // AMK_bInverterOn = 0 MIRROR
            if (/*Inverters_get_inv_on_echo(INV_RR) ||*/
                Inverters_get_inv_on_echo(INV_RL) ||
                Inverters_get_inv_on_echo(INV_FR) ||
                Inverters_get_inv_on_echo(INV_FL)) break;

            // Set inverter enables off
            Inverters_set_enable(false); // AMK_bEnable = 0

            // Receive confirmation that inverters are off
            //AMK_bQuitInverterOn = 0
            if (/*Inverters_get_inv_on(INV_RR) ||*/
                Inverters_get_inv_on(INV_RL) ||
                Inverters_get_inv_on(INV_FR) ||
                Inverters_get_inv_on(INV_FL)) break;

            // Send DC bus off message
            Inverters_set_dc_on(false); // AMK_bDcOn = 0

            // Receive echo for DC bus being off
            // AMK_bDcOn = 0 MIRROR
            if (/*Inverters_get_dc_on_echo(INV_RR) ||*/
                Inverters_get_dc_on_echo(INV_RL) ||
                Inverters_get_dc_on_echo(INV_FR) ||
                Inverters_get_dc_on_echo(INV_FL)) break;
//            if (Inverters_get_dc_on_echo(INV)) break;

            // Receive confirmation that DC bus is off
            // AMK_bQitDcOn = 0
            if (/*Inverters_get_dc_on(INV_RR) ||*/
                Inverters_get_dc_on(INV_RL) ||
                Inverters_get_dc_on(INV_FR) ||
                Inverters_get_dc_on(INV_FL)) break;

            // Kill interlock
            GPIO_set_interlock_relay(false);
            break;
    }

    timer += 10;
    heartbeatTimes += 1;
}
