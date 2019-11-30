#include "lcd.h"

// The Timer2 function is used to write to the screen!
void Timer2IntHandler(void)
{
    static uint16_t n = 400;
    static uint16_t x = CENTER_X, y = CENTER_Y;
    static uint16_t x1 = CENTER_X - 10, y1 = CENTER_Y;
    static uint16_t x2 = 225, y2 = CENTER_Y;
    char buffer[20];
    //GPIO_PORTK_DATA_R |= GPIO_PIN_3;
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    sprintf(buffer, "%3d km/h", velocity);
    print_string(buffer, 50, 20, (COLOR) RED, backroundColor);
    sprintf(buffer, "%.2f km", dailyOdometer);
    print_string(buffer, 50, 350, (COLOR) RED, backroundColor);

    static uint16_t a = CENTER_X, b = CENTER_Y;
    static uint16_t a1 = CENTER_X - 10, b1 = CENTER_Y;
    if (n > 400)
    {
        write_line(x2, y2, x, y, backroundColor);
        write_line(x, y, x1, y1, backroundColor);
        write_line(a, b, a1, b1, backroundColor);
        write_line(x1, y1, a1, b1, backroundColor);
    }

    x = CENTER_X + round(10 * cos((double) n * 2 * PI / 800));
    y = CENTER_Y + round(10 * sin((double) n * 2 * PI / 800));
    x1 = CENTER_X + 120 * cos((double) n * 2 * PI / 800);
    y1 = CENTER_Y + 120 * sin((double) n * 2 * PI / 800);

    a = CENTER_X + round(10 * cos((double) (n + 6) * 2 * PI / 800));
    b = CENTER_Y + round(10 * sin((double) (n + 6) * 2 * PI / 800));
    a1 = CENTER_X + 120 * cos((double) (n + 6) * 2 * PI / 800);
    b1 = CENTER_Y + 120 * sin((double) (n + 6) * 2 * PI / 800);

    if (n > 400)
    {
        write_line(x2, y2, x, y, (COLOR) WHITE);
        write_line(x, y, x1, y1, (COLOR) WHITE);
        write_line(a, b, a1, b1, (COLOR) WHITE);
        write_line(x1, y1, a1, b1, (COLOR) WHITE);
    }
    x2 = x;
    y2 = y;
    n++;
    if (n > 800)
    {
        n = 400;
        write_line(x2, y2, x, y, backroundColor);
        write_line(x, y, x1, y1, backroundColor);
        write_line(a, b, a1, b1, backroundColor);
        write_line(x1, y1, a1, b1, backroundColor);
    }
    movementDirection = BACKWARD;
    switch (movementDirection)
    {
    case NONE:
        print_string("   ", 40, 220, backroundColor, backroundColor);
        break;
    case BACKWARD:
        print_string("(B)", 40, 220, (COLOR) RED, backroundColor);
        break;
    case FORWARD:
        print_string("(F)", 40, 220, (COLOR) RED, backroundColor);
        break;
    }

    //GPIO_PORTK_DATA_R &= ~GPIO_PIN_3;
}

// PortK(1) wheel sensor rising edge Interrupt handler
// TIMER0 is used to measure the time between two wheel impulses
// TIMER1 is a watchdog timer => No impulse for 500ms: timer1 handler function sets velocity to 0
void portD1IntHandler(void)
{
    static uint32_t kmCounter = 0;
    TimerDisable(TIMER0_BASE, TIMER_A);             // stop timer 0
    velocity = timerScaler / TIMER0_TAV_R;         // read and transform measured time
    TIMER0_TAV_R = 0;                  // reset timer 0
    TimerEnable(TIMER0_BASE, TIMER_A);             // start timer 0 again
    GPIOIntClear(GPIO_PORTD_AHB_BASE, GPIO_PIN_1);       // clear edge interrupt
//get movement direction
    if (SENSOR2)
    {
        movementDirection = BACKWARD;
    }
    else
    {
        movementDirection = FORWARD;
    }
    kmCounter++;                                    //
    dailyOdometer = (float) kmCounter / 1000;

    TimerLoadSet(TIMER1_BASE, TIMER_A, sysClk / 2); // timer1 set to fire after 500 ms
}

// Set velocity to 0 while wheel-modul isn't moving (=> and no rising edge occurs)
void Timer1IntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    velocity = 0;
    movementDirection = NONE;
}
