#include "lcd.h"

//extern void sysTickIntHandler(void);
extern void enable_interrupt(void);

// GPIO Port initialize:
void init_peripherals(void)
{

    // Clock Gates enable
    IntMasterDisable();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);     // Clock Gate enable GPIO/D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);     // Clock Gate enable GPIO/L
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);     // Clock Gate enable GPIO/M
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);    // Clock Gate enable TIMER0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);    // Clock Gate enable TIMER1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);    // Clock Gate enable TIMER2
    SysCtlDelay(10);

    // Port K: V-sync signal input(0), motor direction S1 input(1), motor direction S2 input(2)
//    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2); // PortK(0-2) input
//    // debug output (not used in release version)
//    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_3 | GPIO_PIN_4); // PortK(3) output

    // Port D: V-sync signal input(0), motor direction S1 input(1), motor direction S2 input(2)
    GPIOPinTypeGPIOInput(GPIO_PORTD_AHB_BASE,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2); // PortK(0-2) input
    // debug output (not used in release version)
    GPIOPinTypeGPIOOutput(GPIO_PORTD_AHB_BASE, GPIO_PIN_3 | GPIO_PIN_4); // PortK(3) output

    // Port L: LCD Controls output
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, 0x1F);       // PortL(0)-(4) output

    // Port M: LCD Data output
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, 0xFF);       // PortM(0)-(7) output
}
