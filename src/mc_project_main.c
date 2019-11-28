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

    // Interrupt enable
    IntMasterEnable();

    while (1)
        ;
}
