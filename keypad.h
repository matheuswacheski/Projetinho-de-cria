#ifndef KEYPAD_H
#define KEYPAD_H

#include <xc.h>
#include <stdbool.h>

#define _XTAL_FREQ 4000000


void keypad_init(void);

char keypad_getKey(void);

bool keypad_getPassword(char *senha);


#endif
