#ifndef MOTOR_H
#define MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 20000000


// Inicializa o controle do motor
void motor_init(void);


// Liga o motor
void motor_start(void);


// Desliga o motor
void motor_stop(void);


#endif