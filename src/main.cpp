#include <Arduino.h>
#include "LedControl.h"

// Function Prototype
void plot(uint8_t x, uint8_t y, uint8_t value);
void test();
void clear_display();
void fade_down();
void test_all_led();
void print_tiny_char(uint8_t x, uint8_t y, char c);
void print_normal_char(uint8_t x, uint8_t y, char c);
void Set_Clock();
void Show_time();
void Show_day();
// define Zone
#define pin1 10
#define pin2 11
#define pin3 12
#define ledPin 13
#define bot1 4
#define bot2 5
#define botcoc 3
#define botcob 2
#define botsetd 7
#define coc 0
#define clearC 1
#define DM 2
#define clearDM 3
#define set 4
#define clearSet 5
#define count 6
#define clearCo 7
#define PIN_B 9
#define PIN_C 2
#define PIN_D 6
#define PIN_A 7
#define PIN_E 8

// Define PIN dot matrix
LedControl lc = LedControl(10, 11, 12, 4); // DIN,CLK,CS,Number of LED Module

// Clock

struct Clock
{
    char t2 = '0', t1 = '0';
    char min1 = '6', min2 = '0';
    char hr1 = '2', hr2 = '1';
    int day = 1;
    int month = 1;
    int day_count = 1;
} Clock;

// Clock set
struct Clock_set
{
    int state_set = 0;
    char min1_set = '0';
    char min2_set = '0';
    char hr1_set = '0';
    char hr2_set = '0';
    int day_set = 1;
    int chk_hr = 0;

} Clock_set;

struct keep_alam
{
    char min1K = '0';
    char min2K = '0';
    char hr1K = '0';
    char hr2K = '0';
} keep_alam;

struct Clock_count_down

{
    int state_setC = 0;
    char min1C = '0';
    char min2C = '0';
    char hr1C = '0';
    char hr2C = '0';
    int day_setC = 1;
    int chk_hrC = 0;
    int alam = 0;
    int alam_show = 0;
    int beep_sound = 0;
} Clock_count_down;

struct Timer
{
    unsigned long long timermin1;
    unsigned long long timermin2;
    unsigned long long timerhr1;
    unsigned long long timestop;
    unsigned long long timersetd;
    unsigned long long timerClockSet;
    unsigned long long timeralarm;
    int timer1_counter;
    int Count_down_state = 0;
} Timer;

struct Essential
{
    uint8_t intensity = 1;
    unsigned long delaytime = 50; // time to updates of the display
    unsigned long s = 0;
    unsigned long input, input1, input2;
    int show = 0;

} Essential;

struct Alarm_clock
{
    char hr2A = '0';
    char hr1A = '0';
    char min2A = '0';
    char min1A = '0';
    int Alarm_clock_state = 0;
    int set_click = 0;
    int Beep_state = 0;
    int alarm_work = 1;
    /* data */
} Alarm_clock;

// FINITE STATE
struct state
{
    unsigned long out;  //Light
    unsigned long Time; //wait
    unsigned long next[4];
};

struct Count_down
{
    /* data */
    char sec1 = '0';
    char sec2 = '6';
    char min1 = '0';
    char min2 = '0';
    char hr1 = '0';
    char hr2 = '0';
} Count_down;

uint8_t d = 0;
char sun[4] = "Sun.";
char mon[4] = "Mon.";
char tue[4] = "Tue.";
char wed[4] = "Wed.";
char the[4] = "Thu.";
char fri[4] = "Fri.";
char st[4] = "Sat.";

typedef const struct state sType;
sType FSM[8] =
    {
        //SFM  TIME   00     01        10         11
        {0, 50, {coc, clearC, clearDM, clearC}},    //coc     -----Show Clock
        {1, 20, {DM, DM, set, count}},              //clearC  -----Clear Screen
        {2, 50, {DM, DM, clearDM, clearDM}},        //DM      -----Show Day & Month
        {3, 20, {set, DM, set, count}},             //clearDM -----CLear screen
        {4, 50, {set, clearSet, set, clearSet}},    //set     -----set day,month,time,
        {5, 20, {DM, DM, set, count}},              //clearset-----Clear screen
        {6, 500, {count, clearCo, clearCo, count}}, //count    -----Show Time Countdown
        {7, 10, {coc, DM, set, count}}              //clearCount---Clear screen
};

