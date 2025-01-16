#pragma once

typedef enum
{
    VehicleState_VC_NOT_READY, // Doing nothing, default state
    VehicleState_INVERTERS_POWERED, // Reads for inverters saying they're ready
    VehicleState_PRECHARGING, // When precharge done button is pressed, send it out,
    // receive echo and confirmation, flip interlock relay
    VehicleState_WAIT, // Wait for enable button to be pressed
    VehicleState_STANDBY, // Send inverter enable and on, receive inverter on echos and confirmation, set BE2
    VehicleState_RUNNING, // Actively running, send torque request every 200ms. Wait for stop button
    VehicleState_SHUTDOWN // Turn off everything in sequence
} VehicleState_e;

void VehicleState_init();
void VehicleState_100Hz();