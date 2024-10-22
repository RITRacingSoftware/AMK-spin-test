#include "Inverters.h"

Inverter_s invRR;
Inverter_s invRL;
Inverter_s invFR;
Inverter_s invFL;

Inverter_s *invArr[4] = {&invRR, &invRL, &invFR, &invFL};

void inv_init()
{
    for (int i = 0; i < 4; i++)
    {
        Inverter_s *invPtr = invArr[i];
        invPtr->isReady = false;
        invPtr->hvOnEcho= false;
        invPtr->hvOn = false;
        invPtr->isOnEcho = false;
        invPtr->isOn = false;
    }
}


// Getters and setters for inverter struct values
bool get_ready(uint8_t invNum) {return invArr[invNum]->isReady;}
void set_ready(uint8_t invNum, bool val) {invArr[invNum]->isReady = val;}

bool get_hv_on_echo(uint8_t invNum) {return invArr[invNum]->hvOnEcho;}
void set_hv_on_echo(uint8_t invNum, bool val) {invArr[invNum]->hvOnEcho = val;}

bool get_hv_on(uint8_t invNum) {return invArr[invNum]->hvOn;}
void set_hv_on(uint8_t invNum, bool val) {invArr[invNum]->hvOn = val;}

bool get_inv_on_echo(uint8_t invNum) {return invArr[invNum]->isOnEcho;}
void set_inv_on_echo(uint8_t invNum, bool val) {invArr[invNum]->isOnEcho = val;}

bool get_inv_on(uint8_t invNum) {return invArr[invNum]->isOn;}
void set_inv_on(uint8_t invNum, bool val) {invArr[invNum]->isOn = val;}