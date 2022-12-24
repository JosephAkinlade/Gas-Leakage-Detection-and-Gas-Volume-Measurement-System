#include "sim800l.h"
#include "keypad.h"
#include "HX711.h"

#define BUZZER 4
#define MQ5_PIN A0
#define THRESHOLD 150

uint8_t rowPins[NUMBER_OF_ROWS] = {5,6,7,8};
uint8_t columnPins[NUMBER_OF_COLUMNS] = {9,10,11,12};

Keypad keypad(rowPins,columnPins);
SoftwareSerial gsmSerial(2,3); //RX:2, TX:3
SIM800L gsm(&gsmSerial);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(MQ5_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rawVal = analogRead(MQ5_PIN);
  Serial.println(rawVal);

//  if(rawVal > THRESHOLD)
//  {
//    digitalWrite(BUZZER, HIGH);
//    gsm.SendSMS("+2348098056507", "GAS LEAKAGE DETECTED");
//  }
//  else
//  {
//    digitalWrite(BUZZER, LOW);
//  }

}
