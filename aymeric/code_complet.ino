// Affichage OLED + température + ventilateur + qualité_air

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Taille de l’écran (pour écran 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


// Création de l’objet écran (I2C)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int temp_id = 26;
int OLED_SCL_id = 27;
int OLED_SDA_id = 33;
int ventilateur_id = 13;
int qualité_A0_id = 34;

int seuil_temp = 30;

byte dat [5];

// Fonction capteur de temp
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (temp_id) == LOW) {
      while (digitalRead (temp_id) == LOW);
      delayMicroseconds (30);
      if (digitalRead (temp_id) == HIGH)
      data |= (1 << (7-i));
      while (digitalRead (temp_id) == HIGH);
    }
  }
  return data;
}

void start_test () {
  digitalWrite (temp_id, LOW);
  delay (30);
  digitalWrite (temp_id, HIGH);
  delayMicroseconds (40);
  pinMode (temp_id, INPUT);
  while (digitalRead (temp_id) == HIGH);
  delayMicroseconds (80);
  if (digitalRead (temp_id) == LOW);
  delayMicroseconds (80);
  for (int i = 0; i < 4; i ++)
  dat[i] = read_data ();
  pinMode (temp_id, OUTPUT);
  digitalWrite (temp_id, HIGH);
}




void setup () {
  Serial.begin (9600);
  pinMode (temp_id, OUTPUT);
  pinMode(ventilateur_id, OUTPUT);

  digitalWrite(ventilateur_id, HIGH);

  // setup OLED
  // Initialiser I2C si tu utilises d'autres pins (optionnel)
  Wire.begin(OLED_SDA_id, OLED_SCL_id);  // SDA, SCL

  // Initialisation de l’écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C est l’adresse la plus fréquente
    Serial.println(F("Écran OLED non détecté"));
    while (true); // Arrêt
  }
}

void loop () {
  // Récupération données température + humidité
  start_test ();
  Serial.print ("Current humdity =");
  Serial.print (dat [0], DEC);
  Serial.print ('.');
  Serial.print (dat [1], DEC);
  Serial.println ('%');
  Serial.print ("Current temperature =");
  Serial.print (dat [2], DEC);
  Serial.print ('.');
  Serial.print (dat [3], DEC);
  Serial.println ('C');

  // Récupération données qualité air
  int air_value = analogRead(qualité_A0_id);
  Serial.print(air_value);


  // Affichage sur l'écran OLED

  // Efface l’écran
  display.clearDisplay();

  // Définir la taille et couleur du texte
  display.setTextSize(1);             // Taille du texte (1 = normal)
  display.setTextColor(SSD1306_WHITE); // Couleur (blanc sur fond noir)
  display.setCursor(0, 0);             // Position de départ (x, y)

  // Affichage temp
  display.print("Température :");
  display.print (dat [2], DEC);
  display.print ('.');
  display.print (dat [3], DEC);
  display.println ('C');

  // Affichage humidité
  display.print("Humidité :");
  display.print (dat [0], DEC);
  display.print ('.');
  display.print (dat [1], DEC);
  display.println ('%');

  // Affichage qualité humidité
  display.print(air_value);


  display.display();  // Affiche le contenu


  // Activation/désactivation du ventilatuer
  int actual_temp = dat[2] + dat[3]*0.1;
  if(actual_temp > seuil_temp) {
    digitalWrite(ventilateur_id, HIGH);
    Serial.println("Ventilateur allumé");
  } else {
    digitalWrite(ventilateur_id, LOW);
  }

  delay(2000);
}
