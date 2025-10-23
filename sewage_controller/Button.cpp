#include "Arduino.h"
#include "Button.h"

Button::Button()
    : pin(255),
        stable_state(HIGH),
        prev_read(HIGH),
        last_debounce_time(0)
        {
}

void Button::begin(uint8_t _pin, bool use_pullup) {
    pin = _pin;
    stable_state = 1;   // not pressed
    last_debounce_time = 0;
    if(use_pullup) pinMode(pin, INPUT_PULLUP);
    else pinMode(pin, INPUT);
    prev_read = digitalRead(pin);
}

// returns 1 when button is pressed (0 when not pressed). Assumes button
// is active LOW. So this function only looks for a falling edge.
unsigned char Button::process() {
    unsigned char reading = digitalRead(pin);

    // Detect only falling edge: HIGH -> LOW
    if(prev_read == HIGH && reading == LOW) {
        last_debounce_time = millis();
    }
    prev_read = reading;

    // Only check debounce if button is currently LOW (potential press)
    if(reading == LOW) {
        if((millis() - last_debounce_time) > 50) {
            if(stable_state != LOW) {
                stable_state = LOW;
                return true;  // valid press detected
            }
        }
    } else {
        // Reset stable state when released
        stable_state = HIGH;
    }

    return false;   // not pressed
}

