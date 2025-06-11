#include <DHT.h>

#define DHTPIN 26      // Le GPIO utilisé pour DATA
#define DHTTYPE DHT11  // Ou DHT22 selon le modèle

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Erreur de lecture DHT11");
    return;
  }

  Serial.print("Température: ");
  Serial.print(t);
  Serial.println(" °C");

  Serial.print("Humidité: ");
  Serial.print(h);
  Serial.println(" %");

  delay(2000);
}
