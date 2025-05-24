#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

// Bibliotecas Gráfica
#include "MiniGrafx.h"
#include "ILI9341_SPI.h"

#include <SPI.h>


// Definição dos pinos do display
#define TFT_CS    5
#define TFT_DC    4
#define TFT_RST   15
//D8 controla a luz de fundo da tela
#define TFT_LED 25
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


const int pinOn = 14;
const int neoPixel = 2;

int ledsNeopixelCount = 32;
int countsLeds = 0;
int rgbRed = 0;
int rgbGreen = 0;
int rgbBlue = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(ledsNeopixelCount, neoPixel, NEO_GRB + NEO_KHZ800);

/*
  1 bit: 2 cores (2^1 = 2)
  2 bits: 4 cores (2^2 = 4)
  4 bits: 16 cores (2^4 = 16)
  8 bits, 256 cores (2^8 = 256)
*/
int BITS_PER_PIXEL = 4 ; // 2^4 = 16 colors
int SCREEN_WIDTH = 240;
int SCREEN_HEIGHT = 320;
uint16_t palette[] = {ILI9341_BLACK,     //  0
                      ILI9341_WHITE,     //  1
                      ILI9341_NAVY,      //  2
                      ILI9341_DARKCYAN,  //  3
                      ILI9341_DARKGREEN, //  4
                      ILI9341_MAROON,    //  5
                      ILI9341_PURPLE,    //  6
                      ILI9341_OLIVE,     //  7
                      ILI9341_LIGHTGREY, //  8
                      ILI9341_DARKGREY,  //  9
                      ILI9341_BLUE,      // 10
                      ILI9341_GREEN,     // 11
                      ILI9341_CYAN,      // 12
                      ILI9341_RED,       // 13
                      ILI9341_MAGENTA,   // 14
                      0xFD80};           // 15


// Inicializando o pacote gráfico de drivers da biblioteca
ILI9341_SPI tft = ILI9341_SPI(TFT_CS, TFT_DC);
MiniGrafx gfx = MiniGrafx(&tft, BITS_PER_PIXEL, palette);


// Dados de rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";


/**************************************************************************************/
/********************************* INICIANDO O SETUP  *********************************/
/**************************************************************************************/
void setup() {
  // Inicializa o serial
  Serial.begin(115200);

  // Inicializando leds
  pinMode(pinOn, OUTPUT);
  digitalWrite(pinOn, 1);

  // Inicilizando Pins para luz de fundo da tela
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // Inicializando biblioteca gráfica
  gfx.init();
  gfx.fillBuffer(0);
  gfx.commit();

  // Inicializa o display  
  display.begin();
  //display.fillScreen(ILI9341_BLACK);
  //display.setTextColor(ILI9341_RED);
  //display.setTextSize(3);
  //display.setRotation(1);
  //display.setCursor(20, 160);
  //display.println("Hello world!");
  //display.setTextColor(ILI9341_GREEN);
  //display.setTextSize(2);
  //display.println("Boom 0w0");

 
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
  gfx.fillBuffer(0);
  gfx.setColor(1);
  gfx.drawLine(0, 0, 50, 50);
  gfx.setColor(13);
  gfx.fillCircle(100, 100, 10);
  gfx.commit();
}