// Font
const int font_count = 80;
unsigned const char PROGMEM myfont[80][5] = {
    {0, 0, 0, 0, 0},                // space
    {0x3f, 0x48, 0x48, 0x48, 0x3f}, // A
    {0x7f, 0x49, 0x49, 0x49, 0x36},
    {0x3e, 0x41, 0x41, 0x41, 0x22},
    {0x7f, 0x41, 0x41, 0x22, 0x1c},
    {0x7f, 0x49, 0x49, 0x49, 0x41},
    {0x7f, 0x48, 0x48, 0x48, 0x40},
    {0x3e, 0x41, 0x49, 0x49, 0x2e},
    {0x7f, 0x08, 0x08, 0x08, 0x7f},
    {0x00, 0x41, 0x7f, 0x41, 0x00},
    {0x06, 0x01, 0x01, 0x01, 0x7e},
    {0x7f, 0x08, 0x14, 0x22, 0x41},
    {0x7f, 0x01, 0x01, 0x01, 0x01},
    {0x7f, 0x20, 0x10, 0x20, 0x7f},
    {0x7f, 0x10, 0x08, 0x04, 0x7f},
    {0x3e, 0x41, 0x41, 0x41, 0x3e},
    {0x7f, 0x48, 0x48, 0x48, 0x30},
    {0x3e, 0x41, 0x45, 0x42, 0x3d},
    {0x7f, 0x48, 0x4c, 0x4a, 0x31},
    {0x31, 0x49, 0x49, 0x49, 0x46},
    {0x40, 0x40, 0x7f, 0x40, 0x40},
    {0x7e, 0x01, 0x01, 0x01, 0x7e},
    {0x7c, 0x02, 0x01, 0x02, 0x7c},
    {0x7f, 0x02, 0x04, 0x02, 0x7f},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x60, 0x10, 0x0f, 0x10, 0x60},
    {0x43, 0x45, 0x49, 0x51, 0x61}, // Z

    {0x01, 0x00, 0x00, 0x00, 0x00}, // .
    {0x00, 0x00, 0x70, 0x00, 0x00}, // '
    {0x00, 0x00, 0x14, 0x00, 0x00}, // :
    {0x7F, 0x22, 0x14, 0x8, 0x00},  // selector arrow

    {0x3E, 0x45, 0x49, 0x51, 0x3E}, // 0 - 9
    {0x00, 0x21, 0x7F, 0x1, 0x0},
    {0x21, 0x43, 0x45, 0x49, 0x31},
    {0x22, 0x41, 0x49, 0x49, 0x36},
    {0xC, 0x14, 0x24, 0x7F, 0x4},
    {0x72, 0x51, 0x51, 0x51, 0x4E},
    {0x3E, 0x49, 0x49, 0x49, 0x26},
    {0x40, 0x40, 0x4F, 0x50, 0x60},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x32, 0x49, 0x49, 0x49, 0x3E},

    {0x2, 0x15, 0x15, 0x15, 0xF}, // a
    {0x7F, 0x5, 0x9, 0x9, 0x6},
    {0xE, 0x11, 0x11, 0x11, 0x2},
    {0x6, 0x9, 0x9, 0x5, 0x7F},
    {0xE, 0x15, 0x15, 0x15, 0xC},
    {0x8, 0x3F, 0x44, 0x40, 0x20}, // f
    {0x18, 0x25, 0x25, 0x25, 0x3E},
    {0x7F, 0x8, 0x10, 0x10, 0xF},
    {0x0, 0x0, 0x2F, 0x0, 0x0},
    {0x2, 0x1, 0x11, 0x5E, 0x0},
    {0x7F, 0x4, 0x6, 0x11, 0x0},
    {0x0, 0x41, 0x7F, 0x1, 0x0}, // l
    {0x1F, 0x10, 0xC, 0x10, 0xF},
    {0x1F, 0x8, 0x10, 0x10, 0xF},
    {0xE, 0x11, 0x11, 0x11, 0xE},
    {0x1F, 0x14, 0x14, 0x14, 0x8},
    {0x8, 0x14, 0x14, 0xC, 0x1F}, // q
    {0x1F, 0x8, 0x10, 0x10, 0x8},
    {0x9, 0x15, 0x15, 0x15, 0x2},
    {0x10, 0x7E, 0x11, 0x1, 0x2},
    {0x1E, 0x1, 0x1, 0x2, 0x1F}, // u
    {0x1C, 0x2, 0x1, 0x2, 0x1C},
    {0x1E, 0x1, 0x3, 0x1, 0x1E},
    {0x11, 0xA, 0x4, 0xA, 0x11},
    {0x18, 0x5, 0x5, 0x5, 0x1E},
    {0x11, 0x13, 0x15, 0x19, 0x11}, // z

    // invader sprites - may still need some tweaking
    {0x0, 0x19, 0x3A, 0x6D, 0x7A}, // top invader left
    {0x7A, 0x6D, 0x3A, 0x19, 0x0}, // top invader right
    {0x0, 0x1A, 0x3D, 0x68, 0x7C}, // top invader left wiggle
    {0x7C, 0x68, 0x3D, 0x1A, 0x0}, // top invader right wiggle
    {0x38, 0xD, 0x5E, 0x36, 0x1C}, // middle invader left
    {0x1C, 0x36, 0x5E, 0xD, 0x38},
    {0xE, 0xC, 0x5E, 0x35, 0x1C}, // middle invader left wiggle
    {0x1C, 0x35, 0x5E, 0xC, 0xE},
    {0x19, 0x39, 0x3A, 0x6C, 0x7A}, // bottom invader left
    {0x7A, 0x6C, 0x3A, 0x39, 0x19},
    {0x18, 0x39, 0x3B, 0x6C, 0x7C}, // bottom invader left wiggle
    {0x7C, 0x6C, 0x3B, 0x39, 0x18},

};

