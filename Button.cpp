#include "Button.h"

// External references
extern Led_Bar led_bar;
extern Interface interface;

Button::Button(Pin p) {
    pin = p;
}

void Button::setup() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void Button::press(Button_press press_time) {
    int ttp;
    if (press_time == SHORT) {
        ttp = 300;
    } else if (press_time == LONG) {
        ttp = 1700;
    }
    
    digitalWrite(pin, LOW);
    delay(ttp);
    digitalWrite(pin, HIGH);
    delay(100);
}

bool Button::bug_fix(Led_Bar lb, State targ_state, byte targ_level) {
    return (lb.state == MENU_CURRENT && lb.level == 2 && targ_state == MENU_SCHEDULE && targ_level == 1) ||
           (lb.color == WHITE && lb.level == 4 && lb.display == SOLID && targ_state == AVAILABLE) ||
           (lb.color == BLUE && lb.level == 4 && lb.display == 4 && targ_state == TIMED);
}

bool Button::navigate(State targeted_states[], byte targ_state_len, 
                      byte targeted_levels[], byte targ_level_len, 
                      Button_press sequence[], byte seq_len) {
    for (byte i = 0; i < seq_len; i++) {
        led_bar.update();
        if (interface.type_interface == USB) {
            Serial.println(String(i));
            Serial.println(hr_state[led_bar.state]);
            Serial.println(hr_state[targeted_states[i]]);
            Serial.println("bug fix dit " + String(bug_fix(led_bar, targeted_states[i], targeted_levels[i])));
            Serial.println();
        }
        if (!((led_bar.state == targeted_states[i]) || bug_fix(led_bar, targeted_states[i], targeted_levels[i]))) {
            return 1;
        }
        press(sequence[i]);
        delay(500);
    }
    return 0;
}

void Button::from_available_to_timed() {
    interface.print("From available to Timed");
    State TARGETED_STATES[] = {AVAILABLE, MENU_SELECT_CURRENT, MENU_SELECT_SCHEDULE, MENU_SCHEDULE, MENU_SCHEDULE};
    byte TARGETED_LEVELS[] = {5, 2, 5, 1, 2};
    Button_press SEQUENCE[] = {LONG, SHORT, LONG, SHORT, LONG};
    if (navigate(TARGETED_STATES, sizeof(TARGETED_STATES)/sizeof(State), 
                 TARGETED_LEVELS, sizeof(TARGETED_LEVELS)/sizeof(byte), 
                 SEQUENCE, sizeof(SEQUENCE)/sizeof(Button_press))) {
        error();
    }
}

void Button::from_timed_to_available() {
    interface.print("From Timed to Available");
    State TARGETED_STATES[] = {TIMED, MENU_SELECT_CURRENT, MENU_SELECT_SCHEDULE, MENU_SCHEDULE};
    byte TARGETED_LEVELS[] = {5, 2, 5, 1};
    Button_press SEQUENCE[] = {LONG, SHORT, LONG, LONG};
    if (navigate(TARGETED_STATES, sizeof(TARGETED_STATES)/sizeof(State), 
                 TARGETED_LEVELS, sizeof(TARGETED_LEVELS)/sizeof(byte), 
                 SEQUENCE, sizeof(SEQUENCE)/sizeof(Button_press))) {
        error();
    }
}

void Button::from_charging_to_available() {
    interface.print("From Charging to Over (or Available)");
    State TARGETED_STATES[] = {CHARGING};
    byte TARGETED_LEVELS[] = {0};
    Button_press SEQUENCE[] = {LONG};
    if (navigate(TARGETED_STATES, sizeof(TARGETED_STATES)/sizeof(State), 
                 TARGETED_LEVELS, sizeof(TARGETED_LEVELS)/sizeof(byte), 
                 SEQUENCE, sizeof(SEQUENCE)/sizeof(Button_press))) {
        error();
    } else {
        delay(6000);
    }
}

void Button::from_over_to_available() {
    interface.print("From Over to Available");
    State TARGETED_STATES[] = {OVER};
    byte TARGETED_LEVELS[] = {0};
    Button_press SEQUENCE[] = {LONG};
    if (navigate(TARGETED_STATES, sizeof(TARGETED_STATES)/sizeof(State), 
                 TARGETED_LEVELS, sizeof(TARGETED_LEVELS)/sizeof(byte), 
                 SEQUENCE, sizeof(SEQUENCE)/sizeof(Button_press))) {
        error();
    } else {
        delay(6000);
    }
}

void Button::from_timed_to_timed() {
    interface.print("From Timed to Timed");
    State TARGETED_STATES[] = {TIMED, MENU_SELECT_CURRENT, MENU_SELECT_SCHEDULE, MENU_SCHEDULE, MENU_SCHEDULE};
    byte TARGETED_LEVELS[] = {5, 2, 5, 1, 2};
    Button_press SEQUENCE[] = {LONG, SHORT, LONG, SHORT, LONG};
    if (navigate(TARGETED_STATES, sizeof(TARGETED_STATES)/sizeof(State), 
                 TARGETED_LEVELS, sizeof(TARGETED_LEVELS)/sizeof(byte), 
                 SEQUENCE, sizeof(SEQUENCE)/sizeof(Button_press))) {
        error();
    }
}
