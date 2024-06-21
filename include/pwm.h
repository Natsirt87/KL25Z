#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include <stdbool.h>

#ifndef PWM_H
#define PWM_H

typedef struct {
    TPM_Type* tpm;
    uint8_t tpm_ch;
    uint32_t period; // In ms
    uint32_t duty_cycle_min; // In ms
    uint32_t duty_cycle_max; // In ms
} pwm_config_t;

bool PWM_Output_Init(pwm_config_t *config, PORT_Type *port, uint8_t pin, uint8_t mux);
void PWM_Write(pwm_config_t *config, double value);

#endif