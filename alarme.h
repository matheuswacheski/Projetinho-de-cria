#ifndef ALARME_H
#define ALARME_H

#include <xc.h>

#define _XTAL_FREQ 20000000


// Inicializa LED, buzzer e laser
void alarm_init(void);


// Arma o sistema
void alarm_arm(void);


// Dispara o alarme
void alarm_trigger(void);


// Desarma o sistema
void alarm_disarm(void);


#endif