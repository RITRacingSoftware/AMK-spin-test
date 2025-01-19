#pragma once

#include "stdbool.h"

#define ACCEL_A_PORT GPIOA
#define ACCEL_A_PIN GPIO_PIN_3

#define ACCEL_B_PORT GPIOA
#define ACCEL_B_PIN GPIO_PIN_4

#define ACCEL_A_IRRATIONAL_ERROR 0x1    // 0b00000001
#define ACCEL_B_IRRATIONAL_ERROR 0x2    // 0b00000010
#define ACCEL_DISAGREEMENT_ERROR 0x4    // 0b00000100

void Accel_init();
bool Accel_update_task();
float get_raw_pos();
float Accel_get_pos();