const int bigfont_count = 10;
unsigned const char PROGMEM mybigfont[10][20] = {
    {0x3F, 0x7F, 0xE0, 0xC1, 0xC3, 0xC7, 0xCE, 0xFC, 0x7F, 0x3F, 0xF0, 0xF8, 0xFC, 0xCC, 0x8C, 0xC, 0xC, 0x3C, 0xF8, 0xF0} //0
    ,
    {0x0, 0x0, 0x30, 0x70, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xC, 0xC, 0xFC, 0xFC, 0xC, 0xC, 0x0, 0x0},
    {0x30, 0x70, 0xE0, 0xC0, 0xC0, 0xC1, 0xC3, 0xE7, 0x7E, 0x3C, 0xC, 0x1C, 0x3C, 0x7C, 0xEC, 0xCC, 0x8C, 0xC, 0xC, 0xC},
    {0x30, 0x70, 0xE0, 0xC0, 0xC0, 0xC3, 0xC3, 0xE3, 0x7F, 0x3C, 0x30, 0x38, 0x1C, 0xC, 0xC, 0xC, 0xC, 0x1C, 0xF8, 0xF0},
    {0x3, 0x7, 0xE, 0x1C, 0x38, 0x70, 0xFF, 0xFF, 0x0, 0x0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC0, 0xC0},
    {0xFC, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCE, 0xC7, 0xC3, 0x30, 0x38, 0x1C, 0xC, 0xC, 0xC, 0xC, 0x1C, 0xF8, 0xF0},
    {0x3F, 0x7F, 0xE3, 0xC3, 0xC3, 0xC3, 0xC3, 0xE3, 0x61, 0x20, 0xF0, 0xF8, 0x1C, 0xC, 0xC, 0xC, 0xC, 0x9C, 0xF8, 0xF0},
    {0xC0, 0xC0, 0xC0, 0xC1, 0xC3, 0xC7, 0xCE, 0xDC, 0xF8, 0xF0, 0x0, 0x0, 0x0, 0xFC, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0x3C, 0x7F, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7F, 0x3C, 0xF0, 0xF8, 0x9C, 0xC, 0xC, 0xC, 0xC, 0x9C, 0xF8, 0xF0},
    {0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x7F, 0x3F, 0x10, 0x18, 0x1C, 0xC, 0xC, 0xC, 0xC, 0x1C, 0xF8, 0xF0}};

