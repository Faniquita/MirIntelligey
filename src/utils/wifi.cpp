#include <iostream>
#include <stdio.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "wifi.h"


/* ************************************************************************************ */
/* ***** Definições de Constantes ***** */
HTTPClient http;

// Dados de rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

//API Weather
String openWeatherMapApiKey = "39e6f240a9394fa985f25654253005";
String cidade = "Salvador";
String days = "8";


void connect_wifi(){
    // Conecta à rede Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao Wi-Fi...");
    }
    Serial.println("Conectado ao Wi-Fi!");
}

int connect_API_Meteor(){   
    // Faz a requisição HTTP conectando a API    
    String pathAPI = "https://api.open-meteo.com/v1/forecast?latitude=-12.9711&longitude=-38.5108&daily=temperature_2m_max,temperature_2m_min,weather_code&current=temperature_2m&timezone=America%2FSao_Paulo&forecast_days=14";
    http.begin(pathAPI);    
    int httpCode = http.GET();    
    return httpCode;    
}

DynamicJsonDocument dados_API_Open_Meteor(){
    int httpCode = connect_API_Meteor();
    DynamicJsonDocument doc(8192);
    if (httpCode > 0) {        
        String payload = http.getString();
        // Parse do JSON        
        deserializeJson(doc, payload);        
    } else {
        Serial.println("Erro ao obter dados da API_Open_Meteor.");        
    }
    http.end();
    return doc;
}

int connect_API_Weather(){    
    // Faz a requisição HTTP conectando a API    
    String pathAPI = "https://api.weatherapi.com/v1/forecast.json?key=" + openWeatherMapApiKey + "&q=" + cidade + "&days=8&lang=pt";
    http.begin(pathAPI); 
    int httpCode = http.GET();    
    return httpCode;    
}

DynamicJsonDocument dados_API_Weather(){
    int httpCode = connect_API_Weather();
    DynamicJsonDocument doc(16378);

    if (httpCode > 0) {        
        String payload = http.getString();
        // Parse do JSON        
        deserializeJson(doc, payload);
        WiFiClient* stream = http.getStreamPtr();
    } else {
        Serial.println("Erro ao obter dados API Weather.");        
    }
    http.end();
    return doc;
}






