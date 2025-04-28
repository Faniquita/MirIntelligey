#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

#include "SPI.h"
// #include "SquareLine/ui.h"


// Definição dos pinos do display
#define TFT_CS    5
#define TFT_DC    4
#define TFT_RST   15
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const int pinOn = 14;
const int neoPixel = 2;

int ledsNeopixelCount = 32;
int countsLeds = 0;
int rgbRed = 0;
int rgbGreen = 0;
int rgbBlue = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(ledsNeopixelCount, neoPixel, NEO_GRB + NEO_KHZ800);


// Dados de rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";


void setup() {
  // Inicializa o serial
  Serial.begin(115200);

  // // Inicializando leds
  pinMode(pinOn, OUTPUT);
  digitalWrite(pinOn, 1);


  // Inicializa o display
  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextColor(ILI9341_RED);
  display.setTextSize(3);
  display.setRotation(1);
  display.setCursor(20, 160);
  display.println("Hello world!");
  display.setTextColor(ILI9341_GREEN);
  display.setTextSize(2);
  display.println("Boom 0w0");

  // Inicializando grafico do display e seus drivers
  // lv_init();
  // your_display_driver_init();
  // your_input_driver_init();  
  // ui_init(); 

  // while(1) {
  //   lv_timer_handler();             
  //   delay_ms(5);               
  // }

  
  // Conecta à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi!");
  

  // Faz a requisição HTTP conectando a API
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