const int tinyfont_count = 42;
unsigned const int PROGMEM mytinyfont[42][3] = {

    //a-z
    {0, 0, 0} // space!
    ,
    {0x1F, 0x14, 0x1F},
    {0x1F, 0x15, 0xA},
    {0x1F, 0x11, 0x11},
    {0x1F, 0x11, 0xE},
    {0x1F, 0x15, 0x11},
    {0x1F, 0x14, 0x10},
    {0x1F, 0x11, 0x17},
    {0x1F, 0x4, 0x1F},
    {0x11, 0x1F, 0x11},
    {0x3, 0x1, 0x1F},
    {0x1F, 0x4, 0x1B},
    {0x1F, 0x1, 0x1},
    {0x1F, 0x8, 0x1F},
    {0x1F, 0x10, 0xF},
    {0x1F, 0x11, 0x1F},
    {0x1F, 0x14, 0x1C},
    {0x1C, 0x14, 0x1F},
    {0x1F, 0x16, 0x1D},
    {0x1D, 0x15, 0x17},
    {0x10, 0x1F, 0x10},
    {0x1F, 0x1, 0x1F},
    {0x1E, 0x1, 0x1E},
    {0x1F, 0x2, 0x1F},
    {0x1B, 0x4, 0x1B},
    {0x1C, 0x7, 0x1C},
    {0x13, 0x15, 0x19}

    ,
    {0x01, 0x00, 0x00} // .
    ,
    {0x00, 0x0A, 0x00} // :
    ,
    {0x18, 0x00, 0x00} // '
    ,
    {0x1D, 0x00, 0x00} // !
    ,
    {0x10, 0x15, 0x1C} // ?

    ,
    {0x1F, 0x11, 0x1F} //0 - 9
    ,
    {0x00, 0x00, 0x1F},
    {0x17, 0x15, 0x1D},
    {0x11, 0x15, 0x1F},
    {0x1C, 0x4, 0x1F},
    {0x1D, 0x15, 0x17},
    {0x1F, 0x15, 0x17},
    {0x10, 0x10, 0x1F},
    {0x1F, 0x15, 0x1F},
    {0x1D, 0x15, 0x1F}};

// Function
void test()
{
    int devices = lc.getDeviceCount(); // find no of LED modules

    for (int row = 32; row >= 0; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            delay(Essential.delaytime);
            plot(row, col, true);
            delay(Essential.delaytime);
            plot(row, col, false);
        }
    }
}

//plot a point on the display
void plot(uint8_t x, uint8_t y, uint8_t value)
{

    //select which matrix depending on the x coord
    uint8_t address;
    if (x >= 0 && x <= 7)
    {
        address = 3;
    }
    if (x >= 8 && x <= 15)
    {
        address = 2;
        x = x - 8;
    }
    if (x >= 16 && x <= 23)
    {
        address = 1;
        x = x - 16;
    }
    if (x >= 24 && x <= 31)
    {
        address = 0;
        x = x - 24;
    }

    if (value == 1)
    {
        lc.setLed(address, y, x, true);
    }
    else
    {
        lc.setLed(address, y, x, false);
    }
}

//clear screen
void clear_display()
{
    for (uint8_t address = 0; address < 4; address++)
    {
        lc.clearDisplay(address);
    }
}

//fade screen down
void fade_down()
{

    //fade from global intensity to 1
    for (uint8_t i = Essential.intensity; i > 0; i--)
    {
        for (uint8_t address = 0; address < 4; address++)
        {
            lc.setIntensity(address, i);
        }
        delay(30); //change this to change fade down speed
    }

    clear_display(); //clear display completely (off)

    //reset intentsity to global value
    for (uint8_t address = 0; address < 4; address++)
    {
        lc.setIntensity(address, Essential.intensity);
    }
}

void test_all_led()
{
    for (uint8_t x = 0; x <= 31; x++)
    {
        for (uint8_t y = 0; y <= 7; y++)
        {
            plot(x, y, 1);
            delay(10);
        }
    }
}

void print_tiny_char(uint8_t x, uint8_t y, char c)
{
    uint8_t dots;
    if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z'))
    {
        c &= 0x1F; // A-Z maps to 1-26
    }
    else if (c >= '0' && c <= '9')
    {
        c = (c - '0') + 32;
    }
    else if (c == ' ')
    {
        c = 0; // space
    }
    else if (c == '.')
    {
        c = 27; // full stop
    }
    else if (c == ':')
    {
        c = 28; // colon
    }
    else if (c == '\'')
    {
        c = 29; // single quote mark
    }
    else if (c == '!')
    {
        c = 30; // single quote mark
    }
    else if (c == '?')
    {
        c = 31; // single quote mark
    }

    for (uint8_t col = 0; col < 4; col++)
    {
        dots = pgm_read_byte_near(&mytinyfont[c][col]);
        for (uint8_t row = 0; row < 5; row++)
        {
            if (dots & (16 >> row))
                plot(x + col, y + row, 1);
            else
                plot(x + col, y + row, 0);
        }
    }
}

