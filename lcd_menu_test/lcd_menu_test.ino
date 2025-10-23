/*
    A sketch to test whether a user interactable menu can be implemented with
    the rotary encoder, a push button, and the LCD.
*/

#include <Wire.h>       // i2c library
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "Rotary.h"
#include "Button.h"


hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip
Rotary rotary = Rotary(3, 4); // declare rotary object, init pins as inputs aswell
Button back_but;
Button sel_but;     //objects for handling button debouncing.

typedef enum {
    off,
    start,
    name,
    date,
    error
}   State;
State curr_state = off;

typedef enum Input {    // combines rotary encoder and buttons together
    none,
    select,
    back,
    cw = DIR_CW,     // for rotary encoder
    ccw = DIR_CCW    // for rotary encoder
};


void setup() {
    Serial.begin(57600);
    // select button on encoder
    sel_but.begin(2, false);    // no pullup

    // back button
    back_but.begin(5, true);    // use pullup

    // init LCD
    int status;
    status = lcd.begin(20, 4);   // 20 columns 4 rows size screen
    if(status) {    // non zero status means it was unsuccesful
            // hd44780 has a fatalError() routine that blinks an led if possible
            // begin() failed so blink error code using the onboard LED if possible
            hd44780::fatalError(status); // does not return
    }
    lcd.off();
}

void loop() {
    Input input = none;

    
    // get user input assuming only one pressed at a time.
    if(sel_but.process()) {  // if select pressed
        input = select;
    }
    else if(back_but.process()) {    // if back pressed
        input = back;
    }
    else {
        input = static_cast<Input>(rotary.process());       // if rotary encoder moved
        // returns none by default
    }
    if(input != none) Serial.println(input);


    
    State next_state = curr_state;      // if all switch statements below fail at least we stay in the same state.
    switch(curr_state) {
        case off:
            switch(input) {
                case select:
                    next_state = start;
                    lcd.on();
                    lcd.home();
                    lcd.print("Start?");
                    lcd.setCursor(0,1);
                    lcd.print("Y   N");
                    lcd.setCursor(0,1);
                    lcd.cursor();
                break;
            }
        break;
        case start:
            switch(input) {
                case select:
                    Serial.println("Select");
                    unsigned char ch = lcd.read();
                    if(ch=='Y') {
                        next_state = name;
                        lcd.clear();
                        lcd.noCursor();
                        lcd.print("Name");
                    }
                    else if(ch=='N') {
                        next_state = off;
                        lcd.clear();
                        lcd.noCursor();
                        lcd.off();
                    }
                    else {
                        next_state = error;
                        lcd.clear();
                        lcd.noCursor();
                        lcd.print("ERROR");
                    }
                break;
                case cw:
                    Serial.println("CW");
                    lcd.setCursor(4,1); // Y
                break;
                case ccw:
                    Serial.println("CCW");
                    lcd.setCursor(0,1); // N
                break;
                case back:
                    next_state = off;
                    lcd.clear();
                    lcd.noCursor();
                    lcd.off();
                break;
            }
        break;
        case name:

        break;

        case date:

        break;
    }
    curr_state = next_state;
    
}







