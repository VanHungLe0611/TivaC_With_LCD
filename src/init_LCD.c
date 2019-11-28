#include "lcd.h"

/******************************************************************************************************/
void wait(int time)
{
    volatile int tmp;

    for (tmp = 0; tmp < 10800 * time; tmp++)
        ; // ~ 1ms
}

/******************************************************************************************************/
// LCD Panel initialize:
void init_LCD(void)
{
    int value;
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
    wait(10);                               //

    GPIO_PORTL_DATA_R &= ~RST;           // Hardware reset
    wait(1);                                //
    GPIO_PORTL_DATA_R |= RST;               //
    wait(1);                                //

    write_command(0x01);                // Software reset
    wait(10);                               //

    write_command(0xE2);                // Set PLL Freq to 120 MHz
    write_cmd_data(0x24);                   //
    write_cmd_data(0x02);                   //
    write_cmd_data(0x04);                   //

    write_command(0xE0);                // Start PLL
    write_cmd_data(0x01);                   //
    wait(1);                                //

    write_command(0xE0);                // Lock PLL
    write_cmd_data(0x03);                   //
    wait(1);                                //

    write_command(0x01);                // Software reset
    wait(10);                               //
    /*************************************************************************/

    value = 0x01EFFF;                //0x03FFFF;
    write_command(0xE6);               // Set LCD Pixel Clock 11,4Mhz (0x01D4C0)
    write_cmd_data(value >> 16);              //
    write_cmd_data(value >> 8);               //
    write_cmd_data(value);                  //

    value = 0x100;
    write_command(0x44);                    //
    write_cmd_data(value >> 8);               //
    write_cmd_data(value);                  //

    write_command(0xB0);                // Set LCD Panel mode to:
    write_cmd_data(0x20);                   // ..TFT panel 24bit
    write_cmd_data(0x00);                   // ..TFT mode
    write_cmd_data(0x01);             // Horizontal size 480-1 (aka 479 ;)    HB
    write_cmd_data(0xDF);             // Horizontal size 480-1                LB
    write_cmd_data(0x01);             // Vertical size 272-1   (aka 271 ;)    HB
    write_cmd_data(0x0F);             // Vertical size 272-1                  LB
    write_cmd_data(0x00);                   // even/odd line RGB

    write_command(0xB4);                // Set Horizontal period
    write_cmd_data(0x02);             // Set HT total pixel=531               HB
    write_cmd_data(0x13);             // Set HT total pixel=531               LB
    write_cmd_data(0x00);             // Set Horiz.sync pulse start pos = 43  HB
    write_cmd_data(0x2B);             // Set Horiz.sync pulse start pos = 43  LB
    write_cmd_data(0x0A);                   // Set horiz.sync pulse with = 10
    write_cmd_data(0x00);             // Set horiz.Sync pulse start pos= 8    HB
    write_cmd_data(0x08);             // Set horiz.Sync pulse start pos= 8    LB
    write_cmd_data(0x00);                   //

    write_command(0xB6);                // Set Vertical Period
    write_cmd_data(0x01);             // Set VT lines = 288                   HB
    write_cmd_data(0x20);             // Set VT lines = 288                   LB
    write_cmd_data(0x00);             // Set VPS = 12                         HB
    write_cmd_data(0x0C);             // Set VPS = 12                         LB
    write_cmd_data(0x0A);                   // Set vert.sync pulse with = 10
    write_cmd_data(0x00);              // Set vert.Sync pulse start pos= 8    HB
    write_cmd_data(0x00);              // Set vert.Sync pulse start pos= 8    LB
    write_cmd_data(0x04);                   //

    write_command(0x36); // Flip Display - necessary to match with Touch Display
    write_cmd_data(0x03);                   //

#ifdef BIT16
    write_command(0xF0);                // Set LCD color data format
    write_cmd_data(0b00000011);// Set pixel data format = 16-bit (1 cycle => 16 GPIOs)
#endif

#ifdef BIT24
    write_command(0xF0);                // Set LCD color data format
    write_cmd_data(0x00);               // Set pixel data format = 8 bit
#endif

    write_command(0x29);                // Set display on
}