void print_normal_char(uint8_t x, uint8_t y, char c)
{

    uint8_t dots;
    if (c >= 'A' && c <= 'Z')
    {
        c &= 0x1F; // A-Z maps to 1-26
    }
    else if (c >= 'a' && c <= 'z')
    {
        c = (c - 'a') + 41; // A-Z maps to 41-67
    }
    else if (c >= '0' && c <= '9')
    {
        c = (c - '0') + 31;
    }
    else if (c == ' ')
    {
        c = 0; // space
    }
    else if (c == '.')
    {
        c = 27; // full stop
    }
    else if (c == '\'')
    {
        c = 28; // single quote mark
    }
    else if (c == ':')
    {
        c = 29; // clock_mode selector arrow
    }
    else if (c == '>')
    {
        c = 30; // clock_mode selector arrow
    }
    else if (c >= -80 && c <= -67)
    {
        c *= -1;
    }

    for (uint8_t col = 0; col < 5; col++)
    {
        dots = pgm_read_byte_near(&myfont[c][col]);
        for (uint8_t row = 0; row < 7; row++)
        {
            if (dots & (64 >> row))
            { // only 7 rows.
                plot(x + col, y + row, 1);
            }
            else
            {
                plot(x + col, y + row, 0);
            }
        }
    }
}

