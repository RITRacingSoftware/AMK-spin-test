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
    bool hvOnEcho;
    bool hvOn;
    bool isOnEcho;
    bool isOn;
} Inverter_s;

void inv_init();

bool get_ready(uint8_t invNum);
void set_ready(uint8_t invNum, bool val);

bool get_hv_on_echo(uint8_t invNum);
void set_hv_on_echo(uint8_t invNum, bool val);

bool get_hv_on(uint8_t invNum);
void set_hv_on(uint8_t invNum, bool val);

bool get_inv_on_echo(uint8_t invNum);
void set_inv_on_echo(uint8_t invNum, bool val);

bool get_inv_on(uint8_t invNum);
void set_inv_on(uint8_t invNum, bool val);

bool check_all(bool (*f_ptr)(uint8_t));