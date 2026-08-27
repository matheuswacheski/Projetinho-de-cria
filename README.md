# Projetinho-de-cria

Sistema de alarme comercial com PIC18F4550, laser, LDR, buzzer, motor com
L293D e teclado matricial.

## Pinagem mantida

- RA0/AN0: leitura analógica do LDR.
- RB0-RB2: colunas do teclado 4x3.
- RB3-RB6: linhas do teclado 4x3.
- RC0: LED; RC1: buzzer; RC2: laser.
- RD0: IN1 do L293D; RD1: IN2 do L293D; RD2: enable do L293D.

## Ajustes necessários em bancada

Em `main.c`, calibre `LDR_THRESHOLD_DISPARO` e `LDR_THRESHOLD_REARME` com
leituras reais do LDR: a leitura com o laser incidindo deve ficar acima do
limiar de rearme e a leitura com o feixe interrompido abaixo do limiar de
disparo. Em `motor.c`, ajuste `MOTOR_CLOSE_TIME_MS` ao tempo máximo seguro
para fechar a porta.

O motor é desligado automaticamente ao fim desse tempo. Para uma instalação
real, recomenda-se adicionar um fim de curso e detecção de obstáculo; isso
exige pinos ou hardware adicional e não faz parte da pinagem atual.
