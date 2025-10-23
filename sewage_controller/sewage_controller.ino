#include <Wire.h>       // i2c library
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "RTClib.h"
#include "Button.h"
#include "Rotary.h"


// Globals
hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip
RTC_DS1307 rtc;
Rotary rotary = Rotary(3, 4);
Button back_but;
Button sel_but;
/*
    // code to write a custom character to lcd
    uint8_t bell[8]  = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};   // shape of character
    lcd.createChar(1, bell);
    lcd.write(1);   // writes the character defined by previous line.
*/


// makes taking user input easier
typedef enum Input {    // combines rotary encoder and buttons together
    none,
    select,
    back,
    cw = DIR_CW,     // for rotary encoder
    ccw = DIR_CCW    // for rotary encoder
};



void setup() {
    Serial.begin(9600);
    rtc.begin();
    int status = lcd.begin(20, 4);   // 10 columns 4 rows size screen
    if(status) {    // non zero status means it was unsuccesful
        // hd44780 has a fatalError() routine that blinks an led if possible
        // begin() failed so blink error code using the onboard LED if possible
        hd44780::fatalError(status); // does not return
    }

    // select button on encoder
    sel_but.begin(2, false);    // no pullup

    // back button
    back_but.begin(5, true);    // use pullup
}



void loop() {
    static uint32_t timeout = 0;    // a 16bit value of milliseconds can measure 65s of time
    DateTime aerator_on_time(2025, 1, 1, 10, 00, 0);    // 10:00
    DateTime aerator_off_time(2025, 1, 1, 10, 25, 0);   // 10:25
    

    // do stuff with buttons
    Input input = none;
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
    
    if((millis() - timeout) > 5000) {    // triggers every 5 seconds
        DateTime now = rtc.now();
        if(timeCompare(now, target)) {
            Serial.println("ALARM!!");
        }
        else {
            Serial.println("Waiting for alarm...");
        }
        timeout = (uint32_t)millis(); // start new interval
    }
}

// returns 1 if t1 is after or same as t2.
// Returns 0 if t1 is before t2.
uint8_t timeCompare(DateTime t1, DateTime t2) {
    int32_t diff = secsSinceMidnight(t1) - secsSinceMidnight(t2);
    if(diff < -43200) diff += 86400; // wrap if difference > 12 hours backward
    if(diff > 43200) diff -= 86400;  // wrap if difference > 12 hours forward
    if(diff >= 0) return 1;
    else return 0;
}

// convert a time to seconds after midnight that day.
// Basically ignore the year/month/day, just take the time of day hh:mm:ss
uint32_t secsSinceMidnight(DateTime t) {
    return t.hour()*3600UL + t.minute()*60UL + t.second();
}
