#include "Interface.h"

Interface::Interface(Type_Interface type) {
    type_interface = type;
}

void Interface::print(char message[]) {
    if (type_interface == USB) {
        Serial.println(message);
    }
}

void Interface::error() {
    switch (type_interface) {
        case USB:
            Serial.println("Error");
            break;
        case BREADBOARD:
        case LED:
            while (true) {
                digitalWrite(OUT_BLINKING_PIN, HIGH);
                delay(500);
                digitalWrite(OUT_BLINKING_PIN, LOW);
                delay(500);
            }
        case NONE:
            break;
    }
}

void Interface::output(Led_Bar led_bar, int time) {
    switch (type_interface) {
        case USB: {
            char answer[50];
            sprintf(answer, "%d:%d   %s (%s, %s, %d)", 
                    time/60, time%60, 
                    hr_state[led_bar.state], 
                    hr_color[led_bar.color], 
                    hr_display[led_bar.display], 
                    led_bar.level);
            Serial.println(answer);
        }
        case BREADBOARD: {
            byte mask = 0b00000100;
            byte couleur = static_cast<byte>(led_bar.color);
            for (byte pin : OUT_PIN) {
                if (couleur & mask) {
                    digitalWrite(pin, HIGH);
                } else {
                    digitalWrite(pin, LOW);
                }
                mask >>= 1;

                if (led_bar.display == BLINKING) {
                    digitalWrite(OUT_BLINKING_PIN, HIGH);
                } else {
                    digitalWrite(OUT_BLINKING_PIN, LOW);
                }
            }
            break;
        }
        case LED:
            digitalWrite(OUT_BLINKING_PIN, HIGH);
            delay(200);
            digitalWrite(OUT_BLINKING_PIN, LOW);
            break;
        case NONE:
            break;
    }
}
