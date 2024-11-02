#ifndef DRIVER_CAN_H
#define DRIVER_CAN_H

#include "config.h"

#include <stdbool.h>
#include <stdint.h>

#include "inverter_dbc.h"

typedef struct
{
    // Received by VC
    struct inverter_dbc_rr_amk_actual_1_t rr_actual1;
    struct inverter_dbc_rr_amk_actual_2_t rr_actual2;
    struct inverter_dbc_rr_amk_rit_set1_t rr_set1;
    struct inverter_dbc_rr_amk_rit_set2_t rr_set2;

    struct inverter_dbc_rl_amk_actual_1_t rl_actual1;
    struct inverter_dbc_rl_amk_actual_2_t rl_actual2;
    struct inverter_dbc_rl_amk_rit_set1_t rl_set1;
    struct inverter_dbc_rl_amk_rit_set2_t rl_set2;

    struct inverter_dbc_fr_amk_actual_1_t fr_actual1;
    struct inverter_dbc_fr_amk_actual_2_t fr_actual2;
    struct inverter_dbc_fr_amk_rit_set1_t fr_set1;
    struct inverter_dbc_fr_amk_rit_set2_t fr_set2;

    struct inverter_dbc_fl_amk_actual_1_t fl_actual1;
    struct inverter_dbc_fl_amk_actual_2_t fl_actual2;
    struct inverter_dbc_fl_amk_rit_set1_t fl_set1;
    struct inverter_dbc_fl_amk_rit_set2_t fl_set2;

    // Sent by VC
    struct inverter_dbc_rr_amk_setpoints_t rr_setpoints;
    struct inverter_dbc_rr_amk_setpoints2_t rr_setpoints2;
    struct inverter_dbc_rl_amk_setpoints_t rl_setpoints;
    struct inverter_dbc_rl_amk_setpoints2_t rl_setpoints2;
    struct inverter_dbc_fr_amk_setpoints_t fr_setpoints;
    struct inverter_dbc_fr_amk_setpoints2_t fr_setpoints2;
    struct inverter_dbc_fl_amk_setpoints_t fl_setpoints;
    struct inverter_dbc_fl_amk_setpoints2_t fl_setpoints2;
} CAN_BUS;

extern CAN_BUS canBus;

bool CAN_tx();
void CAN_rx();
int CAN_pack_message(int id, uint8_t *msg_data);
bool CAN_add_filters();
#endif