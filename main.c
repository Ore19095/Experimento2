#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

/**
 * main.c
 */

uint32_t timeBase;


 int main(void){
    // reloj del sistema  a 40MHZ
    SysCtlClockSet(
            SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                    | SYSCTL_OSC_MAIN);

    //puerto F activado
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //configuracion de puerto F 1,2,3 como salida
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
                            GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    // Configuracion del timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // configurando el timer 0 de 32 bits
   // timeBase = SysCtlClockGet() / 2; // medio segundo (para tener un perido de 1s o frec de 1Hz
    timeBase = SysCtlClockGet(); // un segundo
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, timeBase - 1);
    // se habilita la interrupcion tel timer0 a
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER0_BASE,TIMER_A );
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , 2);

    while(1){

    }

    return 0;
}

void togglePin(void){
        TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
}




