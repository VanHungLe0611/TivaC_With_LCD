#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "tm4c1294ncpdt.h"
#include "inc/hw_memmap.h" //  needed for interrupts
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"

//define LCD data format: BIT16: 1x16bit, BIT24: 3x8bit
//#define BIT16 1
#define BIT24 2

#define RST 0x10
#define MAX_WIDTH (480 - 12)
#define MAX_HIGHT (272 - 16)
#define CENTER_X 240
#define CENTER_Y  265
//#define NONE 1
//#define BACKWARD 2
//#define FORWARD 3
#define HIGH_PRIORITY 0x00
#define LOW_PRIORITY 0x80
#define SENSOR1 (GPIO_PORTK_DATA_R & GPIO_PIN_1)
#define SENSOR2 (GPIO_PORTK_DATA_R & GPIO_PIN_2)
#define OUT1 (GPIO_PORTK_DATA_R & GPIO_PIN_3)
#define OUT2 (GPIO_PORTK_DATA_R & GPIO_PIN_4)
#define PI 3.1415926536

// enum colors
enum
{
    BLACK = 0x000000,
    RED = 0x0000FF,
    GREEN = 0x00FF00,
    YELLOW = 0x00FFFF,
    BLUE = 0xFF0000,
    WHITE = 0xFFFFFF
};
// enum movement direction
enum
{
    NONE, FORWARD, BACKWARD
};

// Typedefs:
typedef union
{
    int all;
    struct
    {
        char red;
        char green;
        char blue;
    };
} COLOR;

// Prototypes:
void init_peripherals(void);
void init_LCD(void);

void wait(int);
void wait_us(int);
void write_command(unsigned char);
void write_cmd_data(unsigned char);
void write_data(COLOR);
void write_position(short, short, short, short);
void write_screen_color(COLOR);
void write_line(short, short, short, short, COLOR);

void print_string(char*, int, int, COLOR, COLOR backcolor);
void write_char(int, COLOR, COLOR backcolor);
void write_image(void);

// display tacho
void darstellung_tacho(void);
void darstellung_display(void);

// interrupt handler
//void Timer2IntHandler(void);
//void portD1IntHandler(void);
//void Timer1IntHandler(void);
void portD1IntHandler(void);
void Timer1IntHandler(void);
void Timer2IntHandler(void);
void enable_interrupt(void);
//extern uint32_t sysClk;

// global variables
static COLOR backroundColor = (COLOR) BLACK;
//global interface variables from wheel sensor
static uint16_t movementDirection = NONE;
static uint32_t velocity;
static float dailyOdometer;
static uint32_t timerScaler;
static uint32_t sysClk;

/* LCD Grid  ~middle 240x136

 x-axis
 1 2 3 ........479 480
 1 * * * * * * * * * *
 2 *|              | *
 y-axis     .. *|              | *
 271 *|______________| *
 272 * * * * * * * * * *
 */
