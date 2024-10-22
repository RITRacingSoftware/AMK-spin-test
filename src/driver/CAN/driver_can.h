#pragma once

#include <stdbool.h>
#include <stdint.h>

bool CAN_tx();
void CAN_rx();

void CAN_tx_hv_on(bool on);
void CAN_tx_inv_enable(bool on);
void CAN_tx_inv_on(bool on);
void CAN_tx_actual_speed_value(int rpm);
void CAN_tx_torque_request(int16_t setpoint, int16_t negLimit, int16_t posLimit);