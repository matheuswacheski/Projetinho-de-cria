#include "keypad.h"


// Colunas (saídas)

#define COL1 LATBbits.LATB0
#define COL2 LATBbits.LATB1
#define COL3 LATBbits.LATB2


// Linhas (entradas)

#define LIN1 PORTBbits.RB3
#define LIN2 PORTBbits.RB4
#define LIN3 PORTBbits.RB5
#define LIN4 PORTBbits.RB6



const char teclado[4][3] =
{
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};



void keypad_init(void)
{

    // Colunas como saída

    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;


    // Linhas como entrada

    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;


    // Ativa pull-up interno da PORTB

    INTCON2bits.RBPU = 0;


    // Colunas em nível alto

    COL1 = 1;
    COL2 = 1;
    COL3 = 1;

}



static char keypad_scan(void)
{

    unsigned char coluna;


    for(coluna = 0; coluna < 3; coluna++)
    {


        // Todas as colunas desligadas

        COL1 = 1;
        COL2 = 1;
        COL3 = 1;



        // Ativa uma coluna

        if(coluna == 0)
            COL1 = 0;


        if(coluna == 1)
            COL2 = 0;


        if(coluna == 2)
            COL3 = 0;



        __delay_ms(5);



        // Verifica linhas

        if(LIN1 == 0)
        {
            return teclado[0][coluna];
        }


        if(LIN2 == 0)
        {
            return teclado[1][coluna];
        }


        if(LIN3 == 0)
        {
            return teclado[2][coluna];
        }


        if(LIN4 == 0)
        {
            return teclado[3][coluna];
        }


    }


    return 0;

}

char keypad_getKey(void)
{
    static char teclaAnterior = 0;
    char tecla = keypad_scan();

    // Uma tecla só é entregue uma vez, até que seja solta.
    if(tecla == 0)
    {
        teclaAnterior = 0;
        return 0;
    }

    if(tecla == teclaAnterior)
    {
        return 0;
    }

    // Debounce curto; não há mais espera indefinida pela soltura da tecla.
    __delay_ms(20);

    if(keypad_scan() != tecla)
    {
        return 0;
    }

    teclaAnterior = tecla;
    return tecla;
}




bool keypad_getPassword(char *senha)
{

    static unsigned char posicao = 0;


    char tecla;


    tecla = keypad_getKey();



    if(tecla != 0)
    {


        // Aceita apenas números

        if(tecla >= '0' && tecla <= '9')
        {

            senha[posicao] = tecla;

            posicao++;


            if(posicao == 5)
            {

                senha[5] = '\0';

                posicao = 0;


                return true;

            }

        }


        // Tecla * apaga a senha

        if(tecla == '*')
        {

            posicao = 0;
            senha[0] = '\0';

        }


    }


    return false;

}
