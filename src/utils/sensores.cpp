#include "sensores.h"
#include <Adafruit_NeoPixel.h>

#define LUMINOSITY_THRESHOLD 511  

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN_BASE, NEO_GRB + NEO_KHZ800);

unsigned long lastMillis = 0;  
const long interval = 5000;  

void setupSensor() {
  strip.begin();
  strip.show(); 
}

int lerLuminosidade() {
  int valor = analogRead(PHOTORESISTOR_PIN);
  return valor;
}

void acionarLEDsSequenciais() {
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastMillis >= interval) {
    lastMillis = currentMillis;  

    int valorAnalog = lerLuminosidade();
  
    Serial.print("valor analogico: ");
    Serial.println(valorAnalog);

    if (valorAnalog > LUMINOSITY_THRESHOLD) {
      
      int brilho = map(valorAnalog, LUMINOSITY_THRESHOLD, 4095, 0, 255);
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(brilho, 0, 0)); 
      }
    } else {
        for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); 
      }
    }
    strip.show();
  }
}