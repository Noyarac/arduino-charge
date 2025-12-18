#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "config.h"

class Led {
public:
    Pin pin;
    Color color;
    
    void update();
};

#endif // LED_H
