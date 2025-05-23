const int buzzerPin = 8;  // définit le pin du buzzer

void setup() {
  pinMode(buzzerPin, INPUT);  // configure le pin du buzzer en entrée
  Serial.begin(9600);
}

void loop() {
  int buzzerValue = digitalRead(buzzerPin);  // lit la valeur du buzzer

  if (buzzerValue == HIGH) {  // si une fréquence sonore est détectée
    Serial.println("Detected"); // déclenche une action
  }
  else {
    Serial.println("Not Detected");    
  }
}
