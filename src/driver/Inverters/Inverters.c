#include "Inverters.h"
#include "config.h"

#include "inverter_dbc.h"
#include "CAN/driver_can.h"

static Inverter_s invRR;
static Inverter_s invRL;
static Inverter_s invFR;
static Inverter_s invFL;

static Inverter_s *invArr[4] = {&invRR, &invRL, &invFR, &invFL};

static int last_checkin_ms;

void inv_init()
{
    for (int i = 0; i < 4; i++)
    {
        Inverter_s *invPtr = invArr[i];
        invPtr->isReady = false;
        invPtr->dcOnEcho= false;
        invPtr->dcOn = false;
        invPtr->isOnEcho = false;
        invPtr->isOn = false;
    }
    last_checkin_ms = 0;
}

void Inverters_100Hz()
{
    if (last_checkin_ms < INV_CAN_TIMEOUT_MS)
    {
        // Update RR values
        invRR.isReady = (bool) inverter_dbc_rr_amk_actual_1_rr_status_system_ready_decode(canBus.rr_actual1.rr_status_system_ready);
        invRR.dcOnEcho = (bool) inverter_dbc_rr_amk_actual_1_rr_status_dc_on_decode(canBus.rr_actual1.rr_status_dc_on);
        invRR.dcOn = (bool) inverter_dbc_rr_amk_actual_1_rr_status_quit_dc_on_decode(canBus.rr_actual1.rr_status_quit_dc_on);
        invRR.isOnEcho = (bool) inverter_dbc_rr_amk_actual_1_rr_status_inverter_on_decode(canBus.rr_actual1.rr_status_inverter_on);
        invRR.isOn = (bool) inverter_dbc_rr_amk_actual_1_rr_status_quit_inverter_on_decode(canBus.rr_actual1.rr_status_quit_inverter_on);

        // Update RL values
        invRL.isReady = (bool) inverter_dbc_rl_amk_actual_1_rl_status_system_ready_decode(canBus.rl_actual1.rl_status_system_ready);
        invRL.dcOnEcho = (bool) inverter_dbc_rl_amk_actual_1_rl_status_dc_on_decode(canBus.rl_actual1.rl_status_dc_on);
        invRL.dcOn = (bool) inverter_dbc_rl_amk_actual_1_rl_status_quit_dc_on_decode(canBus.rl_actual1.rl_status_quit_dc_on);
        invRL.isOnEcho = (bool) inverter_dbc_rl_amk_actual_1_rl_status_inverter_on_decode(canBus.rl_actual1.rl_status_inverter_on);
        invRL.isOn = (bool) inverter_dbc_rl_amk_actual_1_rl_status_quit_inverter_on_decode(canBus.rl_actual1.rl_status_quit_inverter_on);

        // Update FR values
        invFR.isReady = (bool) inverter_dbc_fr_amk_actual_1_fr_status_system_ready_decode(canBus.fr_actual1.fr_status_system_ready);
        invFR.dcOnEcho = (bool) inverter_dbc_fr_amk_actual_1_fr_status_dc_on_decode(canBus.fr_actual1.fr_status_dc_on);
        invFR.dcOn = (bool) inverter_dbc_fr_amk_actual_1_fr_status_quit_dc_on_decode(canBus.fr_actual1.fr_status_quit_dc_on);
        invFR.isOnEcho = (bool) inverter_dbc_fr_amk_actual_1_fr_status_inverter_on_decode(canBus.fr_actual1.fr_status_inverter_on);
        invFR.isOn = (bool) inverter_dbc_fr_amk_actual_1_fr_status_quit_inverter_on_decode(canBus.fr_actual1.fr_status_quit_inverter_on);

        // Update FL values
        invFR.isReady = (bool) inverter_dbc_fl_amk_actual_1_fl_status_system_ready_decode(canBus.fl_actual1.fl_status_system_ready);
        invFR.dcOnEcho = (bool) inverter_dbc_fl_amk_actual_1_fl_status_dc_on_decode(canBus.fl_actual1.fl_status_dc_on);
        invFR.dcOn = (bool) inverter_dbc_fl_amk_actual_1_fl_status_quit_dc_on_decode(canBus.fl_actual1.fl_status_quit_dc_on);
        invFR.isOnEcho = (bool) inverter_dbc_fl_amk_actual_1_fl_status_inverter_on_decode(canBus.fl_actual1.fl_status_inverter_on);
        invFR.isOn = (bool) inverter_dbc_fl_amk_actual_1_fl_status_quit_inverter_on_decode(canBus.fl_actual1.fl_status_quit_inverter_on);
    }
}

