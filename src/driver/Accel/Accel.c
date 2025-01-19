#include "Accel.h"
#include "config.h"
#include "adc.h"
#include "usart.h"
#include "math.h"
#include "error_handler.h"

#define MAX(x, y) ((x) < (y) ? (y) : (x))

static float accelAVoltage;
static float accelBVoltage;
static float accelAPos;
static float accelBPos;
static float avgPos;

static int accel_A_irr_ms;      // How long accelerator pot A has been irrational for
static int accel_B_irr_ms;      // How long accelerator pot B has been irrational for
static int accel_disagree_ms;   // How long accelerator pots have disagreed for

uint16_t accelAVal;
uint16_t accelBVal;

static bool faulting;


#define UPDATE_MS 10
#define TIMEOUT_MS 100

static uint8_t errorList;

static void errorCheck();

void Accel_init()
{
    if (!core_ADC_init(ADC1)) error_handler();
    if (!core_ADC_init(ADC2)) error_handler();
    if (!core_ADC_setup_pin(ACCEL_A_PORT, ACCEL_A_PIN, 0)) error_handler();
    if (!core_ADC_setup_pin(ACCEL_B_PORT, ACCEL_B_PIN, 0)) error_handler();
    accelAPos = 0;
    accelBPos = 0;
    accelAVoltage = 0;
    accelBVoltage = 0;
    errorList = 0;
    accel_A_irr_ms = 0;
    accel_B_irr_ms = 0;
    faulting = false;
}

float get_raw_pos()
{
    uint16_t accelAVal;
    uint16_t accelBVal;
    core_ADC_read_channel(ACCEL_A_PORT, ACCEL_A_PIN, &accelAVal);
    core_ADC_read_channel(ACCEL_B_PORT, ACCEL_B_PIN, &accelBVal);

    accelAVoltage = ((float) accelAVal / ADC_MAX_VAL) * ADC_MAX_VOLTAGE;
    accelBVoltage = ((float) accelBVal / ADC_MAX_VAL) * ADC_MAX_VOLTAGE;

//    uprintf(USART3, "A: %d, B: %d\n", (int)(accelAVoltage * 1000), (int)(accelBVoltage * 1000));

    float aPos = (MAX(accelAVoltage - ACCEL_A_OFFSET_V, 0.0) / ACCEL_A_RANGE_V);
    float bPos = (MAX(accelBVoltage - ACCEL_B_OFFSET_V, 0.0) / ACCEL_B_RANGE_V);

//    uprintf(USART3, "A Pos: %d, B Pos: %d\n", (int)(aPos * 1000), (int)(bPos * 1000));

    return ((aPos + bPos) / 2);
}

bool Accel_update_task() {
    float pos = get_raw_pos();

    errorCheck();
    if (!errorList) avgPos = pos;
    // If accel potentiometer A is irrational add to its counter, if not reset it to 0
    accel_A_irr_ms = ((errorList & ACCEL_A_IRRATIONAL_ERROR) ? accel_A_irr_ms + UPDATE_MS : 0);

    // If accel potentiometer B is irrational add to its counter, if not reset it to 0
    accel_B_irr_ms = ((errorList & ACCEL_B_IRRATIONAL_ERROR) ? accel_B_irr_ms + UPDATE_MS : 0);

    // If pots disagree add to the counter, if not reset it to 0
    accel_disagree_ms = ((errorList & ACCEL_DISAGREEMENT_ERROR) ? accel_disagree_ms + UPDATE_MS : 0);

    if ((accel_A_irr_ms >= TIMEOUT_MS) ||
        (accel_B_irr_ms >= TIMEOUT_MS) ||
        (accel_disagree_ms >= TIMEOUT_MS))
        faulting = true;

//    uprintf(USART3, "Error: %d, A: %d, B: %d\n", errorList, (int)(accelAVoltage * 1000), (int)(accelBVoltage * 1000));




    return faulting;
}

static void errorCheck()
{
//    uprintf(USART3, "Accel A voltage: %d\n", (int)(accelAVoltage * 1000));
//    uprintf(USART3, "Accel B voltage: %d\n", (int)(accelBVoltage * 1000));
//    uprintf(USART3, "Accel A val: %d\n", accelAVal);
//    uprintf(USART3, "Accel B val: %d\n", accelBVal);

    errorList = 0;
    if ((accelAVoltage < ACCEL_A_IRRATIONAL_LOW_V) || (accelAVoltage > ACCEL_A_MAX_V))
    {
        errorList |= ACCEL_A_IRRATIONAL_ERROR;
    }

    if ((accelBVoltage < ACCEL_B_IRRATIONAL_LOW_V) || (accelBVoltage > ACCEL_B_MAX_V))
    {
        errorList |= ACCEL_B_IRRATIONAL_ERROR;
    }

    if (fabsf(accelAPos - accelBPos) * 100 > ACCEL_MAX_DISAGREEMENT)
    {
        errorList |= ACCEL_DISAGREEMENT_ERROR;
    }

    uprintf(USART3, "Error: %d, A: %d, B: %d, A irr ms: %d, B irr ms: %d\n", errorList, (int)(accelAVoltage * 1000), (int)(accelBVoltage * 1000), accel_A_irr_ms, accel_B_irr_ms);
}

float Accel_get_pos()
{
    return (faulting? 0 : avgPos);
}