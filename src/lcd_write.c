#include "lcd.h"
#include "fonts.h"

/******************************************************************************************************/
void write_char(int w, COLOR color, COLOR backcolor)
{
    int lv;
    for (lv = 0; lv < 12; lv++)
    {
        if (w & 1)
        {             // Write data byte
            GPIO_PORTM_DATA_R = color.red;
            GPIO_PORTL_DATA_R = 0x15;        // Chip select = 0, Write state = 0
            GPIO_PORTL_DATA_R = 0x1F;               // Initial state
            GPIO_PORTM_DATA_R = color.green;
            GPIO_PORTL_DATA_R = 0x15;        // Chip select = 0, Write state = 0
            GPIO_PORTL_DATA_R = 0x1F;               // Initial state
            GPIO_PORTM_DATA_R = color.blue;
            GPIO_PORTL_DATA_R = 0x15;        // Chip select = 0, Write state = 0
            GPIO_PORTL_DATA_R = 0x1F;               // Initial state
        }
        else
        {
            GPIO_PORTM_DATA_R = backcolor.red;
            GPIO_PORTL_DATA_R = 0x15;        // Chip select = 0, Write state = 0
            GPIO_PORTL_DATA_R = 0x1F;               // Initial state
            GPIO_PORTM_DATA_R = backcolor.green;
            GPIO_PORTL_DATA_R = 0x15;        // Chip select = 0, Write state = 0
            GPIO_PORTL_DATA_R = 0x1F;               // Initial state
            GPIO_PORTM_DATA_R = backcolor.blue;
            GPIO_PORTL_DATA_R = 0x15;        // Chip select = 0, Write state = 0
            GPIO_PORTL_DATA_R = 0x1F;               // Initial state
        }
        w = w >> 1;
    }
}

/******************************************************************************************************/
void print_string(char *text, int row, int column, COLOR color, COLOR backcolor)
{
    int w;
    int lv1, numChar;
    int length = strlen(text);
    int font_width = FONT_WIDTH_BIG;
    int font_hight = FONT_HIGHT_BIG;
    int columnStart = column;
    int columnStop = columnStart + font_width - 1;
    int rowStart = row;
    int rowStop = rowStart + font_hight - 1;

    for (numChar = 0; numChar < length; numChar++)
    {
        write_position(columnStart, rowStart, columnStop, rowStop);
        write_command(0x2C);
        for (lv1 = 0; lv1 < 32; lv1 = lv1 + 2)
        {
            w = (font_12_16[text[numChar]][lv1 + 1] << 4)
                    | (font_12_16[text[numChar]][lv1] >> 4);
            write_char(w, color, backcolor);
        }
        columnStart += font_width;
        columnStop += font_width;
    }
}

/******************************************************************************************************/
//Writes the hole screen in one color
void write_screen_color(COLOR color)
{
    int count = 0;

    //while(GPIO_PORTK_DATA_R == 0);
    //while(GPIO_PORTK_DATA_R == 1);              // start at the falling edge of TE signal
    write_position(0, 0, 479, 271);
    write_command(0x2C);
    //GPIO_PORTM_DATA_R = color.red;              // Write data byte

//    GPIO_PORTK_DATA_R = 2;                      // Trigger (Port K(1)) High

    while (count++ < 130560)
    {

#ifdef BIT24
        GPIO_PORTM_DATA_R = color.red;              // Write data byte
        GPIO_PORTL_DATA_R = 0x15;            // Chip select = 0, Write state = 0
        GPIO_PORTL_DATA_R = 0x1F;               // Initial state

        GPIO_PORTM_DATA_R = color.green;            // Write data byte
        GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
        GPIO_PORTL_DATA_R = 0x1F;           // Initial state

        GPIO_PORTM_DATA_R = color.blue;              // Write data byte
        GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
        GPIO_PORTL_DATA_R = 0x1F;           // Initial state
#else
                GPIO_PORTL_DATA_R = 0x15;    // Chip select = 0, Write state = 0
                GPIO_PORTL_DATA_R = 0x1F;// Initial state
#endif
    }

//    GPIO_PORTK_DATA_R = 0;                      // Trigger (Port K(1)) Low
}

/******************************************************************************************************/
void write_command(unsigned char command)
{
    GPIO_PORTM_DATA_R = command;        // Write command byte
    GPIO_PORTL_DATA_R = 0x11; // Chip select = 0, Command mode select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
}

/******************************************************************************************************/
void write_cmd_data(unsigned char data)
{
    GPIO_PORTM_DATA_R = data;           // Write data byte
    GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
}

/******************************************************************************************************/
void write_data(COLOR color)
{
    GPIO_PORTM_DATA_R = color.red;      // Write data byte
    GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state

#ifdef BIT24
    GPIO_PORTM_DATA_R = color.green;    // Write data byte
    GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state

    GPIO_PORTM_DATA_R = color.blue;     // Write data byte
    GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
#endif
}

