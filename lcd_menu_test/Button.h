/*
    Library that encapsulates all the software debouncing of a simple push
    button. It assumes that the button uses the arduino's internal pullup 
    resistor and is an active LOW button. The pinMode() function is called
    in the classes constructor. Debouncing only detects falling edges which
    reduces the processing time needed when the button is idle.
 */

#ifndef button_h
#define button_h

#include "Arduino.h"

class Button {
    public:
        Button();
        void begin(uint8_t _pin, bool use_pullup = true);

        // to be repetitvely called in main loop
        // returns 1 when button is pressed
        unsigned char process();
    private:
        unsigned char pin;  // pin number on arduino PCB to which button is connected.
        unsigned char prev_read;
        unsigned char stable_state; // becomes 0 when pressed
        unsigned long last_debounce_time;
};


#endif