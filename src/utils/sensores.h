#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Definições de pinos
#define PHOTORESISTOR_PIN 34  // Pino do sensor de luminosidade (Photoresistor)
#define LED_PIN_BASE 2       // Pino base para os LEDs Neopixel (sequenciais)
#define NUM_LEDS 19          // Número total de LEDs Neopixel

extern Adafruit_NeoPixel strip;

void setupSensor();
int lerLuminosidade();
void acionarLEDsSequenciais();

#endif // SENSORES_H