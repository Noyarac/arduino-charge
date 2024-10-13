enum Pin {
    RED_PIN = A0,
    GREEN_PIN = A1,
    BLUE_PIN = A2,
    LED1_PIN = 2,
    LED2_PIN = 3,
    LED3_PIN = 4,
    LED4_PIN = 5,
    LED5_PIN = 6
};
const Pin COLOR_PIN[] = {RED_PIN, GREEN_PIN, BLUE_PIN};
const Pin LED_PIN[] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN};

const byte SAMPLE = 100;
const int BLINKING_DELAY = 100;

enum State {
    AVAILABLE, TIMED, CHARGING, OVER, MENU_ROOT, MENU_CURRENT, MENU_SCHEDULE
};

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


String hr_color(Color color) {
    switch (color) {
        case BLACK: return "BLACK";
        case WHITE: return "WHITE";
        case MAGENTA: return "MAGENTA";
        case YELLOW: return "YELLOW";
        case RED: return "RED";
        case CYAN: return "CYAN";
        case GREEN: return "GREEN";
        case BLUE: return "BLUE";
        default: return "PROBLEM";
    }
}

enum Display {
    SOLID,
    BLINKING,
    FLOWING
};



enum Pass {
    OLD, NEW
};

class Led {
    public:
    Pin pin;
    Color color;
    
    void update() {
        uint8_t char_color = 0;
        byte counter[3] = {0, 0, 0};
        for (byte sample = 0; sample < SAMPLE; sample++) {
            delay(1);
            for (byte i = 0; i < 3; i++) {
                if (!digitalRead(COLOR_PIN[i]) && digitalRead(pin)) {counter[i]++;}
            }
        }
        for (byte i = 0; i < 3; i++) {
            char_color <<= 1;
            if (counter[i] > 5) {char_color |= 0b00000001;}
        }
        color = Color(char_color);
    }
};

class {
    public:
    Led leds[5] {Led {LED1_PIN}, Led {LED2_PIN}, Led {LED3_PIN}, Led {LED4_PIN}, Led {LED5_PIN}};
    Color old_leds_color;
    char old_leds_level;
    Color color;
    Display display;
    byte level;
    State state;

    void update() {
        // Update
        delay(BLINKING_DELAY);
        for (byte pass = 0; pass < 2; pass++) {
            if (Pass(pass) == NEW) {
                    old_leds_color = leds[0].color;
                    old_leds_level = level;
            }
            level = 5;
            for (byte i = 0; i < 5; i++) {
                leds[i].update();
                if (leds[i].color == BLACK) {level = i; break;}
            }
        }

        // Display
        Pass colored;
        if (leds[0].color != BLACK && leds[0].color == old_leds_color) {
            if (old_leds_level == level) {
                display == SOLID;
            } else {
                display == FLOWING;
            }
        } else
        if (leds[0].color != BLACK && old_leds_color == BLACK) {
            display = BLINKING;
            colored = NEW;
        } else
        if (leds[0].color == BLACK && old_leds_color != BLACK) {
            display = BLINKING;
            colored = OLD;
        } else {
            display = SOLID;
        }

        // Color
        if (colored == OLD) {
            color = old_leds_color;
            level = old_leds_level;
        } else {
            color = leds[0].color;
        }
    }
} led_bar, old_led_bar;

void setup() {
    analogReference(EXTERNAL);
    for (Pin pin : LED_PIN) {
        pinMode(pin, INPUT);
    }
    for (Pin pin : COLOR_PIN) {
        pinMode(pin, INPUT_PULLUP);
    }
    Serial.begin(115200);

    

}

void loop() {
    old_led_bar.color = led_bar.color;
    old_led_bar.display = led_bar.display;
    old_led_bar.level = led_bar.level;
    led_bar.update();
    if ((old_led_bar.color != led_bar.color) || (old_led_bar.display != led_bar.display) || (old_led_bar.level != led_bar.level) ) {
        Serial.println(hr_color(led_bar.color) + ", " + humano_diseplaybu(led_bar.display) + ", " + String(led_bar.level));
    }

}

String humano_diseplaybu(byte ddiiss) {
    switch (ddiiss) {
        case SOLID: return "SOLID";
        case BLINKING: return "BLINKING";
        case FLOWING: return "FLOWING";
        default: return "PROBLEM";
    }
}