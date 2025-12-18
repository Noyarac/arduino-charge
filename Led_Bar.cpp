#include "Led_Bar.h"

Led_Bar::Led_Bar() {
    leds[0].pin = LED1_PIN;
    leds[1].pin = LED2_PIN;
    leds[2].pin = LED3_PIN;
    leds[3].pin = LED4_PIN;
    leds[4].pin = LED5_PIN;
}

void Led_Bar::update() {
    // Update leds
    unsigned long entry = millis();
    
    for (byte pass = 0; pass < 3; pass++) {
        if (pass == 1) {
            while (millis() < (entry + BLINKING_DELAY)) {
                delay(1);
            }
            old_leds_color = leds[0].color;
            old_leds_level = level;
        }
        if (pass == 2) {
            while (millis() < (entry + FLOWING_DELAY)) {
                delay(1);
            }
            medium_leds_color = leds[0].color;
            medium_leds_level = level;
        }
        
        level = 5;
        for (byte i = 0; i < 5; i++) {
            leds[i].update();
            if (leds[i].color == BLACK) {
                level = i;
                break;
            }
        }
    }

    // Display
    Pass colored = NEW;
    
    if (leds[0].color == GREEN && (old_leds_color == BLACK || medium_leds_color == BLACK)) {
        display = FLOWING;
        colored = NEW;
    } else if (medium_leds_color == GREEN && (leds[0].color == BLACK || old_leds_color == BLACK)) {
        display = FLOWING;
        colored = MEDIUM;
    } else if (old_leds_color == GREEN && (medium_leds_color == BLACK || leds[0].color == BLACK)) {
        display = FLOWING;
        colored = OLD;
    } else if ((leds[0].color == GREEN && old_leds_color == GREEN && medium_leds_color == GREEN) && 
               (level != old_leds_level || level != medium_leds_level)) {
        display = FLOWING;
        colored = NEW;
    } else if ((leds[0].color == BLUE || leds[0].color == RED) && medium_leds_color == BLACK) {
        display = BLINKING;
        colored = NEW;
    } else if (leds[0].color == BLACK && (medium_leds_color == BLUE || medium_leds_color == RED)) {
        display = BLINKING;
        colored = MEDIUM;
    } else if ((medium_leds_color == BLUE || medium_leds_color == RED) && old_leds_color == BLACK) {
        display = BLINKING;
        colored = MEDIUM;
    } else if ((old_leds_color == BLUE || old_leds_color == RED) && medium_leds_color == BLACK) {
        display = BLINKING;
        colored = OLD;
    } else {
        display = SOLID;
    }

    // Color
    if (colored == OLD) {
        color = old_leds_color;
        level = old_leds_level;
    } else if (colored == MEDIUM) {
        color = medium_leds_color;
        level = medium_leds_level;
    } else {
        color = leds[0].color;
    }

    // State
    if (color == YELLOW) {
        state = ERROR;
    } else if (color == WHITE && display == SOLID && level == 5) {
        state = AVAILABLE;
    } else if (color == BLUE && display == SOLID && level == 5) {
        state = TIMED;
    } else if (color == GREEN && display == FLOWING) {
        state = CHARGING;
    } else if (color == GREEN && display == SOLID && level == 5) {
        state = OVER;
    } else if (color == RED && display == BLINKING) {
        state = MENU_SELECT_CURRENT;
    } else if (color == BLUE && display == BLINKING) {
        state = MENU_SELECT_SCHEDULE;
    } else if (color == RED && display == SOLID && level < 5) {
        state = MENU_CURRENT;
    } else if (color == BLUE && display == SOLID) {
        state = MENU_SCHEDULE;
    } else if (color == BLACK && display == SOLID) {
        state = CHARGING;
    } else {
        state = UNKNOWN;
    }
}
