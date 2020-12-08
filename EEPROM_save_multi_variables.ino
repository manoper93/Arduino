// Storing struct variables in EEPROM
// Sequential read / write of variables.
#include <EEPROM.h>

#define BUTTON_TEST 7

long int xc;
long int yc;

void setup() {

   pinMode(BUTTON_TEST,INPUT_PULLUP);
   pinMode(10,OUTPUT);
   digitalWrite(10,LOW);

   Serial.begin(115200);
     xc=0;
     yc=200;
}

void loop() {
long int sx = EEPROM.read(0); //nao pode estar na global
long int sy = EEPROM.read(1); //nao pode estar na global

     xc++;
     yc++;
     Serial.print("xc "); Serial.println(xc);
     Serial.print("yc "); Serial.println(yc);
        Serial.println("---");
     Serial.print("x "); Serial.println(sx);
     Serial.print("y "); Serial.println(sy);
     Serial.println("---");

     if (digitalRead(BUTTON_TEST)==HIGH) {
        digitalWrite(10,HIGH);

        EEPROM.write(0,xc);
        EEPROM.write(1,yc);
        Serial.print("GRAVADO");
        Serial.print("x "); Serial.println(sx);
        Serial.print("y "); Serial.println(sy);
        Serial.println("---");
        Serial.println("---");

        delay (500);
        digitalWrite(10,LOW);
     }
     delay(2000);
}
