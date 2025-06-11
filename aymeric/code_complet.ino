// Affichage OLED + température + ventilateur + qualité_air + vitesse du vent
// Branches à éviter : 0, 2, 12, 15

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h> 

// Taille de l’écran (pour écran 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


// Création de l’objet écran (I2C)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// pin_SDA = 21, pin_SCL = 22

int temp_id = 26;
int ventilateur_id = 13;
int qualité_A0_id = 34;
int buzzer_id = 23;
int capteur_magnetique_id = 18;

int seuil_temp = 30; // Alerte si supérieur à 30°C
int seuil_vitesse_vent = 60; //Alerte si supérieur à km/h
int seuil_humidité = 30; // Alerte si inférieur à 30%
int seuil_luminosité = 1000; // Seuil de luminosité pour passer de joir à nuit

// Création de l'objet dht pour la température
#define DHTTYPE DHT11
DHT dht(temp_id, DHTTYPE);


// Loops principales

void setup () {
  Serial.begin (9600);
  pinMode (temp_id, OUTPUT);
  pinMode(ventilateur_id, OUTPUT);

  // Initialiser I2C
  Wire.begin();  // SDA, SCL

  // Initialisation de l’écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C est l’adresse la plus fréquente
    Serial.println(F("Écran OLED non détecté"));
    while (true); // Arrêt
  }

  // Initialisation du capteur de temp
  dht.begin();
}

void loop () {
  // Récupération/calcul des données
  float humidté = dht.readHumidity();
  float température = dht.readTemperature();
  int air_value = analogRead(qualité_A0_id);
  

  // Récupération données qualité air
  
  Serial.print("qualité air : ");
  Serial.println(air_value);


  // Affichage sur l'écran OLED
  // Efface l’écran
  display.clearDisplay();

  // Définir la taille et couleur du texte
  display.setTextSize(1);             // Taille du texte (1 = normal)
  display.setTextColor(SSD1306_WHITE); // Couleur (blanc sur fond noir)
  display.setCursor(0, 0);             // Position de départ (x, y)


  // Affichage qualité humidité
  display.print(air_value);


  display.display();  // Affiche le contenu


  delay(2000);
}
