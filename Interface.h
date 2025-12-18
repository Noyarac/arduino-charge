#ifndef INTERFACE_H
#define INTERFACE_H

#include <Arduino.h>
#include "config.h"
#include "Led_Bar.h"

class Interface {
public:
    Type_Interface type_interface;
    
    Interface(Type_Interface type);
    void print(char message[]);
    void error();
    void output(Led_Bar led_bar, int time);
};

#endif // INTERFACE_H
