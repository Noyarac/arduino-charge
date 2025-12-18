#include "Led.h"

void Led::update() {
    uint8_t byte_color = 0;
    byte counter[3] = {0, 0, 0};
    
    for (byte sample = 0; sample < SAMPLE; sample++) {
        delay(1);
        for (byte i = 0; i < 3; i++) {
            if (pin == LED5_PIN) {
                if (!digitalRead(COLOR_PIN[i]) && digitalRead(pin)) {
                    counter[i]++;
                }
            } else {
                if (!digitalRead(COLOR_PIN[i]) && analogRead(pin) > THRESHOLD_SENSOR) {
                    counter[i]++;
                }
            }
        }
    }
    
    for (byte i = 0; i < 3; i++) {
        byte_color <<= 1;
        if (counter[i] > THRESHOLD_FREQUENCY) {
            byte_color |= 0b00000001;
        }
    }
    
    color = Color(byte_color);
}
