// Déclaration et initialisation des broches d'entrée
int digital_input = 15; // Sortie numérique du capteur
  
void setup () {
  pinMode(digital_input, INPUT);
  Serial.begin(9600); // Sortie série à 9600 bps
  Serial.println("KY-024 Détection de champs magnétiques");
}
  
// Le programme lit les valeurs actuelles des broches d'entrée
// et les affiche sur la sortie sérielle
void loop () {
  int digital_value;
    
  //Les valeurs actuelles sont lues, converties en valeur de tension...
  digital_value = digitalRead(digital_input);
    
  //... et émis à cet endroit
  Serial.print("Valeur limite: ");
  
  if (digital_value == 1) {
      Serial.println("atteint");
  }
  else {
      Serial.println("pas encore atteint");
  }
  Serial.println("----------------------------------------------------------------");
  delay(1000);
}