void Set_Clock()
{
    if (Alarm_clock.Alarm_clock_state == 1)
    {
        Serial.println("alam Clock set");
        if (digitalRead(PIN_B) == LOW && millis() - Timer.timermin1 > 300)
        {
            Timer.timermin1 = millis();
            Alarm_clock.min1A++;
        }
        if (Alarm_clock.min1A > '9')
        {
            Alarm_clock.min1A = '0';
        }

        if (digitalRead(PIN_C) == LOW && millis() - Timer.timermin2 > 300)
        {
            Timer.timermin2 = millis();
            Alarm_clock.min2A++;
            if (Alarm_clock.min2A > '5')
            {
                Alarm_clock.min2A = '0';
            }
        }
        if (digitalRead(PIN_D) == LOW && millis() - Timer.timerhr1 > 300)
        {
            Timer.timerhr1 = millis();
            Alarm_clock.hr1A++;
            if (Alarm_clock.hr1A > '9')
            {
                Alarm_clock.hr1A = '0';
                Alarm_clock.hr2A++;
            }
            if (Alarm_clock.hr1A > '9' && Alarm_clock.hr2A < '2')
            {
                Alarm_clock.hr1A = '0';
                Alarm_clock.hr2A++;
            }
            if (Alarm_clock.hr2A > '1' && Alarm_clock.hr1A > '3')
            {
                Alarm_clock.hr1A = '0';
                Alarm_clock.hr2A = '0';
            }
        }
        if (digitalRead(PIN_E) == LOW && millis() - Timer.timeralarm > 1000)
        {
            clear_display();
            Timer.timeralarm = millis();
            Timer.timerClockSet = millis();
            Essential.s = 0;
            Alarm_clock.set_click = 1;
            delay(1000);
        }
    }

    if (Clock_count_down.alam == 1)
    {
        Serial.println("Count down clock set");
        if (digitalRead(PIN_B) == LOW && millis() - Timer.timermin1 > 300)
        {
            Clock_count_down.state_setC = 1;
            Timer.timermin1 = millis();
            Clock_count_down.min1C++;
        }
        if (Clock_count_down.min1C > '9')
        {
            Clock_count_down.min1C = '0';
        }

        if (digitalRead(PIN_C) == LOW && millis() - Timer.timermin2 > 300)
        {
            Clock_count_down.state_setC = 1;
            Timer.timermin2 = millis();
            Clock_count_down.min2C++;
            if (Clock_count_down.min2C > '5')
            {
                Clock_count_down.min2C = '0';
            }
        }
        if (digitalRead(PIN_D) == LOW && millis() - Timer.timerhr1 > 300)
        {
            Clock_count_down.state_setC = 1;
            Timer.timerhr1 = millis();
            Clock_count_down.hr1C++;
            if (Clock_count_down.hr1C > '9')
            {
                Clock_count_down.hr1C = '0';
                Clock_count_down.chk_hrC = 1;
            }
            if (Clock_count_down.chk_hrC == 1)
            {
                Clock_count_down.hr2C++;
                Clock_count_down.chk_hrC = 0;
            }
            if (Clock_count_down.hr2C > '1' && Clock_count_down.hr1C > '3')
            {
                Clock_count_down.hr2C = '0';
                Clock_count_down.hr1C = '0';
            }
        }
        if (digitalRead(PIN_E) == LOW && millis() - Timer.timerClockSet > 1000)
        {
            Timer.timerClockSet = millis();
            clear_display();
            Clock_count_down.alam = 0;
            Clock_count_down.alam_show = 1;
            Count_down.hr2 = Clock_count_down.hr2C;
            Count_down.hr1 = Clock_count_down.hr1C;
            Count_down.min2 = Clock_count_down.min2C;
            Count_down.min1 = Clock_count_down.min1C;
            //Essential.s = 0;
            Serial.println("count down");

            Clock_count_down.hr2C = '0';
            Clock_count_down.hr1C = '0';
            Clock_count_down.min2C = '0';
            Clock_count_down.min1C = '0';

            Timer.Count_down_state = 1;
        }
    }
    //------------------------------------------------------
    if (digitalRead(PIN_B) == LOW && millis() - Timer.timermin1 > 300)
    {
        Clock_set.state_set = 1;
        Timer.timermin1 = millis();
        Clock_set.min1_set++;
        if (Essential.show == 1)
        {
            Serial.print(Clock.min1);
        }

        if (Clock_set.min1_set > '9')
        {
            Clock_set.min1_set = '0';
        }
    }

    if (digitalRead(PIN_C) == LOW && millis() - Timer.timermin2 > 300)
    {
        Clock_set.state_set = 1;
        Timer.timermin2 = millis();
        Clock_set.min2_set++;
        if (Essential.show == 1)
        {
            Serial.print(Clock.min2);
        }

        if (Clock_set.min2_set > '5')
        {
            Clock_set.min2_set = '0';
        }
    }
    if (digitalRead(PIN_D) == LOW && millis() - Timer.timerhr1 > 300)
    {
        Clock_set.state_set = 1;
        Timer.timerhr1 = millis();
        Clock_set.hr1_set++;
        if (Clock_set.hr1_set > '9')
        {
            Clock_set.hr1_set = '0';
            Clock_set.chk_hr = 1;
        }
        if (Clock_set.chk_hr == 1)
        {
            Clock_set.hr2_set++;
            Clock_set.chk_hr = 0;
        }
        if (Clock_set.hr2_set > '1' && Clock_set.hr1_set > '3')
        {
            Clock_set.hr2_set = '0';
            Clock_set.hr1_set = '0';
        }
    }
    if (digitalRead(botsetd) == 0 && millis() - Timer.timersetd > 300 && Alarm_clock.Beep_state == 0)
    {
        Timer.timersetd = millis();

        Clock_set.state_set = 3;
        Clock_set.day_set++;
        if (Clock_set.day_set == 8)
        {
            Clock_set.day_set = 1;
        }
        Serial.println(Clock_set.day_set);

        clear_display();
    }
    if (digitalRead(botsetd) == 0 && millis() - Timer.timersetd > 300 && Alarm_clock.Beep_state == 1)
    {
        Timer.timersetd = millis();
        Alarm_clock.Beep_state = 0;
        Alarm_clock.alarm_work = 0;
    }

    //------Clock_count_down.alam

    if (digitalRead(PIN_E) == LOW && millis() - Timer.timerClockSet > 1000)
    {
        Serial.println("Clock set");
        Clock_set.state_set = 0;
        Timer.timerClockSet = millis();
        clear_display();
        delay(10);
        Clock.min1 = Clock_set.min1_set;
        Clock.min2 = Clock_set.min2_set;
        Clock.hr1 = Clock_set.hr1_set;
        Clock.hr2 = Clock_set.hr2_set;

        Clock.t1 = '0';
        Clock.t2 = '0';
        Clock.day = Clock_set.day_set;
        if (Essential.show == 1)
        {
            Serial.print("-------------press----------");
        }
        Clock_set.hr1_set = '0';
        Clock_set.hr2_set = '0';
        Clock_set.min1_set = '0';
        Clock_set.min2_set = '0';
        Clock_set.day_set = 1;
        Essential.s = 0;
    }
}

void Show_time(char hr2_f, char hr1_f, char min2_f, char min1_f)
{
    char timer[5] = {hr2_f, hr1_f, ':', min2_f, min1_f};
    uint8_t i = 0;
    for (int j = 0; j < timer[i]; j++)
    {
        print_normal_char((i * 6) + 2, 1, timer[i]);

        i++;
    }
    i = 0;
}

