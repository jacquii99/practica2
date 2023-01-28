#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
 
#include "TM4C1294NCPDT.h"
 
// fin  = fxtal / [(Q+1) * (N+1)]
//      = 25MHz / [(0+1) * (4+1)] = 5MHz
// fvoc = fin * MDIV
//      = 5MHz * 96.00 = 480MHz
// fsyclk = fvco / (PSYSDIV + 1)
//        = 480 / (3 + 1 ) = 120MHz 80M

//TM4C1294xl con un cristal de 16 MHz para que funcione a 80MHz 
//fin= 16MHz / [(0+1)*(4+1)]=3.2MHz
//fvoc=3.2Mhz * 150 =480Mhz
//fsyclk=480/(5+1)=80MHz 

#define MINT    150
#define MFRAC   0
#define N       4
#define Q       0
#define PSYSDIV 5
 
#define FBCHT   0x6
#define EBCHT   0x6
#define FBCE    0
#define EBCE    0
#define FWS     0x5
#define EWS     0x5
#define MB1     0x00100010
 
void SetSystemClock_80MHz()
{
    // 1. Configurar el rango del oscilador MOSC al rango de alta frecuencia
    SYSCTL->MOSCCTL |= 1 << 4;          // cristal conectado a 10MHZ
    SYSCTL->MOSCCTL &= ~(1 << 2);       // habilita eloscilador externo 
    SYSCTL->MOSCCTL &= ~(1 << 3);       // encender MOSC 
     
    // 2. Esperar a que eloscilador este listo, consultando el registro SYSctl_RIS 
    while(!(SYSCTL->RIS & (1 << 8))){}; // wait for oscillator ready
 
    // 3. Configurar la fuente de reloj PLL en MOSC, configurando  SYSCTL_RSCLKCFG en 0x03 (MOSC)
    SYSCTL->RSCLKCFG &= ~(0xFF << 20);  // clear oscillator and PLL source field
    SYSCTL->RSCLKCFG |= (0x3 << 20);    // set MOSC as oscillator source
    SYSCTL->RSCLKCFG |= (0x3 << 24);    // set MOSC as PLL input source
 
    // 4. Configure los valores enteros y fraccionarios de PLL en SYSCTL_PLLFREQ0 (MINT) Y PLLFREQ0(MFRAC)
    SYSCTL->PLLFREQ0 = (MFRAC << 10) | MINT;    // set PLL M integer and fractional values
    SYSCTL->PLLFREQ1 = (Q << 8)| N;            // set PLL N and Q
    SYSCTL->PLLFREQ0 |= 1 << 23;                // power up the PLL
         
    // 5. Configure el parametro de temporizacion de la memoria a traves del registro SYSCTL->MEMTIM0
    // SYSCTL->MEMTIM0 = (EBCHT << 22) | (EBCE << 21) | (EWS << 16) | (FBCHT << 6) | (FBCE << 5) | FWS;      // set memory timing parameters   
    SYSCTL->MEMTIM0 = (EBCHT << 22) | (EBCE << 21) | (EWS << 16) | (FBCHT << 6) | (FBCE << 5) | FWS; // set memory timing parameters
    SYSCTL->MEMTIM0 |= (1 << 4) | (1 << 20);
         
    // 6. Esperar a que PLL se bloquee sodeando el registro SYSCTL_PLLSTAT
    while( (SYSCTL->PLLSTAT & 0x01) != 0x01) {};
 
    // 7. Habilitar la actualiacion de MEMTIM, aceptar el nuevo  PLLFREQ y USEPLL
    SYSCTL->RSCLKCFG |= (1 << 31) | (1 << 30) | (1 << 28) | PSYSDIV;         // set PLL system clock divisor, lock the system clock to the PLL, and force a memory timing register update
}
 
int main()
{
    SetSystemClock_80MHz();
    // Place your application code here
     
}