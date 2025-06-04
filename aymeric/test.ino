#define HALL_ANALOG_PIN 34
#define HALL_DIGITAL_PIN 35

void setup() {
  Serial.begin(9600);
  pinMode(HALL_DIGITAL_PIN, INPUT);
}

void loop() {
  int analogValue = analogRead(HALL_ANALOG_PIN);
  int digitalValue = digitalRead(HALL_DIGITAL_PIN);

  Serial.print("Valeur analogique : ");
  Serial.print(analogValue);
  Serial.print(" | Digital : ");
  Serial.println(digitalValue == LOW ? "Champ détecté" : "Pas de champ");

  delay(500);
}
