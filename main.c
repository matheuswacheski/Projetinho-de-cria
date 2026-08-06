/*
========================================================
Projeto: Sistema de Alarme com Laser
PIC18F4550
MPLAB X + XC8
Arquivo: main.c
Clock: 20MHz
========================================================
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "keypad.h"
#include "adc.h"
#include "motor.h"
#include "alarme.h"

#define _XTAL_FREQ 20000000


//================ CONFIG BITS =================

#pragma config PLLDIV = 5
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2

#pragma config FOSC = HS
#pragma config FCMEN = OFF
#pragma config IESO = OFF

#pragma config PWRT = ON
#pragma config BOR = OFF

#pragma config WDT = OFF

#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = ON


//================================================

#define LDR_THRESHOLD 300


// Estados do sistema

typedef enum
{
    DESLIGADO = 0,
    ARMADO,
    ALARME

}EstadoSistema;


EstadoSistema estado = DESLIGADO;


// Senha

char senhaCorreta[] = "12345";
char senhaDigitada[6];


// Protótipo

void configurarPIC(void);


//================================================
// PROGRAMA PRINCIPAL
//================================================

void main(void)
{

    configurarPIC();


    // Inicialização dos módulos

    keypad_init();

    adc_init();

    motor_init();

    alarm_init();



    while(1)
    {


        switch(estado)
        {


            //====================================
            // SISTEMA DESLIGADO
            //====================================

            case DESLIGADO:


                if(keypad_getPassword(senhaDigitada))
                {

                    if(strcmp(senhaDigitada, senhaCorreta) == 0)
                    {

                        // Liga LED e laser

                        alarm_arm();


                        estado = ARMADO;

                    }

                }


            break;



            //====================================
            // SISTEMA ARMADO
            //====================================

            case ARMADO:


                /*
                 Quando o laser deixa de chegar no LDR,
                 o alarme dispara.
                */

                if(adc_readLDR() < LDR_THRESHOLD)
                {


                    // Desliga laser e liga buzzer

                    alarm_trigger();



                    // Liga motor pelo L293D

                    motor_start();



                    estado = ALARME;

                }


            break;



            //====================================
            // SISTEMA EM ALARME
            //====================================

            case ALARME:


                if(keypad_getPassword(senhaDigitada))
                {

                    if(strcmp(senhaDigitada, senhaCorreta) == 0)
                    {


                        // Desliga LED, laser e buzzer

                        alarm_disarm();



                        // Desliga motor

                        motor_stop();



                        // Volta ao início

                        estado = DESLIGADO;


                    }

                }


            break;



        }


    }


}


//================================================
// CONFIGURAÇÃO DO PIC
//================================================

void configurarPIC(void)
{


    // Desliga comparadores

    CMCON = 0x07;



    /*
      Configuração analógica:
      RA0 = AN0 (LDR)
      demais pinos digitais
    */

    ADCON1 = 0x0E;



    // PORTA

    TRISA = 0xFF;

    LATA = 0x00;



    /*
      PORTB:
      RB0-RB2 = colunas teclado
      RB3-RB6 = linhas teclado

      Configurado no keypad_init()
    */

    LATB = 0x00;



    /*
      PORTC:

      RC0 = LED
      RC1 = BUZZER
      RC2 = LASER
    */

    TRISC = 0x00;

    LATC = 0x00;



    /*
      PORTD:

      RD0 = IN1 L293D
      RD1 = IN2 L293D
      RD2 = ENABLE L293D
    */

    TRISD = 0x00;

    LATD = 0x00;



    // PORTE

    TRISE = 0x00;

    LATE = 0x00;


}