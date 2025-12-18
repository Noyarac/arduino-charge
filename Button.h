#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "config.h"
#include "Led_Bar.h"
#include "Interface.h"

// Forward declaration
void error();

class Button {
public:
    Pin pin;
    
    Button(Pin p);
    void setup();
    void press(Button_press press_time);
    bool bug_fix(Led_Bar lb, State targ_state, byte targ_level);
    bool navigate(State targeted_states[], byte targ_state_len, 
                  byte targeted_levels[], byte targ_level_len, 
                  Button_press sequence[], byte seq_len);
    void from_available_to_timed();
    void from_timed_to_available();
    void from_charging_to_available();
    void from_over_to_available();
    void from_timed_to_timed();
};

#endif // BUTTON_H
