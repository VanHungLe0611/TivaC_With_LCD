#include "lcd.h"

//extern void sysTickIntHandler(void);
extern void portD1IntHandler(void);
extern void Timer1IntHandler(void);
extern void Timer2IntHandler(void);
extern uint32_t sysClk;

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
    GPIOPinTypeGPIOInput(GPIO_PORTD_AHB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2); // PortK(0-2) input
    // debug output (not used in release version)
    GPIOPinTypeGPIOOutput(GPIO_PORTD_AHB_BASE, GPIO_PIN_3 | GPIO_PIN_4); // PortK(3) output

    // Port L: LCD Controls output
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, 0x1F);       // PortL(0)-(4) output

    // Port M: LCD Data output
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, 0xFF);       // PortM(0)-(7) output


    // Interrupts
    // Configure PortD(1) rising edge Interrupt
    GPIOIntRegister(GPIO_PORTD_AHB_BASE, portD1IntHandler);
    GPIOIntTypeSet(GPIO_PORTD_AHB_BASE, GPIO_PIN_1, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTD_AHB_BASE, GPIO_INT_PIN_1);
    IntPrioritySet(INT_GPIOD, HIGH_PRIORITY);

    // Configure Timer0 Interrupt
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerEnable(TIMER0_BASE, TIMER_A);

    // Configure Timer1 Interrupt
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, sysClk / 50);      // fires every 500 ms
    TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1IntHandler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);

    // Configure Timer2 Interrupt
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);  // set clock for timer 2
    TimerLoadSet(TIMER2_BASE, TIMER_A, sysClk / 20);      // fires every 50 ms
    TimerIntRegister(TIMER2_BASE, TIMER_A, Timer2IntHandler); // set interrupt vector for timer 2A
    IntPrioritySet(INT_TIMER2A, LOW_PRIORITY);                // set priority
    IntEnable(INT_TIMER2A);                                    // enable timer 2A interrupt
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER2_BASE, TIMER_A);

}
