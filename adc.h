#ifndef ADC_H
#define ADC_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 20000000


// Inicializa o conversor AD
void adc_init(void);


// Faz a leitura do LDR (AN0)
uint16_t adc_readLDR(void);


#endif