#pragma once

#include <stdbool.h>

#define START_BUTTON_PORT GPIOA
#define START_BUTTON_PIN GPIO_PIN_0

#define PRECHARGE_BUTTON_PORT GPIOA
#define PRECHARGE_BUTTON_PIN GPIO_PIN_2

#define PRECHARGE_RELAY_PORT GPIOA
#define PRECHARGE_RELAY_PIN GPIO_PIN_2

#define PRECHARGE_DONE_BUTTON_PORT GPIOA
#define PRECHARGE_DONE_BUTTON_PIN GPIO_PIN_3

#define ENABLE_BUTTON_PORT GPIOA
#define ENABLE_BUTTON_PIN GPIO_PIN_4

#define RR_ACTIVATE_RELAY_PORT GPIOA
#define RR_ACTIVATE_RELAY_PIN GPIO_PIN_5

#define RL_ACTIVATE_RELAY_PORT GPIOA
#define RL_ACTIVATE_RELAY_PIN GPIO_PIN_6

#define FR_ACTIVATE_RELAY_PORT GPIOA
#define FR_ACTIVATE_RELAY_PIN GPIO_PIN_7

#define FL_ACTIVATE_RELAY_PORT GPIOA
#define FL_ACTIVATE_RELAY_PIN GPIO_PIN_8

#define INTERLOCK_RELAY_PORT GPIOA
#define INTERLOCK_RELAY_PIN GPIO_PIN_9


void GPIO_init();
bool start_button_pressed();
bool precharge_button_pressed();
void set_precharge_relay(bool on);
bool precharge_done_button_pressed();
bool enable_button_pressed();
void set_activate_inv_relays(bool on);
void set_interlock_relay(bool on);