#pragma once

#include <stdbool.h>

#define START_BUTTON_PORT GPIOA
#define START_BUTTON_PIN GPIO_PIN_6

#define PRECHARGE_BUTTON_PORT GPIOA
#define PRECHARGE_BUTTON_PIN GPIO_PIN_7

#define PRECHARGE_DONE_BUTTON_PORT GPIOA
#define PRECHARGE_DONE_BUTTON_PIN GPIO_PIN_8

#define ENABLE_BUTTON_PORT GPIOA
#define ENABLE_BUTTON_PIN GPIO_PIN_9

// ORDER ON RELAY DRIVER:
// RR Activate
// RL Activate
// FR Activate
// FL Activate
// Interlock
// Precharge

#define PRECHARGE_RELAY_PORT GPIOA
#define PRECHARGE_RELAY_PIN GPIO_PIN_3

#define INTERLOCK_RELAY_PORT GPIOA
#define INTERLOCK_RELAY_PIN GPIO_PIN_4

#define RR_ACTIVATE_RELAY_PORT GPIOA
#define RR_ACTIVATE_RELAY_PIN GPIO_PIN_10

#define RL_ACTIVATE_RELAY_PORT GPIOB
#define RL_ACTIVATE_RELAY_PIN GPIO_PIN_10

#define FR_ACTIVATE_RELAY_PORT GPIOB
#define FR_ACTIVATE_RELAY_PIN GPIO_PIN_0

#define FL_ACTIVATE_RELAY_PORT GPIOA
#define FL_ACTIVATE_RELAY_PIN GPIO_PIN_5


void GPIO_init();
bool GPIO_start_button_pressed();
bool GPIO_precharge_button_pressed();
void GPIO_set_precharge_relay(bool on);
bool GPIO_precharge_done_button_pressed();
bool GPIO_enable_button_pressed();
void GPIO_set_activate_inv_relays(bool on);
void GPIO_set_interlock_relay(bool on);

void GPIO_toggle_precharge_relay();
void GPIO_toggle_interlock_relay();