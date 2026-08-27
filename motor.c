#include "motor.h"


// Definição dos pinos do L293D

#define MOTOR_IN1 LATDbits.LATD0
#define MOTOR_IN2 LATDbits.LATD1
#define MOTOR_ENABLE LATDbits.LATD2

// Ajustar de acordo com o tempo mecânico necessário para fechar a porta.
#define MOTOR_CLOSE_TIME_MS 2000



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

void motor_close_for_time(void)
{
    uint8_t i;

    motor_start();

    // __delay_ms aceita constantes; 20 x 100 ms mantém o código portátil no XC8.
    for(i = 0; i < (MOTOR_CLOSE_TIME_MS / 100); i++)
    {
        __delay_ms(100);
    }

    motor_stop();
}
