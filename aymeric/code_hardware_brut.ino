// Branches à éviter : 0, 2, 12, 15
// Branche sans risque : 4, 5, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
// Branche libre : 19, 21, 22, 25, 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <BH1750.h>


// Déclaration des pin + seuils limites + variables

// pin_SDA = 21, pin_SCL = 22
int temp_id = 26;
int ventilateur_id = 13;
int qualité_A0_id = 34; 
int buzzer_id = 23;
int capteur_magnetique_id = 18;
int LED_tempete_id = 4;
int LED_canicule_id = 5;
int LED_secheresse_id = 14;
int LED_volets_id = 33;
int LED_jour_id = 27;
int LED_nuit_id = 32;

int seuil_temp = 30; // Alerte si supérieur à 30°C
int seuil_vitesse_vent = 60; //Alerte si supérieur à km/h
int seuil_humidité = 30; // Alerte si inférieur à 30%
int seuil_luminosité = 1000; // Seuil de luminosité pour passer de joir à nuit

volatile int nb_tours = 0; // Variable globale
unsigned long dernier_calcul = 0; 
float vitesse_vent = 0;
float diamètre_anémomètre = 0.08; // 8 cm en mètres


// Création de l'objet dht pour la température
#define DHTTYPE DHT11
DHT dht(temp_id, DHTTYPE);


// Création de l’objet écran 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Création de l'objet pour le luxmetre
BH1750 lightMeter;


// Fonction "globale" qui va incrémenter le nb_tours à chaque fois que l'aimant passe (à tester)
void IRAM_ATTR onAimantPasse() {
  nb_tours++;
}


// Loops principales

void setup () {
  Serial.begin (9600);

  // Déclarion des OUTPUT et INPUT
  pinMode (temp_id, OUTPUT);
  pinMode(ventilateur_id, OUTPUT);
  pinMode(buzzer_id, OUTPUT);

  pinMode(LED_tempete_id, OUTPUT);
  pinMode(LED_canicule_id, OUTPUT);
  pinMode(LED_secheresse_id, OUTPUT);
  pinMode(LED_volets_id, OUTPUT);
  pinMode(LED_jour_id, OUTPUT);
  pinMode(LED_nuit_id, OUTPUT);



  // Initialiser I2C
  Wire.begin();  // SDA, SCL

  // Initialisation de l’écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C est l’adresse la plus fréquente
    Serial.println(F("Écran OLED non détecté"));
    while (true); // Arrêt
  }
  // Initialisation du capteur de temp
  dht.begin();
  // Initialisation du luxmetre
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

  // Pour le calcul de la vitesse du vent
  attachInterrupt(digitalPinToInterrupt(capteur_magnetique_id), onAimantPasse, FALLING); // ou RISING selon ton montage
}



void loop () {
  // Récupération des données "classiques"
  float humidité = dht.readHumidity();
  float température = dht.readTemperature();
  float luminosité = lightMeter.readLightLevel();
  int air_value = analogRead(qualité_A0_id);
  
  // Calcule de la vitesse du vent
  unsigned long maintenant = millis(); 

  if (maintenant - dernier_calcul >= 5000) {
    float tours_par_sec = nb_tours / 5.0;
    float perimetre = diamètre_anémomètre * PI;
    vitesse_vent = tours_par_sec * perimetre * 3.6; // m/s → km/h

    nb_tours = 0;
    dernier_calcul = maintenant;
  }

  // Affichage sur l'écran OLED
  
  // Efface l’écran
  display.clearDisplay();

  // Définir la taille et couleur du texte
  display.setTextSize(1);             // Taille du texte (1 = normal)
  display.setTextColor(SSD1306_WHITE); // Couleur (blanc sur fond noir)
  display.setCursor(0, 0);             // Position de départ (x, y)

  // Ecriture sur l'ecran OLED
  display.print("Temperature : ");
  display.print(température);
  display.print(" C");

  display.print("Humidite : ");
  display.print(humidité);
  display.println(" %");

  display.print("Vitesse vent : ");
  display.print(vitesse_vent);
  display.println(" km/h");


  if (air_value < 400) {
    display.println("Air pur");
  } else if (air_value < 1000) {
    display.println("Air moyen");
  } else {
    display.println("Air pollue");
  }

  display.print("Luminosite : ");
  display.print(luminosité);
  display.println(" lx");

  // Affiche le contenu
  display.display();  

  delay(2000);
}