void Show_day(int day_f)
{
    if (day_f == 1)
    {
        while (sun[d])
        {
            print_normal_char((d * 8) + 4, 1, sun[d]);
            d++;
        }
    }
    if (day_f == 2)
    {
        while (mon[d])
        {
            print_normal_char((d * 8) + 4, 1, mon[d]);
            d++;
        }
    }
    if (day_f == 3)
    {
        while (tue[d])
        {
            print_normal_char((d * 8) + 4, 1, tue[d]);
            d++;
        }
    }
    if (day_f == 4)
    {
        while (wed[d])
        {
            print_normal_char((d * 8) + 4, 1, wed[d]);
            d++;
        }
    }
    if (day_f == 5)
    {
        while (the[d])
        {
            print_normal_char((d * 8) + 4, 1, the[d]);
            d++;
        }
    }
    if (day_f == 6)
    {
        while (fri[d])
        {
            print_normal_char((d * 8) + 4, 1, fri[d]);
            d++;
        }
    }
    if (day_f == 7)
    {
        while (st[d])
        {
            print_normal_char((d * 8) + 4, 1, st[d]);
            d++;
        }
    }
    if (day_f == 8)
    {
        while (sun[d])
        {
            print_normal_char((d * 8) + 4, 1, sun[d]);
            d++;
        }
    }

    d = 0;
}

void setup()
{
    Serial.begin(9600);
    int devices = lc.getDeviceCount(); // find no of LED modules
    for (int address = 0; address < devices; address++)
    { // set up each device
        lc.shutdown(address, false);
        lc.setIntensity(address, 8);
        lc.clearDisplay(address);
    }
    // initialize switch
    pinMode(bot1, INPUT_PULLUP);
    pinMode(bot2, INPUT_PULLUP);
    pinMode(botcoc, INPUT_PULLUP);
    pinMode(PIN_B, INPUT_PULLUP);
    pinMode(PIN_C, INPUT_PULLUP);
    pinMode(PIN_D, INPUT_PULLUP);
    pinMode(PIN_A, INPUT_PULLUP);
    pinMode(PIN_E, INPUT_PULLUP);
    // initialize timer1
    noInterrupts(); // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    Timer.timer1_counter = 3036;  // preload timer 65536-16MHz/256/1Hz
    TCNT1 = Timer.timer1_counter; // preload timer
    TCCR1B |= (1 << CS12);        // 256 prescaler
    TIMSK1 |= (1 << TOIE1);       // enable timer overflow interrupt
    interrupts();                 // enable all interrupts
}

ISR(TIMER1_OVF_vect) // interrupt service routine
{
    TCNT1 = Timer.timer1_counter; // preload timer
    //--------------Set Clock ------------------------------
    Clock.t1++;
    if (Clock.t1 > '9')
    {
        Clock.t2++;
        Clock.t1 = '0';
    }

    if (Clock.t2 > '5')
    {
        Clock.t2 = '0';
        Clock.min1++;
    }

    if (Clock.min1 > '9')
    {
        Clock.min1 = '0';
        Clock.min2++;
    }

    if (Clock.min2 > '5')
    {
        Clock.min2 = '0';
        Clock.hr1++;
    }
    if (Clock.hr1 > '9' && Clock.hr2 < '2')
    {
        Clock.hr1 = '0';
        Clock.hr2++;
    }
    if (Clock.hr2 > '1' && Clock.hr1 > '3')
    {
        Clock.hr1 = '0';
        Clock.hr2 = '0';
        Clock.day++;
        Clock.day_count++;
    }
    if (Clock.day > 7)
    {
        Clock.day = 1;
    }
    if (Clock.day_count > 31)
    {
        Clock.day_count = 1;
        Clock.month++;
    }
    if (Clock.month > 12)
    {
        Clock.month = 1;
    }

    if (Timer.Count_down_state == 1)
    {
        Count_down.sec1--;
        if (Count_down.sec1 < '0')
        {
            Count_down.sec1 = '9';
            Count_down.sec2--;
        }
        if (Count_down.sec2 < '0')
        {
            Count_down.sec2 = '5';
            Count_down.sec1 = '9';
            Count_down.min1--;
        }
        if (Count_down.min1 < '0')
        {
            Count_down.min1 = '9';
            Count_down.min2--;
        }
        if (Count_down.min2 < '0')
        {
            Count_down.min2 = '5';
            Count_down.min1 = '9';
            Count_down.hr1--;
        }
        if (Count_down.hr1 < '0')
        {
            Count_down.hr1 = '9';
            Count_down.hr2--;
        }
        if (Count_down.hr2 < '0')
        {
            Count_down.hr1 = '3';
            Count_down.hr2 = '2';
        }
        if (Count_down.hr2 == '0' && Count_down.hr1 == '0' && Count_down.min2 == '0' && Count_down.min1 == '0' && Count_down.sec2 == '0' && Count_down.sec1 == '0')
        {
            Clock_count_down.beep_sound = 1;
            Clock_count_down.alam_show = 0;
            Timer.Count_down_state = 0;
        }

        Serial.print("  ");
        Serial.print(Count_down.hr2);
        Serial.print("  ");
        Serial.print(Count_down.hr1);
        Serial.print("  ");
        Serial.print("  ");
        Serial.print(Count_down.min2);
        Serial.print("  ");
        Serial.print(Count_down.min1);
        Serial.print("  ");
        Serial.print(Count_down.sec2);
        Serial.print("  ");
        Serial.print(Count_down.sec1);
        Serial.println(" ");
    }
    //-----
}

