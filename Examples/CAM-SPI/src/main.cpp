#include <Arduino.h>

HardwareSerial Serial6(PC7, PC6);

#define BAUD_RATE 19200

void setup() {
    Serial6.begin(BAUD_RATE);
    Serial6.setRx(PC7);
    Serial6.setTx(PC6);
    Serial6.begin(BAUD_RATE);
    
    Serial.begin(9600);
    delay(1000);  // Give the OpenMV Cam time to bootup.
    Serial.println("start");
}



void loop() {
    byte byteRead = 0;
    
    if (Serial6.available()) {
        byteRead = Serial6.read();
        Serial.println(byteRead);
    } else {
        Serial.println("でーたなんてなかった");
    }
}