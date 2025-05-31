#ifndef WIFI_H
#define WIFI_H

#include <ArduinoJson.h>

void connect_wifi();

int connect_API_Meteor();
DynamicJsonDocument dados_API_Open_Meteor();

int connect_API_Weather ();
DynamicJsonDocument dados_API_Weather();



#endif
