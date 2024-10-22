#include "driver_GPIO.h"

#include "GPIO.h"

void GPIO_init()
{
    core_GPIO_init(START_BUTTON_PORT, START_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    core_GPIO_init(PRECHARGE_BUTTON_PORT, PRECHARGE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    core_GPIO_init(PRECHARGE_RELAY_PORT, PRECHARGE_RELAY_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN);
    core_GPIO_init(PRECHARGE_DONE_BUTTON_PORT, PRECHARGE_DONE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    core_GPIO_init(ENABLE_BUTTON_PORT, ENABLE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    core_GPIO_init(INTERLOCK_RELAY_PORT, INTERLOCK_RELAY_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN);

    core_GPIO_digital_write(PRECHARGE_RELAY_PORT, PRECHARGE_RELAY_PIN, false);
    core_GPIO_digital_write(INTERLOCK_RELAY_PORT, INTERLOCK_RELAY_PIN, false);
}

bool GPIO_start_button_pressed() {return !core_GPIO_digital_read(START_BUTTON_PORT, START_BUTTON_PIN);}

bool GPIO_precharge_button_pressed() {return !core_GPIO_digital_read(PRECHARGE_BUTTON_PORT, PRECHARGE_BUTTON_PIN);}

void GPIO_set_precharge_relay(bool on) {core_GPIO_digital_write(PRECHARGE_RELAY_PORT, PRECHARGE_RELAY_PIN, true);}

bool GPIO_precharge_done_button_pressed() {return !core_GPIO_digital_read(PRECHARGE_DONE_BUTTON_PORT, PRECHARGE_DONE_BUTTON_PIN);}

bool GPIO_enable_button_pressed() {return !core_GPIO_digital_read(ENABLE_BUTTON_PORT, ENABLE_BUTTON_PIN);}

void GPIO_set_activate_inv_relays(bool on)
{
    core_GPIO_digital_write(RR_ACTIVATE_RELAY_PORT, RR_ACTIVATE_RELAY_PIN, true);
    core_GPIO_digital_write(RL_ACTIVATE_RELAY_PORT, RL_ACTIVATE_RELAY_PIN, true);
    core_GPIO_digital_write(FR_ACTIVATE_RELAY_PORT, FR_ACTIVATE_RELAY_PIN, true);
    core_GPIO_digital_write(FL_ACTIVATE_RELAY_PORT, FL_ACTIVATE_RELAY_PIN, true);
}