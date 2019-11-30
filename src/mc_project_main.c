#include "lcd.h"

void main(void)
{
    sysClk = SysCtlClockFreqSet(
            (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
                    | SYSCTL_CFG_VCO_480),
            120000000);
    timerScaler = sysClk * 3.6;      // needed to calculate the current velocity
    // Initiation GPIO and Timer
    init_peripherals();
    // Initiation LCD
    init_LCD();

    write_screen_color(backroundColor);
    darstellung_display();

    // Dartellung Tacho
    darstellung_tacho();
    enable_interrupt();

    // Interrupt enable
    IntMasterEnable();

    while (1)
        ;
}

void enable_interrupt(void)
{
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
    IntEnable(INT_TIMER2A);                         // enable timer 2A interrupt
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER2_BASE, TIMER_A);

}

