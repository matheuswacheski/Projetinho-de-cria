/*
========================================================
Projeto: Sistema de Alarme com Laser
PIC18F4550
MPLAB X + XC8
Arquivo: main.c
Clock: 4MHz
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

#define _XTAL_FREQ 4000000


//================ CONFIG BITS =================

#pragma config PLLDIV = 1
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

#define LDR_THRESHOLD_DISPARO 300
#define LDR_THRESHOLD_REARME  340
#define LDR_AMOSTRAS_DISPARO  8
#define TEMPO_ESTABILIZACAO_MS 2000


// Estados do sistema

typedef enum
{
    DESLIGADO = 0,
    ARMANDO,
    ARMADO,
    ALARME,
    FALHA_SENSOR

}EstadoSistema;


EstadoSistema estado = DESLIGADO;
static uint8_t leiturasBaixas = 0;


// Senha

const char senhaCorreta[] = "12345";
char senhaDigitada[6];


// Protótipo

void configurarPIC(void);
bool feixe_interrompido(void);
void feixe_resetar(void);
bool ldr_recebe_laser(void);
void aguardar_estabilizacao(void);


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

                        // Liga o laser e aguarda o sensor estabilizar.

                        alarm_arm();

                        feixe_resetar();


                        estado = ARMANDO;

                    }

                }


            break;


            //====================================
            // ESTABILIZACAO DO FEIXE
            //====================================

            case ARMANDO:

                aguardar_estabilizacao();

                // Não arma se o LDR não estiver recebendo o laser.
                if(ldr_recebe_laser())
                {
                    estado = ARMADO;
                }
                else
                {
                    // Falha de alinhamento, laser ou sensor: não move a porta.
                    alarm_trigger();
                    motor_stop();
                    estado = FALHA_SENSOR;
                }

            break;



            //====================================
            // SISTEMA ARMADO
            //====================================

            case ARMADO:


                // A senha também permite desarmar preventivamente.
                if(keypad_getPassword(senhaDigitada))
                {
                    if(strcmp(senhaDigitada, senhaCorreta) == 0)
                    {
                        alarm_disarm();
                        estado = DESLIGADO;
                        break;
                    }
                }


                /*
                 Quando o laser deixa de chegar no LDR,
                 o alarme dispara.
                */

                if(feixe_interrompido())
                {


                    // Desliga laser e liga buzzer

                    alarm_trigger();



                    // Fecha a porta pelo tempo máximo configurado no motor.c.

                    motor_close_for_time();



                    estado = ALARME;

                }


            break;


            //====================================
            // FALHA DE SENSOR / FEIXE DESALINHADO
            //====================================

            case FALHA_SENSOR:

                // A senha reconhecida silencia a falha e retorna ao estado seguro.
                if(keypad_getPassword(senhaDigitada))
                {
                    if(strcmp(senhaDigitada, senhaCorreta) == 0)
                    {
                        alarm_disarm();
                        motor_stop();
                        estado = DESLIGADO;
                    }
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

/*
 * Exige leituras baixas consecutivas antes de disparar. Isso reduz falsos
 * alarmes por ruído elétrico, cintilação ou sombra passageira no LDR.
 */
bool feixe_interrompido(void)
{
    uint16_t leitura = adc_readLDR();

    if(leitura < LDR_THRESHOLD_DISPARO)
    {
        if(leiturasBaixas < LDR_AMOSTRAS_DISPARO)
        {
            leiturasBaixas++;
        }
    }
    else if(leitura > LDR_THRESHOLD_REARME)
    {
        leiturasBaixas = 0;
    }

    return (leiturasBaixas >= LDR_AMOSTRAS_DISPARO);
}

void feixe_resetar(void)
{
    leiturasBaixas = 0;
}

bool ldr_recebe_laser(void)
{
    uint8_t i;
    uint32_t soma = 0;

    for(i = 0; i < 8; i++)
    {
        soma += adc_readLDR();
        __delay_ms(5);
    }

    return ((soma / 8) > LDR_THRESHOLD_REARME);
}

void aguardar_estabilizacao(void)
{
    uint8_t i;

    for(i = 0; i < (TEMPO_ESTABILIZACAO_MS / 100); i++)
    {
        __delay_ms(100);
    }
}
