// Branches à éviter : 0, 2, 12, 15
// Branche sans risque : 4, 5, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
// Branche libre : 19, 21, 22, 25, 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <BH1750.h>
#include <WiFi.h>
#include <PubSubClient.h> 



// Déclaration des pin + seuils limites, ...

// Déclaration pin
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

// Seuils limites
int seuil_temp = 30; // Alerte si supérieur à 30°C
int seuil_vitesse_vent = 60; //Alerte si supérieur à km/h
int seuil_humidité = 30; // Alerte si inférieur à 30%
int seuil_luminosité = 1000; // Seuil de luminosité pour passer de joir à nuit

// Variable pour le calcul de la vitesse
volatile int nb_tours = 0; // Variable globale
unsigned long dernier_calcul = 0; 
float vitesse_vent = 0;
float diamètre_anémomètre = 0.08; // 8 cm en mètres


// Déclaration variable NodeRed

// Réseau WiFi auquel vous voulez vous connecter.
const char* ssid = "Android_ad";
const char* password = "Siryim49";

// Ce champ contient l'adresse IP ou le nom d'hôte du serveur MQTT.
const char* mqtt_server = "mqtt.ci-ciad.utbm.fr";

WiFiClient espace_client_degieux; // Initialiser la bibliothèque client wifi (connexion WiFi)
PubSubClient client_degieux(espace_client_degieux); // Créer un objet client MQTT (connexion MQTT) 
// Maintenant, notre client MQTT s'appelle "client". Nous devons maintenant créer une connexion à un broker.
long lastMsg = 0;


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


// Fonction pour le NodeRed (intéraction NodeRed -> ESP32 dans callback)

//Cette fonction se connecte au réseau WiFi en utilisant les paramètres de connexion fournis dans les variables ssid et password.
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); 

  WiFi.begin(ssid, password); // //démarre la connexion Wi-Fi avec les informations de connexion (SSID et mot de passe) fournies.

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



// Utilisée pour traiter les messages MQTT reçus par l'ESP32 et prévoir une action.
// Elle est appelée chaque fois qu'un nouveau message est reçu sur un Topic auquel l'ESP32 est abonné.

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  

  // Conversion des byte (message) en str
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


  //------------------------------------------- Début intéractions NodeRed -> ESP32 -----------------------------------------//
  
  /* (EXEMPLE) Vérification du message, on->activer LED, off->éteindre LED
  if(messageTemp == "on") {
    digitalWrite(ledPin, HIGH);
  }
  else if(messageTemp == "off") {
    digitalWrite(ledPin, LOW);
  }
  */
}


// Utilisée pour garantir la connexion MQTT entre l'ESP32 et le broker MQTT. 
//Elle est appelée dans la boucle principale et elle se répète jusqu'à ce que la connexion soit établie avec succès.

void reconnect() {
  // Loop until we're reconnected
  while (!client_degieux.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client_degieux.connect("espace_client_degieux")) {
      Serial.println("connected");
      client_degieux.subscribe("esp32/led_AYMERIC_DEGIEUX"); // S'abonne au topic
    } 

    else {
      Serial.print("failed, rc=");
      Serial.print(client_degieux.state());
      Serial.println(" try again in 5 seconds");
      // On attend 5 secondes avant de retenter la connexion
      delay(5000);
    }
  }
}




// Loops principales

void setup () {
  Serial.begin (9600);
  delay(2000);
  Serial.println("ESP32 Button Test");
  

  // NodeRed
  setup_wifi(); // Configurer la connexion Wi-Fi sur l'ESP32.
  client_degieux.setServer(mqtt_server, 1883); // Définit le serveur (broker) MQTT.
  client_degieux.setCallback(callback); // Définit la fonction callback comme la fonction à appeler lorsqu'un message MQTT est reçu.

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
  //-------------------- Partie "hardware" ----------------------//

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
  display.println(" C");

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



  //-------------------- Partie "NodeRed" ----------------------//

  // Vérifie si le client est connecté, sinon reconnect()
  if (!client_degieux.connected()) {
    reconnect();
  }

  // Maintient la connexion avec le serveur MQTT en vérifiant si de nouveaux messages sont arrivés et en envoyant les messages en attente.
  client_degieux.loop();

  // Cette partie vérifie le temps écoulé depuis le dernier message publié et n'envoie le prochain message que toutes les 0.5 secondes (500 millisecondes).
  long now = millis();
  if (now - lastMsg > 10000) { // Envoie des messages toute les 2 secondes
		lastMsg = now;
		
    //---------------------------------------- Début intéraction ESP32 -> NodeRed ---------------------------------------//

    // Envoie des données au NodeRed
    client_degieux.publish("station/humidity", String(humidité).c_str());
    client_degieux.publish("station/temperature", String(température).c_str());
    client_degieux.publish("station/light", String(luminosité).c_str());
    client_degieux.publish("station/air_quality", String(air_value).c_str());
    client_degieux.publish("station/wind_speed", String(vitesse_vent).c_str());

  }

  delay(2000);
}
