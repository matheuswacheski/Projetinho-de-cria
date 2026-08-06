#include "adc.h"


void adc_init(void)
{

    // RA0 como entrada

    TRISAbits.TRISA0 = 1;


    /*
       Configuração dos pinos analógicos

       AN0 = analógico
       Demais pinos = digitais
    */

    ADCON1 = 0x0E;



    /*
       ADCON2:

       ADFM = 1
       Resultado justificado à direita

       ACQT = 8 TAD

       ADCS = Fosc/32
    */

    ADCON2 = 0b10101010;



    /*
       ADCON0:

       Canal AN0 selecionado

       ADON = 1
       Liga o ADC
    */

    ADCON0 = 0b00000001;


}



uint16_t adc_readLDR(void)
{


    // Seleciona canal AN0

    ADCON0bits.CHS = 0;



    // Tempo de aquisição

    __delay_us(20);



    // Inicia conversão

    ADCON0bits.GO = 1;



    // Espera terminar

    while(ADCON0bits.GO);



    /*
       Junta os dois registradores:

       ADRESH = 8 bits superiores
       ADRESL = 2 bits inferiores
    */

    return ((ADRESH << 8) + ADRESL);


}