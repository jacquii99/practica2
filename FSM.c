#include "lib/include.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

struct estado {
  uint32_t Out;
  uint32_t Time; 
  uint32_t Next[4];};

volatile unsigned long LED;
int main (void)
{
  Configurar_PLL(); //configuracion de velocidad de reloj
  Configurar_GPIO();
  while(1)
  {

    LED = 0X01;
    GPION -> DATA=LED;
    Delay();
    LED = 0x02; 
    GPION -> DATA=LED;
    Delay ();
    LED = 0X04;
    GPION -> DATA=LED;
    Delay();
    LED = 0x08; 
    GPION -> DATA=LED;
    Delay ();

  }
}


typedef const struct estado State_t;
#define N   0
#define EN 1
#define E   2
#define EE 3
uint32_t S;  
uint32_t Input;

State_t FSM[4]=
{
 {0x21,3000,{N,EN,N,EN}},
 {0x22, 500,{E,E,E,E}},
 {0x0C,3000,{E,E,EE,EE}},
 {0x14, 500,{N,N,N,N}}
 };