bool Inverters_get_ready(uint8_t invNum) {return invArr[invNum]->isReady;}
bool Inverters_get_hv_on_echo(uint8_t invNum) {return invArr[invNum]->dcOnEcho;}
bool Inverters_get_hv_on(uint8_t invNum) {return invArr[invNum]->dcOn;}
bool Inverters_get_inv_on_echo(uint8_t invNum) {return invArr[invNum]->isOnEcho;}
bool Inverters_get_inv_on(uint8_t invNum) {return invArr[invNum]->isOn;}

void Inverters_set_dc_on(bool val)
{
    inverter_dbc_rr_amk_setpoints_rr_amk_b_dc_on_encode(val);
    inverter_dbc_rl_amk_setpoints_rl_amk_b_dc_on_encode(val);
    inverter_dbc_fr_amk_setpoints_fr_amk_b_dc_on_encode(val);
    inverter_dbc_fl_amk_setpoints_fl_amk_b_dc_on_encode(val);
}
void Inverters_set_enable(bool val)
{
    inverter_dbc_rr_amk_setpoints_rr_amk_b_enable_encode(val);
    inverter_dbc_rl_amk_setpoints_rl_amk_b_enable_encode(val);
    inverter_dbc_fr_amk_setpoints_fr_amk_b_enable_encode(val);
    inverter_dbc_fl_amk_setpoints_fl_amk_b_enable_encode(val);
}
void Inverters_set_inv_on(bool val)
{
    inverter_dbc_rr_amk_setpoints_rr_amk_b_inverter_on_encode(val);
    inverter_dbc_rl_amk_setpoints_rl_amk_b_inverter_on_encode(val);
    inverter_dbc_fr_amk_setpoints_fr_amk_b_inverter_on_encode(val);
    inverter_dbc_fl_amk_setpoints_fl_amk_b_inverter_on_encode(val);
}

void Inverters_set_torque_request(int16_t setpoint, int16_t negLimit, int16_t posLimit)
{
    inverter_dbc_rr_amk_setpoints_rr_amk_torque_setpoint_encode(setpoint);
    inverter_dbc_rr_amk_setpoints_rr_amk_torque_limit_negative_encode(negLimit);
    inverter_dbc_rr_amk_setpoints_rr_amk_torque_limit_positive_encode(posLimit);

    inverter_dbc_rl_amk_setpoints_rl_amk_torque_setpoint_encode(setpoint);
    inverter_dbc_rl_amk_setpoints_rl_amk_torque_limit_negative_encode(negLimit);
    inverter_dbc_rl_amk_setpoints_rl_amk_torque_limit_positive_encode(posLimit);

    inverter_dbc_fr_amk_setpoints_fr_amk_torque_setpoint_encode(setpoint);
    inverter_dbc_fr_amk_setpoints_fr_amk_torque_limit_negative_encode(negLimit);
    inverter_dbc_fr_amk_setpoints_fr_amk_torque_limit_positive_encode(posLimit);

    inverter_dbc_fl_amk_setpoints_fl_amk_torque_setpoint_encode(setpoint);
    inverter_dbc_fl_amk_setpoints_fl_amk_torque_limit_negative_encode(negLimit);
    inverter_dbc_fl_amk_setpoints_fl_amk_torque_limit_positive_encode(posLimit);
}