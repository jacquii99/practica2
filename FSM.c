#include "lib/include.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#define sensor(*((volatile unit32_t*)0x4002400C))
#define luz (*((volatile unit32_t*)0x400050FC))


struct estado {
  uint32_t Out;
  uint32_t Time; 
  uint32_t Next[4];};
typedef const struct estado State_t;
#define N   0
#define EN 1
#define E   2
#define EE 3
State_t FSM[4]=
{
 {0x21,3000,{N,EN,N,EN}},
 {0x22, 500,{E,E,E,E}},
 {0x0C,3000,{E,E,EE,EE}},
 {0x14, 500,{N,N,N,N}}
 };

uint32_t S;  //para el estado actual
uint32_t Input; //entrada 

int main (void){
Configurar_PLL(); //configuracion de velocidad de reloj
Configurar_GPIO();
SysTick_Init();
  
  S=  N; 
  while(1)
  {
    luz=FSM[S].Out;
    Input=sensor; 
    S= FSM[S].Next[Input]; 
    SysTick_Wait10ms(FSM[S].Time)
  }
}


