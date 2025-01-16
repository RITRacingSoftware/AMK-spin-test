#pragma once

#include <stdbool.h>

#define TSMS_PORT GPIOC
#define TSMS_PIN GPIO_PIN_5

#define RTD_BUTTON_PORT GPIOC
#define RTD_BUTTON_PIN GPIO_PIN_4

// ORDER ON RELAY DRIVER:
// RR Activate
// RL Activate
// FR Activate
// FL Activate
// Interlock
// Precharge

#define PRECHARGE_RELAY_PORT GPIOA
#define PRECHARGE_RELAY_PIN GPIO_PIN_1

#define AIR1_PORT GPIOA
#define AIR1_PIN GPIO_PIN_0

#define RR_ACTIVATE_RELAY_PORT GPIOC
#define RR_ACTIVATE_RELAY_PIN GPIO_PIN_2

#define RL_ACTIVATE_RELAY_PORT GPIOC
#define RL_ACTIVATE_RELAY_PIN GPIO_PIN_3

#define FR_ACTIVATE_RELAY_PORT GPIOC
#define FR_ACTIVATE_RELAY_PIN GPIO_PIN_1

#define FL_ACTIVATE_RELAY_PORT GPIOC
#define FL_ACTIVATE_RELAY_PIN GPIO_PIN_0

#define VC_LED_PORT GPIOC
#define VC_LED_PIN GPIO_PIN_6

#define MAIN_LED_PORT GPIOB
#define MAIN_LED_PIN GPIO_PIN_15

#define AMK_LED_PORT GPIOC
#define AMK_LED_PIN GPIO_PIN_8

#define SENSOR_LED_PORT GPIOC
#define SENSOR_LED_PIN GPIO_PIN_7

#define RR_STATUS_PORT GPIOA
#define RR_STATUS_PIN GPIO_PIN_10

#define RL_STATUS_PORT GPIOA
#define RL_STATUS_PIN GPIO_PIN_9

#define FR_STATUS_PORT GPIOA
#define FR_STATUS_PIN GPIO_PIN_8

#define FL_STATUS_PORT GPIOC
#define FL_STATUS_PIN GPIO_PIN_9


void GPIO_init();
void GPIO_set_precharge_relay(bool on);
void GPIO_set_activate_inv_relays(bool on);
void GPIO_set_interlock_relay(bool on);

bool GPIO_get_TSMS();
bool GPIO_get_RTD();