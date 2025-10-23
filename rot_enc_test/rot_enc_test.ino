/*
    This code checks whether the rotary encoder can be software debounced, and
    whether it correctly shows each button press. The state of the rotary
    encoder is printed via serial.

    This rotary encoder button is active LOW.
*/

#include "Rotary.h"
#include "Button.h"

// Also sets pins as inputs without pullups
// The order in which the pins are given as arguments changes which direction
// of rotation is recognised as 'clockwise'.
Rotary rotary = Rotary(3, 4);
Button back_but;
Button sel_but;
int counter = 0;



void setup() {
    Serial.begin(9600);

    // select button on encoder
    sel_but.begin(2, false);    // no pullup

    // back button
    back_but.begin(5, true);    // use pullup
}

void loop() {
    unsigned char rot_state = rotary.process();

    if(rot_state == DIR_CW) {
        counter++;
        Serial.println(counter);
    }
    else if(rot_state == DIR_CCW) {
        counter--;
        Serial.println(counter);
    }

    if(back_but.process()) {
        Serial.println("Back");
    }
    if(sel_but.process()) {
        Serial.println("Select");
    }

}
