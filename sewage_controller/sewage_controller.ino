#include <Wire.h>       // i2c library
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

// Globals
hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip
/*
    // code to write a custom character to lcd
    uint8_t bell[8]  = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};   // shape of character
    lcd.createChar(1, bell);
    lcd.write(1);   // writes the character defined by previous line.
*/


void setup() {
    int status = lcd.begin(20, 4);   // 10 columns 4 rows size screen
    if(status) {    // non zero status means it was unsuccesful
            // hd44780 has a fatalError() routine that blinks an led if possible
            // begin() failed so blink error code using the onboard LED if possible
            hd44780::fatalError(status); // does not return
        }
    
}

void loop() {

}
