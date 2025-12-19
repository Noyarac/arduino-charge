// Arduino Charge Controller
// Refactored version with separated classes

#include <RTClib.h>
#include <Wire.h>
#include "config.h"
#include "Led.h"
#include "Led_Bar.h"
#include "Interface.h"
#include "Button.h"

// Configuration variables
bool ADJUST_RTC = false;
DateTime adj(2025, 12, 19, 16, 49, 0);

// Constants definitions
const byte SAMPLE = 60;
const byte THRESHOLD_FREQUENCY = SAMPLE / 6;
const byte THRESHOLD_SENSOR = 140;
const int BLINKING_DELAY = 500;
const int FLOWING_DELAY = 700;

const Pin COLOR_PIN[3] = {RED_PIN, GREEN_PIN, BLUE_PIN};
const Pin LED_PIN[5] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN};
const Pin OUT_PIN[4] = {OUT_RED_PIN, OUT_GREEN_PIN, OUT_BLUE_PIN, OUT_BLINKING_PIN};
const Pin RTC_PIN[2] = {RTC_SDA_PIN, RTC_SCL_PIN};

// Alarm configuration
byte ALARM_ON_HOUR = 22;
byte ALARM_ON_MINUTE = 0;
byte ALARM_OFF_HOUR = 6;
byte ALARM_OFF_MINUTE = 0;
int ALARM_ON_TIME = 60 * ALARM_ON_HOUR + ALARM_ON_MINUTE;
int ALARM_OFF_TIME = 60 * ALARM_OFF_HOUR + ALARM_OFF_MINUTE;

// Human-readable labels
char* hr_state[] = {"AVAILABLE", "TIMED", "CHARGING", "OVER", "MENU_SELECT_CURRENT", 
                    "MENU_SELECT_SCHEDULE", "MENU_CURRENT", "MENU_SCHEDULE", "ERROR"};
char* hr_color[] = {"BLACK", "BLUE", "GREEN", "CYAN", "RED", "MAGENTA", "YELLOW", "WHITE"};
char* hr_display[] = {"SOLID", "BLINKING", "FLOWING"};

// Global objects
DS3231 rtc;
Led_Bar led_bar;
Interface interface(LED);
Button button(BTN_PIN);

// Runtime variables
DateTime now;
DateTime summer_begin;
DateTime summer_end;
int time_sum;
bool in_charging_hours;

// Error handler function
void error() {
    interface.error();
    while (true) {
        delay(60000);
    }
}

void setup() {
    button.setup();
    for (Pin pin : LED_PIN) {
        pinMode(pin, INPUT);
    }
    for (Pin pin : COLOR_PIN) {
        pinMode(pin, INPUT_PULLUP);
    }
    for (Pin pin : RTC_PIN) {
        pinMode(pin, INPUT);
    }
    analogReference(EXTERNAL);
    if (interface.type_interface == USB) {
        Serial.begin(115200);
        interface.print("Début");
    } else {
        for (Pin pin : OUT_PIN) {
            pinMode(pin, OUTPUT);
        }
    }
    Wire.begin();
    rtc.begin();

    if (ADJUST_RTC) {
        Serial.println("Ajustement du RTC...");
        rtc.adjust(adj);
        delay(100);
    }

}

void loop() {
    now = rtc.now();
    summer_begin.setyear(now.year());
    summer_begin.setmonth(3);
    summer_begin.setday(31);
    summer_begin.setday(31 - summer_begin.dayOfWeek());
    summer_end.setyear(now.year());
    summer_end.setmonth(10);
    summer_end.setday(31);
    summer_end.setday(31 - summer_end.dayOfWeek());
    if (now.operator>=(summer_begin) && now.operator<=(summer_end)) {
        now.operator+=(3600);
    }
    time_sum = now.hour() * 60 + now.minute();
    led_bar.update();
    interface.output(led_bar, time_sum);
    if (ALARM_ON_TIME < ALARM_OFF_TIME) {
        in_charging_hours = (ALARM_ON_TIME <= time_sum && time_sum < ALARM_OFF_TIME);
    } else {
        in_charging_hours = ((time_sum > ALARM_ON_TIME) || (time_sum < ALARM_OFF_TIME));
    }

    if (led_bar.state == ERROR) {error();}
    if (!in_charging_hours && led_bar.state == AVAILABLE) {
        button.from_available_to_timed();
    } else
    if (!in_charging_hours && led_bar.state == TIMED) {
        button.from_timed_to_timed();
    } else
    if (!in_charging_hours && led_bar.state == CHARGING) {
        button.from_charging_to_available();
    } else
    if (!in_charging_hours && led_bar.state == OVER && time_sum > 9 * 60) {
        button.from_over_to_available();button.from_available_to_timed();
    } else
    if (in_charging_hours && led_bar.state == AVAILABLE) {
        button.from_available_to_timed(); delay(20000); button.from_timed_to_available();
    } else
    if (in_charging_hours && led_bar.state == TIMED) {
        button.from_timed_to_available();
    } else
    if (in_charging_hours && led_bar.state == OVER) {
        button.from_over_to_available();;
    } 

    delay(60000);
}