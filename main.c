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
#include "driverlib/uart.h"

/**
 * main.c
 */

uint32_t timeBase;
volatile uint32_t caracterRecibido;
uint32_t ledR;
uint32_t ledG;
uint32_t ledB;

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
    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , 2);
    // habilitar el uart0, los GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // PARTE 5
    //hacer los pines que sean controlados por uart
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1 );
    // configurado a 115200 bauds sin paridad un bit de parada y largo de 8 bits
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 |  UART_CONFIG_STOP_ONE| UART_CONFIG_PAR_NONE );
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTEnable(UART0_BASE);
    // parte 6, recepcion de la interrupcion del uart
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE,UART_INT_RX);

    IntMasterEnable();
    while(1){
       //UARTCharPutNonBlocking(UART0_BASE, 'd');
        if(caracterRecibido == 'r'){
            ledR = ~ledR & GPIO_PIN_1;
            caracterRecibido = 0;
        }else if(caracterRecibido == 'g'){
            ledG = ~ledG & GPIO_PIN_3;
            caracterRecibido = 0;
        }
        else if(caracterRecibido == 'b'){
            ledB = ~ledB & GPIO_PIN_2;
            caracterRecibido = 0;
        }
    }

    return 0;
}
// ------------    MANEJADORES DE INTERRUPCIONES ----------------------------------------------
 // TIMER 0
void togglePin(void){
        TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);

        GPIOPinWrite(GPIO_PORTF_BASE, ledR | ledB | ledG , ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3));
}
// UART 0
void Uart0IntHanddler(void){
    UARTIntClear(UART0_BASE, UART_INT_RX);
    caracterRecibido = UARTCharGetNonBlocking(UART0_BASE);


}


