#include <EEPROM.h>
#include "sim800l.h"
#include "keypad.h"
#include "HX711.h"
#include "hmi.h"

#define BUZZER 4
#define MQ5_PIN A0
#define THRESHOLD 120

uint8_t rowPins[NUMBER_OF_ROWS] = {5,6,7,8};
uint8_t columnPins[NUMBER_OF_COLUMNS] = {9,10,11,12};

static Keypad keypad(rowPins,columnPins);
static SoftwareSerial gsmSerial(2,3); //RX:2, TX:3
static SIM800L gsm(&gsmSerial);
static LiquidCrystal_I2C lcd(0x27,20,4);
static HMI hmi(&lcd,&keypad);

static void EEPROM_storeMobileNum(void)
{
  for(uint8_t i = 0; i < 11; i++)
  {
    EEPROM.update(i, hmi.mobileNum[i]);
    delay(5);
  }
}

static void EEPROM_getMobileNum(void)
{
  for(uint8_t i = 0; i < 11; i++)
  {
    hmi.mobileNum[i] = EEPROM[i];
  }
}

/**
 * @brief Momentarily signifies storage of HMI configurable 
 * parameters in <> flash memory.
*/
static void NotifyParamSave(LiquidCrystal_I2C& lcd)
{
  lcd.clear();
  lcd.print("DATA SAVED");
  delay(1000);
  lcd.clear();  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(MQ5_PIN, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Gas Leakage "); //startup message
  lcd.setCursor(4,1);
  lcd.print("detection and");
  lcd.setCursor(4,2);
  lcd.print("Gas Volume");
  lcd.setCursor(4,3);
  lcd.print("measurement");
  delay(3000);
  EEPROM_getMobileNum();
  lcd.clear(); 
}

void loop() {
  // put your main code here, to run repeatedly:
//  int rawVal = analogRead(MQ5_PIN);
//  Serial.println(rawVal);

//  if(rawVal > THRESHOLD)
//  {
//    digitalWrite(BUZZER, HIGH);
//    gsm.SendSMS("+2348098056507", "GAS LEAKAGE DETECTED");
//  }
//  else
//  {
//    digitalWrite(BUZZER, LOW);
//  }

  hmi.Start();
  if(hmi.GetTypingDoneFlag())
  {
    EEPROM_storeMobileNum();
    NotifyParamSave(lcd);
    hmi.ClearTypingDoneFlag();
  }

}
