#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "pwm.h"
#include "debug.h"

#include <stdbool.h>

static uint32_t GetPortMask(PORT_Type *port);
static uint32_t GetTPMMask(TPM_Type *tpm);
static uint32_t MsToTicks(uint32_t ms);

bool PWM_Output_Init(pwm_config_t *config, PORT_Type *port, uint8_t pin, uint8_t mux) {

    port->PCR[pin] |= PORT_PCR_MUX(mux);
    SIM_SCGC5 |= GetPortMask(port);

    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
    SIM->SCGC6 |= GetTPMMask(config->tpm);

    // Divide clock frequency by 128 = 375Khz
    config->tpm->SC |= TPM_SC_PS(7); 

    //  mod = period * 375k
    config->tpm->MOD = MsToTicks(config->period) - 1;

    // Edge aligned mode
    config->tpm->CONTROLS[config->tpm_ch].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;

    // Output to 0
    config->tpm->CONTROLS[config->tpm_ch].CnV = 0;

    // Enable TPM and set center-aligned disabled
    config->tpm->SC |= TPM_SC_CPWMS(0) | TPM_SC_CMOD(1);

    return true;
}

void PWM_Write(pwm_config_t *config, double value) {
    uint32_t max_ticks = MsToTicks(config->duty_cycle_max);
    uint32_t min_ticks = MsToTicks(config->duty_cycle_min);

    uint32_t duty_cycle = (uint32_t)(value * (max_ticks - min_ticks)) + min_ticks;

    config->tpm->CONTROLS[config->tpm_ch].CnV = duty_cycle;
}

static uint32_t GetPortMask(PORT_Type *port) {
    switch ((uint32_t) port) {
        case PORTA_BASE:
            return SIM_SCGC5_PORTA_MASK;
        case PORTB_BASE:
            return SIM_SCGC5_PORTB_MASK;
        case PORTC_BASE:
            return SIM_SCGC5_PORTC_MASK;
        case PORTD_BASE:
            return SIM_SCGC5_PORTD_MASK;
        case PORTE_BASE:
            return SIM_SCGC5_PORTE_MASK;
        default:
            return 0;
    }
}

static uint32_t GetTPMMask(TPM_Type *tpm) {
    switch ((uint32_t)tpm) {
        case TPM0_BASE:
            return SIM_SCGC6_TPM0_MASK;
        case TPM1_BASE:
            return SIM_SCGC6_TPM1_MASK;
        case TPM2_BASE:
            return SIM_SCGC6_TPM2_MASK;
        default:
            return 0;
    }
}

static uint32_t MsToTicks(uint32_t ms) {
    return (uint32_t)((ms / 1000.0) * 375000);
}