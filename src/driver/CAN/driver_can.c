#include "driver_can.h"
#include "can.h"
#include "inverter_dbc.h"

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
//            case INVERTER_DBC_RL_AMK_ACTUAL_1_FRAME_ID:
//                break;
        }
    }

}