#include "alarme.h"


// Definição dos pinos

#define LED LATCbits.LATC0

#define BUZZER LATCbits.LATC1

#define LASER LATCbits.LATC2



void alarm_init(void)
{


    // RC0, RC1 e RC2 como saída

    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;



    // Estado inicial desligado

    LED = 0;
    BUZZER = 0;
    LASER = 0;


}



void alarm_arm(void)
{


    /*
       Sistema armado:

       LED ligado
       Laser ligado
       Buzzer desligado
    */

    LED = 1;

    LASER = 1;

    BUZZER = 0;


}



void alarm_trigger(void)
{


    /*
       Feixe interrompido:

       Mantém LED ligado
       Desliga laser
       Liga buzzer
    */


    LED = 1;

    LASER = 0;

    BUZZER = 1;


}



void alarm_disarm(void)
{


    /*
       Sistema desligado:

       Tudo OFF
    */


    LED = 0;

    LASER = 0;

    BUZZER = 0;


}