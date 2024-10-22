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

    core_GPIO_pin_set(PRECHARGE_RELAY_PORT, PRECHARGE_RELAY_PIN, GPIO_PIN_RESET);
    core_GPIO_pin_set(INTERLOCK_RELAY_PORT, INTERLOCK_RELAY_PIN, GPIO_PIN_RESET);
}

bool start_button_pressed()
{
    return
}

bool precharge_button_pressed();
void set_precharge_relay(bool on);
bool precharge_done_button_pressed();
bool enable_button_pressed();
void set_activate_inv_relays(bool on);