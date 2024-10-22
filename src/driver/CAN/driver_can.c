#include "driver_can.h"
#include "can.h"

#define CAN FDCAN2

bool CAN_tx()
{
    core_CAN_send_from_tx_queue_task(CAN);
    return false;
}

void CAN_rx()
{
    CanMessage_s canMessage;

    if (core_CAN_receive_from_queue(CAN, &canMessage))
    {
        int id = canMessage.id;

        switch (id)
        {
            // RR
            case INVERTER_DBC_RR_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_rr_amk_actual_1_unpack(&canBus.rr_actual1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_RR_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_rr_amk_rit_set1_unpack(&canBus.rr_set1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_RR_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_rr_amk_rit_set2_unpack(&canBus.rr_set2, (uint8_t *)canMessage.data, 8); break;

            // RL
            case INVERTER_DBC_RL_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_rl_amk_actual_1_unpack(&canBus.rl_actual1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_RL_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_rl_amk_rit_set1_unpack(&canBus.rl_set1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_RL_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_rl_amk_rit_set2_unpack(&canBus.rl_set2, (uint8_t *)canMessage.data, 8); break;

            // FR
            case INVERTER_DBC_FR_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_fr_amk_actual_1_unpack(&canBus.fr_actual1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_FR_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_fr_amk_rit_set1_unpack(&canBus.fr_set1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_FR_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_fr_amk_rit_set2_unpack(&canBus.fr_set2, (uint8_t *)canMessage.data, 8); break;

            // FL
            case INVERTER_DBC_FL_AMK_ACTUAL_1_FRAME_ID:
                inverter_dbc_fl_amk_actual_1_unpack(&canBus.fl_actual1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_FL_AMK_RIT_SET1_FRAME_ID:
                inverter_dbc_fl_amk_rit_set1_unpack(&canBus.fl_set1, (uint8_t *)canMessage.data, 8); break;

            case INVERTER_DBC_FL_AMK_RIT_SET2_FRAME_ID:
                inverter_dbc_fl_amk_rit_set2_unpack(&canBus.fl_set2, (uint8_t *)canMessage.data, 8); break;
        }
    }
}
