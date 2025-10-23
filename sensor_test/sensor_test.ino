/*
    A sketch to test the functionality of the water sensor.
    Simply turns on and off an LED on the arduino when water is present or
    not. Also, prints to serial port the status of the sensor.
*/



void setup() {
    Serial.begin(9600);

    pinMode(6, INPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop() {
    byte sensor_status = digitalRead(6);

    if(sensor_status==1) {
        digitalWrite(13, HIGH);
        Serial.println("HIGH");
    }
    else {
        digitalWrite(13, LOW);
        Serial.println("LOW");
    }
}
