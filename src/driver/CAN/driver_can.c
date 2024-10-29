#include "driver_can.h"
#include "can.h"
#include "Inverters/Inverters.h"
#include "inverter_dbc.h"
#include "VC/VC.h"


CAN_BUS canBus;

int idArr[NUM_IDS_FDCAN2] = {
        INVERTER_DBC_RR_AMK_ACTUAL_1_FRAME_ID,
        INVERTER_DBC_RR_AMK_RIT_SET1_FRAME_ID,
        INVERTER_DBC_RR_AMK_RIT_SET2_FRAME_ID,
        INVERTER_DBC_RL_AMK_ACTUAL_1_FRAME_ID,
        INVERTER_DBC_RL_AMK_RIT_SET1_FRAME_ID,
        INVERTER_DBC_RL_AMK_RIT_SET2_FRAME_ID,
        INVERTER_DBC_FR_AMK_ACTUAL_1_FRAME_ID,
        INVERTER_DBC_FR_AMK_RIT_SET1_FRAME_ID,
        INVERTER_DBC_FR_AMK_RIT_SET2_FRAME_ID,
        INVERTER_DBC_FL_AMK_ACTUAL_1_FRAME_ID,
        INVERTER_DBC_FL_AMK_RIT_SET1_FRAME_ID,
        INVERTER_DBC_FL_AMK_RIT_SET2_FRAME_ID
};

bool CAN_tx()
{
    core_CAN_send_from_tx_queue_task(FDCAN2);
    return false;
}

void CAN_rx()
{
    CanMessage_s canMessage;

    if (core_CAN_receive_from_queue(FDCAN2, &canMessage))
    {
        uint8_t data[8];
        for (int i = 0; i < 8; i++)
        {
            data[i] = (canMessage.data >> (i * 8)) & 0xff;
        }
        int id = canMessage.id;
//        toggle_heartbeat();

        switch (id)
        {
            // RR
            case INVERTER_DBC_RR_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_rr_amk_actual_1_unpack(&canBus.rr_actual1, &data[0], 8); break;

            case INVERTER_DBC_RR_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_rr_amk_rit_set1_unpack(&canBus.rr_set1, &data[0], 8); break;

            case INVERTER_DBC_RR_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_rr_amk_rit_set2_unpack(&canBus.rr_set2, &data[0], 8); break;

            // RL
            case INVERTER_DBC_RL_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_rl_amk_actual_1_unpack(&canBus.rl_actual1, &data[0], 8); break;

            case INVERTER_DBC_RL_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_rl_amk_rit_set1_unpack(&canBus.rl_set1, &data[0], 8); break;

            case INVERTER_DBC_RL_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_rl_amk_rit_set2_unpack(&canBus.rl_set2, &data[0], 8); break;

            // FR
            case INVERTER_DBC_FR_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_fr_amk_actual_1_unpack(&canBus.fr_actual1, &data[0], 8); break;

            case INVERTER_DBC_FR_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_fr_amk_rit_set1_unpack(&canBus.fr_set1, &data[0], 8); break;

            case INVERTER_DBC_FR_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_fr_amk_rit_set2_unpack(&canBus.fr_set2, &data[0], 8); break;

            // FL
            case INVERTER_DBC_FL_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_fl_amk_actual_1_unpack(&canBus.fl_actual1, &data[0], 8); break;

            case INVERTER_DBC_FL_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_fl_amk_rit_set1_unpack(&canBus.fl_set1, &data[0], 8); break;

            case INVERTER_DBC_FL_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_fl_amk_rit_set2_unpack(&canBus.fl_set2, &data[0], 8); break;
        }
        Inverters_update();

    }

}

int CAN_pack_message(int id, uint8_t *msg_data)
{
    switch (id)
    {
        case INVERTER_DBC_RR_AMK_SETPOINTS_FRAME_ID:
            return inverter_dbc_rr_amk_setpoints_pack(msg_data, &canBus.rr_setpoints, 8);

        case INVERTER_DBC_RL_AMK_SETPOINTS_FRAME_ID:
            return inverter_dbc_rl_amk_setpoints_pack(msg_data, &canBus.rl_setpoints, 8);

        case INVERTER_DBC_FR_AMK_SETPOINTS_FRAME_ID:
            return inverter_dbc_fr_amk_setpoints_pack(msg_data, &canBus.fr_setpoints, 8);

        case INVERTER_DBC_FL_AMK_SETPOINTS_FRAME_ID:
            return inverter_dbc_fl_amk_setpoints_pack(msg_data, &canBus.fl_setpoints, 8);

        default:
            break;
    }

    return -1;
}

bool CAN_add_filters()
{
    int minFilter = idArr[0];
    int maxFilter = idArr[0];

    for (int i = 1; i < NUM_IDS_FDCAN2; i++)
    {
        if (idArr[i] < minFilter) minFilter = idArr[i];
        if (idArr[i] > maxFilter) maxFilter = idArr[i];
    }
    return core_CAN_add_filter(FDCAN2, false, minFilter, maxFilter);
}