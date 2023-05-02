#include "HX711.h"

#define LC_DOUT_PIN 4
#define LC_SCK_PIN  3

HX711 scale;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  scale.begin(LC_DOUT_PIN,LC_SCK_PIN);
  scale.read();
  scale.read_average(20);
  scale.get_value(5);
  scale.get_units(5);
  scale.set_scale(-400);
  scale.tare();
  scale.read();
  scale.read_average(20);
  scale.get_value(5);
  scale.get_units(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("One reading:\t");
  Serial.print(scale.get_units(),1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10),5);

  scale.power_down();
  delay(5000);
  scale.power_up();
}
