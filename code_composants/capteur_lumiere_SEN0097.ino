#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup() {
  Serial.begin(9600);
  Wire.begin();  // SDA, SCL
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Lumière: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(500);
}