void loop()
{
    bool setmin = digitalRead(PIN_B);
    bool sethr = digitalRead(PIN_C);
    bool setday = digitalRead(PIN_D);
    bool setmonth = digitalRead(PIN_A);
    bool stop = digitalRead(PIN_E);
    //State Show Time
    if (FSM[Essential.s].out == 0)
    {
        //-Show Time
        Alarm_clock.Alarm_clock_state = 0;
        Set_Clock();
        Serial.println("coc ");
        if (Essential.show == 1)
        {
            Serial.println("Time");
        }

        if (Clock_set.state_set == 0)
        {
            Show_time(Clock.hr2, Clock.hr1, Clock.min2, Clock.min1);
        }
        else if (Clock_set.state_set == 1)
        {
            Show_time(Clock_set.hr2_set, Clock_set.hr1_set, Clock_set.min2_set, Clock_set.min1_set);
        }
        else
        {
            delay(100);
            Show_day(Clock_set.day_set);
            delay(100);
        }

        if (Clock.hr2 == Alarm_clock.hr2A && Clock.hr1 == Alarm_clock.hr1A && Clock.min2 == Alarm_clock.min2A && Clock.min1 == Alarm_clock.min1A && Alarm_clock.alarm_work == 1)
        {

            Alarm_clock.Beep_state = 1;
        }
        else
        {
            Serial.println("Beep state  = 0");
        }

        if (Alarm_clock.Beep_state == 1)
        {
            Serial.println("Beep");
        }

        //Show Time
    }

    //State Clear
    if (FSM[Essential.s].out % 2 != 0)
    {
        if (Essential.show == 1)
        {
            Serial.println("clear");
        }
        Serial.println("clear");
        clear_display();
        delay(1000);
        Clock_count_down.alam = 0;
        Clock_count_down.alam_show = 0;
        Timer.Count_down_state = 0;
        Alarm_clock.Alarm_clock_state = 0;
    }

    //State show Day
    if (FSM[Essential.s].out == 2)
    {
        Serial.println("DM");
        Show_day(Clock.day);
        delay(1000);
        clear_display();
        Essential.s = 0;
    }

    if (FSM[Essential.s].out == 4)
    {
        Serial.println("alam");
        Clock_count_down.alam = 1;
        Set_Clock();
        if (Clock_count_down.alam_show == 0)
        {
            Show_time(Clock_count_down.hr2C, Clock_count_down.hr1C, Clock_count_down.min2C, Clock_count_down.min1C);
        }
        else
        {
            Show_time(Count_down.hr2, Count_down.hr1, Count_down.min2, Count_down.min1);
        }

        if (Clock_count_down.beep_sound == 1)
        {
            //beep
            Serial.println("Beep");
            delay(1000);
            Clock_count_down.beep_sound = 0;
        }
        //show
    }

    //------------------count----------------------------------------
    if (FSM[Essential.s].out == 6)
    {
        Serial.println("count");
        Alarm_clock.Alarm_clock_state = 1;
        Set_Clock();
        Show_time(Alarm_clock.hr2A, Alarm_clock.hr1A, Alarm_clock.min2A, Alarm_clock.min1A);
        delay(1000);
    }
    //State count
    delay(FSM[Essential.s].Time);
    //Input from switch
    Essential.input1 = !digitalRead(bot1);
    Essential.input2 = !digitalRead(bot2);
    Essential.input = (Essential.input1 * 2) + Essential.input2;
    Essential.s = FSM[Essential.s].next[Essential.input];
    Serial.print("    ");
    Serial.print(Essential.s);
    Serial.print("    ");
    if (Essential.show == 1)
    {
        Serial.print(Essential.input1);
        Serial.print("   ");
        Serial.print(Essential.input2);
        Serial.print("------------");
        Serial.print(Essential.s);
        Serial.print("   ");
        Serial.print(Essential.input);
        Serial.println("   ");
    }
    //---------------
}
//hey fai
//test por