/******************************************************************************************************/
void write_position(short point1_x, short point1_y, short point2_x,
                    short point2_y)
{
    write_command(0x2A);                    // Set page address (x-axis)
    write_cmd_data(point1_x >> 8);   // Set start page address                HB
    write_cmd_data(point1_x);        //                                       LB
    write_cmd_data(point2_x >> 8);   // Set stop page address                 HB
    write_cmd_data(point2_x);        //                                       LB

    write_command(0x2B);                    // Set column address (y-axis)
    write_cmd_data(point1_y >> 8);   // Set start column address              HB
    write_cmd_data(point1_y);        //                                       LB
    write_cmd_data(point2_y >> 8);   // Set stop column address               HB
    write_cmd_data(point2_y);        //                                       LB
}

/******************************************************************************************************/
//draws a line from startpoint x to stoppoint y directly to the display
void write_line(short startx, short starty, short stopx, short stopy,
                COLOR color)
{
    short old_x, old_y, x, y, i;
    int start_x, stop_x, start_y, stop_y;
    double gain;

    // 90° line:
    if (startx == stopx)
    {
        if (starty > stopy)
        {        // 90° from DOWN to UP   else: 270° from UP to DOWN
            start_y = stopy;
            stop_y = starty;
        }
        else
        {
            start_y = starty;
            stop_y = stopy;
        }
        write_position(startx, start_y, stopx, stop_y);
        write_command(0x2C);
        for (x = start_y; x <= stop_y; x++)
        {
            write_data(color);
        }
    }
    // 0° line:
    else if (starty == stopy)
    {
        if (startx > stopx)
        {
            start_x = stopx;
            stop_x = startx;
        }
        else
        {
            start_x = startx;
            stop_x = stopx;
        }
        write_position(start_x, starty, stop_x, stopy);
        write_command(0x2C);
        for (x = start_x; x <= stop_x; x++)
        {
            write_data(color);
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////
    else
    {
        if (startx > stopx)
        {      // running direction is negative ! => switch start and stop
            start_x = stopx;
            stop_x = startx;
            start_y = stopy;
            stop_y = starty;
        }
        else
        {
            start_x = startx;
            stop_x = stopx;
            start_y = starty;
            stop_y = stopy;
        }
        //(stop_y - start_y) can be positive or negative
        gain = (double) (stop_y - start_y) / (stop_x - start_x);

        if (gain >= 1)
        {
            old_y = start_y;

            for (x = start_x; x <= stop_x; x++)
            {
                y = gain * (x - start_x) + start_y;
                write_position(x, old_y, x, y);
                write_command(0x2C);
                for (i = old_y; i <= y; i++)
                {
                    write_data(color);
                }
                old_y = y;
            }
        }
        else if (gain > 0)
        {
            old_x = start_x;

            for (y = start_y; y <= stop_y; y++)
            {
                x = (y - start_y) / gain + start_x;
                write_position(old_x, y, x, y);
                write_command(0x2C);
                for (i = old_x; i <= x; i++)
                {
                    write_data(color);
                }
                old_x = x;
            }
        }

        else if (gain <= -1)
        {
            old_y = start_y;

            for (x = start_x; x <= stop_x; x++)
            {
                y = gain * (x - start_x) + start_y;
                write_position(x, y, x, old_y);
                write_command(0x2C);
                for (i = y; i <= old_y; i++)
                {
                    write_data(color);
                }
                old_y = y;
            }
        }
        // start_y > stop_y
        else if (gain < 0)
        {
            old_x = start_x;

            for (y = start_y; y >= stop_y; y--)
            {
                x = (y - start_y) / gain + start_x;
                write_position(old_x, y, x, y);
                write_command(0x2C);
                for (i = old_x; i <= x; i++)
                {
                    write_data(color);
                }
                old_x = x;
            }
        }
    }
}

void darstellung_tacho(void)
{
    COLOR backroundColor = (COLOR) BLACK;
    static int n = 0, x = CENTER_X, y = CENTER_Y;
    uint16_t x1 = 225, y1 = CENTER_Y, N1 = 1500;
    uint16_t x2 = 225, y2 = CENTER_Y;
    uint16_t xa, ya, xb, yb;
    uint16_t xa1, ya1, xb1, yb1;

    char tachoNumberInString[20];
    int tachoNumber = 0;
    // Darstellung des Tachos
    for (n = 400; n <= 800; n = n + 40)
    {

        x = 255 + round(175 * sin((double) n * 2 * PI / 800)); // 250
        y = 220 + round(175 * cos((double) n * 2 * PI / 800)); // 205

        xa = CENTER_X + round(155 * cos((double) n * 2 * PI / 800));
        ya = CENTER_Y + round(155 * sin((double) n * 2 * PI / 800));

        xb = CENTER_X + round(140 * cos((double) n * 2 * PI / 800));
        yb = CENTER_Y + round(140 * sin((double) n * 2 * PI / 800));

        xa1 = CENTER_X + round(155 * cos((double) (n + 1) * 2 * PI / 800));
        ya1 = CENTER_Y + round(155 * sin((double) (n + 1) * 2 * PI / 800));

        xb1 = CENTER_X + round(140 * cos((double) (n + 1) * 2 * PI / 800));
        yb1 = CENTER_Y + round(140 * sin((double) (n + 1) * 2 * PI / 800));

        sprintf(tachoNumberInString, "%3d", tachoNumber);
        write_line(xa, ya, xb, yb, (COLOR) WHITE);
        write_line(xa1, ya1, xb1, yb1, (COLOR) WHITE);

        print_string(tachoNumberInString, x, y, (COLOR) RED, backroundColor);
        tachoNumber += 40;

    }

    // Darstellung der unteren gekrümmten Kurve

    for (n = N1 / 2; n <= N1; n++)
    {
        x = CENTER_X + round(150 * cos((double) n * 2 * PI / N1));
        y = 270 + round(150 * sin((double) n * 2 * PI / N1));
        if (n > N1 / 2)
        {
            write_line(x1, y1, x, y, (COLOR) BLUE);
        }
        x1 = x;
        y1 = y;
    }
    uint16_t mitten_linie_links_x, mitten_linie_links_y, mitten_linie_rechts_x,
            mitten_linie_rechts_y;
    uint16_t punkt_unter_x = CENTER_X
            + round(150 * cos((double) (N1 / 2 - 1) * 2 * PI / N1));
    uint16_t punkt_unter_y = 270
            + round(150 * sin((double) (N1 / 2 - 1) * 2 * PI / N1));
    uint16_t punkt_oben_x = CENTER_X
            + round(200 * cos((double) (N1 / 2 - 1) * 2 * PI / N1));
    uint16_t punkt_oben_y = 270
            + round(200 * sin((double) (N1 / 2 - 1) * 2 * PI / N1));
    mitten_linie_links_x = punkt_unter_x;
    mitten_linie_links_y = punkt_unter_y;

    write_line(punkt_unter_x, punkt_unter_y, punkt_oben_x, punkt_oben_y,
               (COLOR) BLUE);

    punkt_unter_x = CENTER_X
            + round(150 * cos((double) (N1 + 1) * 2 * PI / N1));
    punkt_unter_y = 270 + round(150 * sin((double) (N1 + 1) * 2 * PI / N1));
    punkt_oben_x = CENTER_X + round(200 * cos((double) (N1 + 1) * 2 * PI / N1));
    punkt_oben_y = 270 + round(200 * sin((double) (N1 + 1) * 2 * PI / N1));
    mitten_linie_rechts_x = punkt_unter_x;
    mitten_linie_rechts_y = punkt_unter_y;

    write_line(punkt_unter_x, punkt_unter_y, punkt_oben_x, punkt_oben_y,
               (COLOR) BLUE);
    write_line(mitten_linie_links_x, mitten_linie_links_y,
               mitten_linie_rechts_x, mitten_linie_rechts_y, (COLOR) BLUE);

    // Darstellung der oberen gekrümmten Kurve

    for (n = N1 / 2; n <= N1; n++)
    {
        x = CENTER_X + round(200 * cos((double) n * 2 * PI / N1));
        y = 270 + round(200 * sin((double) n * 2 * PI / N1));
        if (n > N1 / 2)
        {
            write_line(x2, y2, x, y, (COLOR) BLUE);
        }
        x2 = x;
        y2 = y;
    }

    for (n = 400; n <= 800; n++)
    {
        x = CENTER_X + round(10 * cos((double) n * 2 * PI / 800));
        y = CENTER_Y + round(10 * sin((double) n * 2 * PI / 800));
        if (n > 400)
        {
            write_line(x2, y2, x, y, (COLOR) WHITE);
            write_line(CENTER_X, CENTER_Y, x, y, (COLOR) WHITE);
        }
        x2 = x;
        y2 = y;
    }

    write_line(punkt_unter_x, punkt_unter_y, punkt_oben_x, punkt_oben_y,
               (COLOR) BLUE);

}

void darstellung_display(void)
{
    print_string("MC-Projekt Fahzeug-Informations-Display", 0, 0, (COLOR) WHITE,
                 backroundColor);
    print_string("Geschwindigkeit", 25, 1, (COLOR) YELLOW, backroundColor);
    print_string("Richtung", 20, 200, (COLOR) YELLOW, backroundColor);
    print_string("Tageskilometer", 25, 310, (COLOR) YELLOW, backroundColor);

}

