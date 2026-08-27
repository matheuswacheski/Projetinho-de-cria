#ifndef MOTOR_H
#define MOTOR_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000


// Inicializa o controle do motor
void motor_init(void);


// Liga o motor
void motor_start(void);


// Desliga o motor
void motor_stop(void);

// Fecha a porta pelo tempo máximo configurado e desliga o motor.
void motor_close_for_time(void);


#endif
