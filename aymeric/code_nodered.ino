#include <WiFi.h> // Cette bibliothèque permet à l'ESP32 de se connecter au réseau WiFi.
#include <PubSubClient.h> // Cette bibliothèque vous permet d'envoyer et de recevoir des messages MQTT.



// Réseau WiFi auquel vous voulez vous connecter.
const char* ssid = "Android_ad";
const char* password = "Siryim49";

// Ce champ contient l'adresse IP ou le nom d'hôte du serveur MQTT.
const char* mqtt_server = "mqtt.ci-ciad.utbm.fr";


WiFiClient espace_client_degieux; // Initialiser la bibliothèque client wifi (connexion WiFi)
PubSubClient client_degieux(espace_client_degieux); // Créer un objet client MQTT (connexion MQTT) 
// Maintenant, notre client MQTT s'appelle "client". Nous devons maintenant créer une connexion à un broker.
long lastMsg = 0;




void setup() {

  delay(2000);
  Serial.println("ESP32 Button Test");

  setup_wifi(); // Configurer la connexion Wi-Fi sur l'ESP32.
  client_degieux.setServer(mqtt_server, 1883); // Définit le serveur (broker) MQTT.
  client_degieux.setCallback(callback); // Définit la fonction callback comme la fonction à appeler lorsqu'un message MQTT est reçu.
}




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


  // Début intéractions NodeRed -> ESP32
  
  // (EXEMPLE) Vérification du message, on->activer LED, off->éteindre LED
  if(messageTemp == "on") {
    digitalWrite(ledPin, HIGH);
  }
  else if(messageTemp == "off") {
    digitalWrite(ledPin, LOW);
  }
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





void loop() {
	
  // Vérifie si le client est connecté, sinon reconnect()
  if (!client_degieux.connected()) {
    reconnect();
  }
  
  // Maintient la connexion avec le serveur MQTT en vérifiant si de nouveaux messages sont arrivés et en envoyant les messages en attente.
  client_degieux.loop();
  

  //La dernière partie vérifie le temps écoulé depuis le dernier message publié et n'envoie le prochain message que toutes les 2 secondes (2000 millisecondes).
  
  long now = millis();
  if (now - lastMsg > 500) {
		lastMsg = now;
		
    // Début intéraction ESP32 -> NodeRed

		/// (EXEMPLE) Lire l'état actuel du bouton et publier un message sur le topic "esp32/button" en utilisant la fonction client.publish()
		if (digitalRead(buttonPin) == HIGH) {
      Serial.println("Publishing: on");
      client_degieux.publish("esp32/button_AYMERIC_DEGIEUX", "on");
    } else {
      Serial.println("Publishing: off");
      client_degieux.publish("esp32/button_AYMERIC_DEGIEUX", "off");
}

  }
}
