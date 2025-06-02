#include <iostream>
#include <stdio.h>
#include <string>
#include "tempoData.h"

/* ************************************************************************************ */
/* ***** Definições de Constantes ***** */
int d_ano, mes, dia;
byte d_mes, d_dia, diasSemana_Val;
byte t_hora, t_minuto, t_segundo;



//----------------------------------------NTP Server and Time Settings.
// The DS3231 RTC module is not always accurate, so every time the ESP32 is turned on or rebooted,
// the time and date on the DS3231 RTC module will be set based on the time from the NTP Server.
// Please adjust the settings below to suit your area.

// Source : https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/

const char* ntpServer = "pool.ntp.org";
int time_zone_UTC = -3;
// - For UTC -5.00 : -5 * 60 * 60 = -18000
// Check the UTC list here: https://en.wikipedia.org/wiki/List_of_UTC_offsets
//const long gmtOffset_sec = (60*60)*time_zone_UTC;

// Set it to 3600 if your country observes Daylight saving time. Otherwise, set it to 0.
// https://en.wikipedia.org/wiki/Daylight_saving_time
const int daylightOffset_sec = 0;



boolean dataTime_connect(RTC_DS1307 &rtc, int gmtOffset_sec){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
    struct tm timeinfo;
    char TimeDate[40];
  
    if (!getLocalTime(&timeinfo)) {    
      Serial.println();
      Serial.println("-------------");
      Serial.println("Falha ao obter horario.");
      Serial.println("-------------");
      delay(1500);
      return false;
    }
  
    t_hora   = timeinfo.tm_hour;
    t_minuto = timeinfo.tm_min;
    t_segundo = timeinfo.tm_sec;
  
    d_dia   = timeinfo.tm_mday;
    d_mes = timeinfo.tm_mon+1;
    d_ano  = timeinfo.tm_year+1900;
  
    rtc.adjust(DateTime(d_ano, d_mes, d_dia, t_hora, t_minuto, t_segundo));  
        
    sprintf(TimeDate, "Data : %02d/%02d/%d | Tempo : %02d:%02d:%02d", d_dia, d_mes, d_ano, t_hora, t_minuto, t_segundo);
    return true;   
}

std::string data(RTC_DS1307 &rtc){
    char Data[16];

    DateTime now = rtc.now();
    d_ano = now.year();
    d_mes = now.month();
    d_dia = now.day();       
        
    sprintf(Data, "%02d/%02d/%d", d_dia, d_mes, d_ano, 0, 0, 0);
    return std::string(Data);    
}

std::string time(RTC_DS1307 &rtc){   
    char Time[16];
    DateTime now = rtc.now();    
    t_hora = now.hour();
    t_minuto = now.minute();
    t_segundo = now.second();     
    sprintf(Time, "%02d:%02d:%02d", t_hora, t_minuto, t_segundo);  
    return std::string(Time);   
}

int dia_semana(RTC_DS1307 &rtc,String data){
    if(data != ""){
        sscanf(data.c_str(), "%d-%d-%d", &d_ano, &mes, &dia);
        DateTime data(d_ano, mes, dia);
        diasSemana_Val = data.dayOfTheWeek();

    }else{
        DateTime now = rtc.now();    
        diasSemana_Val = now.dayOfTheWeek();        
    }       
    if (diasSemana_Val > 7 || diasSemana_Val < 0) diasSemana_Val = 7;          
    return diasSemana_Val; 
}

std::string format_data(String data){
    sscanf(data.c_str(), "%d-%d-%d", &d_ano, &mes, &dia);
    char data_formatada[16];
    sprintf(data_formatada, "%02d/%02d", dia, mes);
    return std::string(data_formatada);
}