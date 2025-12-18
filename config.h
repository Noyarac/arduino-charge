#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// RTC Configuration
extern bool ADJUST_RTC;
extern DateTime adj;

// Alarm Configuration
extern byte ALARM_ON_HOUR;
extern byte ALARM_ON_MINUTE;
extern byte ALARM_OFF_HOUR;
extern byte ALARM_OFF_MINUTE;
extern int ALARM_ON_TIME;
extern int ALARM_OFF_TIME;

// Pin Definitions
enum Pin {
    BTN_PIN = 0,
    RTC_SDA_PIN = A4,
    RTC_SCL_PIN = A5,
    RED_PIN = 13,
    GREEN_PIN = 12,
    BLUE_PIN = 11,
    LED1_PIN = A0,
    LED2_PIN = A1,
    LED3_PIN = A2,
    LED4_PIN = A3,
    LED5_PIN = 1,
    OUT_RED_PIN = 5,
    OUT_GREEN_PIN = 6,
    OUT_BLUE_PIN = 7,
    OUT_BLINKING_PIN = 8
};

// Sampling and Threshold Constants
extern const byte SAMPLE;
extern const byte THRESHOLD_FREQUENCY;
extern const byte THRESHOLD_SENSOR;
extern const int BLINKING_DELAY;
extern const int FLOWING_DELAY;

// Pin Arrays
extern const Pin COLOR_PIN[];
extern const Pin LED_PIN[];
extern const Pin OUT_PIN[];
extern const Pin RTC_PIN[];

// State Enumeration
enum State {
    AVAILABLE, 
    TIMED, 
    CHARGING, 
    OVER, 
    MENU_SELECT_CURRENT, 
    MENU_SELECT_SCHEDULE, 
    MENU_CURRENT, 
    MENU_SCHEDULE, 
    ERROR, 
    UNKNOWN
};
extern char* hr_state[];

// Color Enumeration
enum Color {
    BLACK = 0b00000000,
    WHITE = 0b00000111,
    YELLOW = 0b00000110,
    MAGENTA = 0b00000101,
    RED = 0b00000100,
    CYAN = 0b00000011,
    GREEN = 0b00000010,
    BLUE = 0b00000001
};
extern char* hr_color[];

// Display Mode Enumeration
enum Display {
    SOLID,
    BLINKING,
    FLOWING
};
extern char* hr_display[];

// Pass Enumeration
enum Pass {
    OLD, 
    MEDIUM, 
    NEW
};

// Interface Type Enumeration
enum Type_Interface {
    USB, 
    BREADBOARD, 
    LED, 
    NONE
};

// Button Press Enumeration
enum Button_press {
    SHORT = 35, 
    EDGE = 60, 
    LONG = 1700
};

#endif // CONFIG_H
