#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Definição dos pinos do display
#define TFT_CS    5
#define TFT_DC    4
#define TFT_RST   15

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Dados de rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  // Inicializa o serial
  Serial.begin(115200);
  
  // Conecta à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi!");

  // Inicializa o display
  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);

  // Faz a requisição HTTP
  HTTPClient http;
  http.begin("https://api.open-meteo.com/v1/forecast?latitude=-12.9711&longitude=-38.5108&current_weather=true");
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();  // Recebe a resposta

    // Parse do JSON
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    float temperatura = doc["current_weather"]["temperature"];

    // Exibe no display
    display.setCursor(10, 10);
    
    display.print("Temperatura: ");
    display.print(temperatura);
    display.print(" C");
    Serial.println("Temperatura: ");
    Serial.println(temperatura);
    Serial.println(" C");
  } else {
    display.setCursor(10, 10);
    display.print("Erro ao obter dados.");
    Serial.println("Erro ao obter dados. ");
  }
  http.end();
}

void loop() {
  // Nenhuma ação no loop
}
