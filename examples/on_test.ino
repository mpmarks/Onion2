
#include <Wire.h>
#include <Onion2.h>
#include <Adafruit_NeoPixel.h>


#define LED 13
#define LED2 A1
#define NEO 8

Adafruit_NeoPixel rgb(1, NEO);


void recvEvent() {
  byte data;
  byte r=0, g=0, b=0;
  if (Wire.available()) r = Wire.read();
  if (Wire.available()) g = Wire.read();
  if (Wire.available()) b = Wire.read();
  rgb.setPixelColor(0, r, g, b);
  rgb.show();
}

uint16_t temp;

byte reqEvent() {
  temp = analogRead(A0);
  return temp;
}


void setup() {
  
  pinMode(LED, OUTPUT);
//  pinMode(LED2, OUTPUT);
  Serial.begin(115200);
  rgb.begin();
  rgb.clear();
  rgb.show();
 
  
  ONION.registerRcvHandler(1, &recvEvent); 
  ONION.registerReqHandler(2, &reqEvent);

 // digitalWrite(LED2, LOW);
}

void loop() {
  Serial.print("Analog "); Serial.println(temp);
  digitalWrite(LED, 1);
  delay(1000);
  digitalWrite(LED, 0);
  delay(1000);

}
