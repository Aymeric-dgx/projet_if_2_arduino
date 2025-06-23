const byte capteurPin = 2;       // Broche du capteur à effet Hall
volatile unsigned int nbTours = 0;
unsigned long dernierTemps = 0;
float rayon_cm = 9.0;            // rayon en centimètres (ex : 9 cm)
float perimetre = 2 * 3.1416 * (rayon_cm / 100.0); // en mètres

void setup() {
  Serial.begin(9600);
  pinMode(capteurPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(capteurPin), compterTour, FALLING);
  dernierTemps = millis();
}

void loop() {
  unsigned long tempsActuel = millis();

  if (tempsActuel - dernierTemps >= 1000) { // toutes les 1 sec
    noInterrupts();
    unsigned int tours = nbTours;
    nbTours = 0;
    interrupts();

    float tps = tours; // tours par seconde
    float vitesseVent = tps * perimetre; // en m/s

    Serial.print("Tours/s : ");
    Serial.print(tps);
    Serial.print(" | Vitesse du vent : ");
    Serial.print(vitesseVent);
    Serial.println(" m/s");

    dernierTemps = tempsActuel;
  }
}

void compterTour() {
  nbTours++;
}
