#pragma once

#include <stdbool.h>
#include <stdint.h>

#define INV_RR 0
#define INV_RL 1
#define INV_FR 2
#define INV_FL 3

typedef struct
{
    bool isReady;
    bool dcOnEcho;
    bool dcOn;
    bool isOnEcho;
    bool isOn;
} Inverter_s;

void Inverters_init();
void Inverters_100Hz();
void Inverters_update();

// Getters for inverter struct values
bool Inverters_get_ready(uint8_t invNum);
bool Inverters_get_dc_on_echo(uint8_t invNum);
bool Inverters_get_dc_on(uint8_t invNum);
bool Inverters_get_inv_on_echo(uint8_t invNum);
bool Inverters_get_inv_on(uint8_t invNum);

// Setters for DBC struct values
void Inverters_set_dc_on(bool val);
void Inverters_set_enable(bool val);
void Inverters_set_inv_on(bool val);
void Inverters_set_torque_request(uint8_t invNum, double setpoint, double negLimit, double posLimit);

void Inverters_send_setpoints(uint8_t invNum);