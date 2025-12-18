#ifndef LED_BAR_H
#define LED_BAR_H

#include <Arduino.h>
#include "config.h"
#include "Led.h"

class Led_Bar {
public:
    Led leds[5];
    Color old_leds_color;
    byte old_leds_level;
    Color medium_leds_color;
    byte medium_leds_level;
    Color color;
    Display display;
    byte level;
    State state;
    
    Led_Bar();
    void update();
};

#endif // LED_BAR_H
