#include "motor.h"


// Definição dos pinos do L293D

#define MOTOR_IN1 LATDbits.LATD0
#define MOTOR_IN2 LATDbits.LATD1
#define MOTOR_ENABLE LATDbits.LATD2



void motor_init(void)
{


    // RD0, RD1 e RD2 como saída

    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;



    // Motor desligado inicialmente

    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    MOTOR_ENABLE = 0;


}



void motor_start(void)
{

    /*
       Habilita a ponte H

       EN1 = 1
    */

    MOTOR_ENABLE = 1;



    /*
       Define sentido de rotação

       IN1 = 1
       IN2 = 0
    */

    MOTOR_IN1 = 1;
    MOTOR_IN2 = 0;


}



void motor_stop(void)
{

    /*
       Desabilita a ponte H

       EN1 = 0
    */

    MOTOR_ENABLE = 0;



    // Remove os sinais de controle

    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;


}