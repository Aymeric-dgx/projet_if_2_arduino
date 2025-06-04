// Déclaration et initialisation des broches d'entrée
int analog_input = A0; // Sortie analogique du capteur
int digital_input = 3; // Sortie numérique du capteur
  
void setup () {
  pinMode(analog_input, INPUT);
  pinMode(digital_input, INPUT);
  Serial.begin(9600); // Sortie série à 9600 bps
  Serial.println("KY-024 Détection de champs magnétiques");
}
  
// Le programme lit les valeurs actuelles des broches d'entrée
// et les affiche sur la sortie sérielle
void loop () {
  float analog_value;
  int digital_value;
    
  //Les valeurs actuelles sont lues, converties en valeur de tension...
  analog_value = analogRead(analog_input) * (5.0 / 1023.0); 
  digital_value = digitalRead(digital_input);
    
  //... et émis à cet endroit
  Serial.print("Valeur de tension analogique: "); 
  Serial.print(analog_value, 4);
  Serial.print(" V, \t Valeur limite: ");
  
  if (digital_value == 1) {
      Serial.println("atteint");
  }
  else {
      Serial.println("pas encore atteint");
  }
  Serial.println("----------------------------------------------------------------");
  delay(1000);
}
