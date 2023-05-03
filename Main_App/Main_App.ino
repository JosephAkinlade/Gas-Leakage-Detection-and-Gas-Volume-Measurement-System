#include <EEPROM.h>
#include "sim800l.h"
#include "keypad.h"
#include "HX711.h"
#include "hmi.h"

#define BUZZER            A1
#define MQ5_PIN           A0
#define THRESHOLD         140
#define LOADCELL_DOUT_PIN  4
#define LOADCELL_SCK_PIN   3

uint8_t rowPins[NUMBER_OF_ROWS] = {5,6,7,8};
uint8_t columnPins[NUMBER_OF_COLUMNS] = {9,10,11,12};

static Keypad keypad(rowPins,columnPins);
static SoftwareSerial gsmSerial(3,2); //GSM RX:2, GSM TX:3
static SIM800L gsm(&gsmSerial);
static LiquidCrystal_I2C lcd(0x27,20,4);
static HX711 scale;
static HMI hmi(&lcd,&keypad,&scale);

float calibration_factor = -104000; //This value seemed to work well
bool msgSent = false;

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
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(MQ5_PIN, INPUT);
  //[LOAD CELL INIT]
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  //[LCD INIT]
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
  lcd.clear(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int rawVal = analogRead(MQ5_PIN);
  Serial.println(rawVal);
  if(rawVal > THRESHOLD)
  {
    digitalWrite(BUZZER, HIGH);
    if(!msgSent)
    {
      gsm.SendSMS("+2348098056507", "GAS LEAKAGE DETECTED");
      msgSent = true;
    } 
  }
  else
  {
    digitalWrite(BUZZER, LOW);
    msgSent = false;
  }
  
  hmi.Start();
  if(hmi.GetTypingDoneFlag())
  {
    NotifyParamSave(lcd);
    hmi.ClearTypingDoneFlag();
  }
}
