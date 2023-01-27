#include "lib/include.h"

#ifndef GPIO_H_
#define GPIO_H_
extern void Configurar_GPIO(void)
{
volatile unit32_t delay = SYSCTL->RCGCGPIO;
                   // B        E
SYSCTL->RCGCGPIO |= (1<<1) | (1<<4) ;
GPIOE -> DIR &=~0x03; //deshabilita el pin E como entrada 
GPIOE -> DEN |= 0x03;//habilta el pin E como digital
GPIOB -> DIR |= 0X3F; //habilita el pin B como salida 
GPIOB -> DEN |= 0X3F; //habilita el pin B como digital 
}