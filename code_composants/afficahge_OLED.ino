// Affichage OLED

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Taille de l’écran (pour écran 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Création de l’objet écran (I2C)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialiser I2C si tu utilises d'autres pins (optionnel)
  Wire.begin(33,27);  // SDA, SCL

  Serial.begin(115200);

  // Initialisation de l’écran
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C est l’adresse la plus fréquente
    Serial.println(F("Écran OLED non détecté"));
    while (true); // Arrêt
  }

  // Efface l’écran
  display.clearDisplay();

  // Définir la taille et couleur du texte
  display.setTextSize(1);             // Taille du texte (1 = normal)
  display.setTextColor(SSD1306_WHITE); // Couleur (blanc sur fond noir)
  display.setCursor(0, 0);             // Position de départ (x, y)

  display.println("Hello, OLED!");
  display.println("ESP32 power :)");

  display.display();  // Affiche le contenu
}

void loop() {
  // rien ici
}
