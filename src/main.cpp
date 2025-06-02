/* ***** BIBLIOTECA ***** */
#include <iostream>
#include <stdio.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <string>
#include <ArduinoJson.h>
#include <locale.h>
#include "RTClib.h"
#include "time.h"

/* ************************************************************************************ */
/* ***** ARQUIVOS EXTERNOS ***** */
#include "utils/wifi.h"
#include "utils/tempoData.h"
#include "utils/sensores.h"
#include "ui/ui.h"
#include "ui/images.h"


/* ************************************************************************************ */
/* ***** Definições de Pinos ***** */
// Definição dos pinos do display
#define TFT_CS    5
#define TFT_DC    4
#define TFT_RST   15
#define TFT_LED   25 //D8 controla a luz de fundo da tela
#define LED_ONOFF 14
#define RTC_SDA   21
#define RTC_SCL   22

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8)) 

#define INTERVAL_UPDATING_WEATHER_DATA 300 

TFT_eSPI tft = TFT_eSPI();
uint8_t *draw_buf;
uint32_t lastTick = 0;
lv_display_t * disp; 
String jsonBuffer;
RTC_DS1307 rtc;

/* ************************************************************************************ */
/* ***** Definições de Váriaveis/Constantes ***** */
//----------------------------------------Variable for millis/timer.
unsigned long prevMill = 0;
const long intervalMill = 1000;

//---------------------------------------- Dia, mês, ano, data e tempo
char diasSemana[8][10] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sábado", "Error"};
int cnt = 0;
int int_dayWeek, int_dayWeek1, int_dayWeek2, int_dayWeek3, int_dayWeek4, int_dayWeek5, int_dayWeek6, int_dayWeek7;
int gmtOffset_sec;

int str_Day_Night;
int cod_type_clima1, cod_type_clima2, cod_type_clima3, cod_type_clima4, cod_type_clima5, cod_type_clima6, cod_type_clima7;

std::string std_data, std_time, std_dataTime, str_data_formatada;

String std_data1, std_data2, std_data3, std_data4, std_data5, std_data6, std_data7;
String str_label_city, str_city, str_uf;
String str_Weather_Icon, str_Weather_Description;
String str_Weather_Icon1, str_Weather_Icon2, str_Weather_Icon3, str_Weather_Icon4, str_Weather_Icon5, str_Weather_Icon6, str_Weather_Icon7;
String str_type_temperature;
String str_Temperature, str_Humidity, str_Feels_Like, str_Temp_Max, str_Temp_Min, str_Wind_Speed;
String str_Temperature1, str_Temperature2, str_Temperature3, str_Temperature4, str_Temperature5, str_Temperature6, str_Temperature7;
String str_Temp_Max1, str_Temp_Min1;
String str_Temp_Max2, str_Temp_Min2;
String str_Temp_Max3, str_Temp_Min3;
String str_Temp_Max4, str_Temp_Min4;
String str_Temp_Max5, str_Temp_Min5;
String str_Temp_Max6, str_Temp_Min6;
String str_Temp_Max7, str_Temp_Min7;
String type_clima;

DynamicJsonDocument doc_API_Open_Meteor(8192);
DynamicJsonDocument doc_API_Wheater(16378);


/**************************************************************************************/
/********************************* FUNÇÕES  *********************************/
/**************************************************************************************/
void update_UI() {
  lv_tick_inc(millis() - lastTick);
  lastTick = millis();
  lv_timer_handler(); 
}

void set_initial_display_of_labels() {
  lv_label_set_text(objects.label_city, "Cidade / Estado");
  lv_label_set_text(objects.label_date, "--/--/----");
  lv_label_set_text(objects.label_time, "--:--:--");
  lv_label_set_text(objects.label_day, "-------");
  lv_label_set_text(objects.label_date1, "--/--/--");
  lv_label_set_text(objects.label_date2, "--/--/--");
  lv_label_set_text(objects.label_date3, "--/--/--");
  lv_label_set_text(objects.label_date4, "--/--/--");
  lv_label_set_text(objects.label_date5, "--/--/--");
  lv_label_set_text(objects.label_date6, "--/--/--");
  lv_label_set_text(objects.label_date7, "--/--/--");
  lv_label_set_text(objects.label_day1, "-------");
  lv_label_set_text(objects.label_day2, "-------");
  lv_label_set_text(objects.label_day3, "-------");
  lv_label_set_text(objects.label_day4, "-------");
  lv_label_set_text(objects.label_day5, "-------");
  lv_label_set_text(objects.label_day6, "-------");
  lv_label_set_text(objects.label_day7, "-------");
  lv_label_set_text(objects.label_weather_description, "-----");
  lv_label_set_text(objects.label_temperature, "--.-");
  lv_label_set_text(objects.label_type_temperature, "°C");
  lv_label_set_text(objects.label_maxtemperature1, "--.-°");
  lv_label_set_text(objects.label_mintemperature1, "--.-°");
  lv_label_set_text(objects.label_maxtemperature2, "--.-°");
  lv_label_set_text(objects.label_mintemperature2, "--.-°");
  lv_label_set_text(objects.label_maxtemperature3, "--.-°");
  lv_label_set_text(objects.label_mintemperature3, "--.-°");
  lv_label_set_text(objects.label_maxtemperature4, "--.-°");
  lv_label_set_text(objects.label_mintemperature4, "--.-°");
  lv_label_set_text(objects.label_maxtemperature5, "--.-°");
  lv_label_set_text(objects.label_mintemperature5, "--.-°");
  lv_label_set_text(objects.label_maxtemperature6, "--.-°");
  lv_label_set_text(objects.label_mintemperature6, "--.-°");
  lv_label_set_text(objects.label_maxtemperature7, "--.-°");
  lv_label_set_text(objects.label_mintemperature7, "--.-°");
  lv_label_set_text(objects.label_feels_like, "--.-°");
  lv_label_set_text(objects.label_temp_max, "--.-°");
  lv_label_set_text(objects.label_temp_min, "--.-°");
  lv_label_set_text(objects.label_humidity, "---.-%");
  lv_label_set_text(objects.label_wind, "---.- km/h");
  lv_label_set_text(objects.label_type_temperature1, "°C");
  lv_label_set_text(objects.label_type_temperature2, "°C");
  lv_label_set_text(objects.label_type_temperature3, "°C");

  update_UI();
}

void get_TimeDate_from_NTP_server() {
  update_UI();
  delay(500);

  if(dataTime_connect(rtc, gmtOffset_sec)){
    update_UI();
    delay(1000);
    update_UI();
  }else{
    update_UI();
    Serial.println("-------------");
    Serial.println("Sem Conexão com Internet...");
    Serial.println("-------------");
    delay(1500);
  }
}

const lv_image_dsc_t* get_weather_days_week_icon(int cod_weather_day) {
  /* 
    TIPOS DE CLIMA | API_Wheater
    Céu limpo: Códigos 0 e 1
    Parcialmente nublado: Código 2
    --Nublado: Código 3
    --Encoberto: Código 3
    --Névoa: Códigos 45 e 48
    Possibilidade de chuva irregular: Códigos 51, 53, 55, 61, 63 e 80, 81
    --Chuva forte: Códigos 65 e 82
    --Tempestade: Códigos 95, 96 e 99
    --Neve: Códigos 71, 73, 75, 85 e 86
  */ 
  if(cod_weather_day == 3){
    return &img_icon_03d_03n_16p;
  }else  if(cod_weather_day == 45 || cod_weather_day == 48){
    return &img_icon_50d_50n_16p;
  }else if(cod_weather_day == 65 || cod_weather_day == 82){
    return &img_icon_09d_09n_16p;
  }else if(cod_weather_day == 95 || cod_weather_day == 96 || cod_weather_day == 99){
    return &img_icon_11d_11n_16p;
  }else if(cod_weather_day == 71 || cod_weather_day == 73 || cod_weather_day == 75 || 
            cod_weather_day == 85 || cod_weather_day == 86){
    return &img_icon_13d_13n_16p;
  }else{
    if(cod_weather_day == 0 || cod_weather_day == 1){
      return &img_icon_01d_16p;
    }else if(cod_weather_day == 2){
      return &img_icon_02d_16p;
    }else if(cod_weather_day == 51 || cod_weather_day == 53 || cod_weather_day == 55 || 
              cod_weather_day == 61 || cod_weather_day == 63 || cod_weather_day == 80 ||
              cod_weather_day == 81){
      return &img_icon_10d_16p;
    }else{
      return &img_icon_01d_16p;
    }
  }
} 

const lv_image_dsc_t* get_weather_day_icon(String type_clima, int day_night) {
  /* 
    TIPOS DE CLIMA | API_Wheater
    Céu limpo: Códigos 0 e 1
    Parcialmente nublado: Código 2
    --Nublado: Código 3
    --Encoberto: Código 3
    --Névoa: Códigos 45 e 48
    Possibilidade de chuva irregular: Códigos 51, 53, 55, 61, 63 e 80, 81
    --Chuva forte: Códigos 65 e 82
    --Tempestade: Códigos 95, 96 e 99
    --Neve: Códigos 71, 73, 75, 85 e 86
  */   
  // if(str_Weather_Description == "Nublado"){
  //   return &img_icon_04d_04n_72p;
  // }else if(str_Weather_Description == "Encoberto"){
  //   return &img_icon_03d_03n_72p;
  // }else if(str_Weather_Description == "Névoa"){
  //   return &img_icon_50d_50n_72p;
  // }else if(str_Weather_Description == "Chuva forte"){
  //   return &img_icon_09d_09n_72p;
  // }else if(str_Weather_Description == "Tempestade"){
  //   return &img_icon_11d_11n_72p;
  // }else if(str_Weather_Description == "Neve"){
  //   return &img_icon_13d_13n_72p;
  // }else{
  //   if(str_Day_Night == 1){
  //     //Dia
  //     if(str_Weather_Description == "Céu limpo"){
  //       return &img_icon_01d_72p;
  //     }else if(str_Weather_Description == "Parcialmente nublado"){
  //       return &img_icon_02d_72p;
  //     }else if(str_Weather_Description == "Possibilidade de chuva irregular"){
  //       return &img_icon_10d_72p;
  //     }else{
  //       return &img_icon_01d_72p;
  //     }
  //   }else{
  //     //Noite
  //     if(str_Weather_Description == "Céu limpo"){
  //       return &img_icon_01n_72p;
  //     }else if(str_Weather_Description == "Parcialmente nublado"){
  //       return &img_icon_02n_72p;
  //     }else if(str_Weather_Description == "Possibilidade de chuva irregular"){
  //       return &img_icon_10n_72p;
  //     }else{
  //       return &img_icon_01n_72p;
  //     }
  //   }
  // }

  // if(str_Weather_Description == "Nublado"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_04d_04n_72p);
  // }else if(str_Weather_Description == "Encoberto"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_03d_03n_72p);
  // }else if(str_Weather_Description == "Névoa"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_50d_50n_72p);
  // }else if(str_Weather_Description == "Chuva forte"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_09d_09n_72p);
  // }else if(str_Weather_Description == "Tempestade"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_11d_11n_72p);
  // }else if(str_Weather_Description == "Neve"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_13d_13n_72p);
  // }else{
  //   if(str_Day_Night == 1){
  //     //Dia
  //     if(str_Weather_Description == "Céu limpo"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01d_72p);
  //     }else if(str_Weather_Description == "Parcialmente nublado"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_02d_72p);
  //     }else if(str_Weather_Description == "Possibilidade de chuva irregular"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_10d_72p);
  //     }else{
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01d_72p);
  //     }
  //   }else{
  //     //Noite
  //     if(str_Weather_Description == "Céu limpo"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01n_72p);
  //     }else if(str_Weather_Description == "Parcialmente nublado"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_02n_72p);
  //     }else if(str_Weather_Description == "Possibilidade de chuva irregular"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_10n_72p);
  //     }else{
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01n_72p);
  //     }
  //   }
  // }

}   

void update_DateTime() {  
  //Horario
  std_time = time(rtc);
  lv_label_set_text(objects.label_time, std_time.c_str());

  //Data e Dia da Semana atual
  std_data = data(rtc);
  lv_label_set_text(objects.label_date, std_data.c_str());
  int_dayWeek = dia_semana(rtc, "");
  lv_label_set_text(objects.label_day, diasSemana[int_dayWeek]);
  
  // Dias e data da Semana
  std_data1 = doc_API_Open_Meteor["daily"]["time"][1].as<String>();
  str_data_formatada = format_data(std_data1);
  lv_label_set_text(objects.label_date1, str_data_formatada.c_str());  
  int_dayWeek1 = dia_semana(rtc, std_data1);
  lv_label_set_text(objects.label_day1, diasSemana[int_dayWeek1]);

  std_data2 = doc_API_Open_Meteor["daily"]["time"][2].as<String>();
  str_data_formatada = format_data(std_data2);
  lv_label_set_text(objects.label_date2, str_data_formatada.c_str());  
  int_dayWeek2 = dia_semana(rtc, std_data2);
  lv_label_set_text(objects.label_day2, diasSemana[int_dayWeek2]);

  std_data3 = doc_API_Open_Meteor["daily"]["time"][3].as<String>();
  str_data_formatada = format_data(std_data3);
  lv_label_set_text(objects.label_date3, str_data_formatada.c_str());  
  int_dayWeek3 = dia_semana(rtc, std_data3);
  lv_label_set_text(objects.label_day3, diasSemana[int_dayWeek3]);

  std_data4 = doc_API_Open_Meteor["daily"]["time"][4].as<String>();
  str_data_formatada = format_data(std_data4);
  lv_label_set_text(objects.label_date4, str_data_formatada.c_str());  
  int_dayWeek4 = dia_semana(rtc, std_data4);
  lv_label_set_text(objects.label_day4, diasSemana[int_dayWeek4]);

  std_data5 = doc_API_Open_Meteor["daily"]["time"][5].as<String>();
  str_data_formatada = format_data(std_data5);
  lv_label_set_text(objects.label_date5, str_data_formatada.c_str());  
  int_dayWeek5 = dia_semana(rtc, std_data5);
  lv_label_set_text(objects.label_day5, diasSemana[int_dayWeek5]);

  std_data6 = doc_API_Open_Meteor["daily"]["time"][6].as<String>();
  str_data_formatada = format_data(std_data6);
  lv_label_set_text(objects.label_date6, str_data_formatada.c_str());  
  int_dayWeek6 = dia_semana(rtc, std_data6);
  lv_label_set_text(objects.label_day6, diasSemana[int_dayWeek6]);

  std_data7 = doc_API_Open_Meteor["daily"]["time"][7].as<String>();
  str_data_formatada = format_data(std_data7);
  lv_label_set_text(objects.label_date7, str_data_formatada.c_str());  
  int_dayWeek7 = dia_semana(rtc, std_data7);
  lv_label_set_text(objects.label_day7, diasSemana[int_dayWeek7]);
}

void update_Data() {
  // Zona
  gmtOffset_sec = doc_API_Open_Meteor["utc_offset_seconds"];
  Serial.print("Zona: ");Serial.print(gmtOffset_sec);

  // Local
  str_city = doc_API_Wheater["location"]["name"].as<String>();
  str_uf = doc_API_Wheater["location"]["region"].as<String>();
  str_label_city = str_city + " / " + str_uf;
  lv_label_set_text(objects.label_city, str_label_city.c_str());  

  // Temperatura geral    
  str_type_temperature = doc_API_Open_Meteor["current_units"]["temperature_2m"].as<String>();  
  if(str_type_temperature == "" || str_type_temperature == " "){
    str_type_temperature = "°C";
  }
  lv_label_set_text(objects.label_type_temperature, str_type_temperature.c_str());
  lv_label_set_text(objects.label_type_temperature1, str_type_temperature.c_str());
  lv_label_set_text(objects.label_type_temperature2, str_type_temperature.c_str());
  lv_label_set_text(objects.label_type_temperature3, str_type_temperature.c_str());

  // Definições do Dia Atual  
  str_Weather_Description = doc_API_Wheater["current"]["condition"]["text"].as<String>();
  lv_label_set_text(objects.label_weather_description, str_Weather_Description.c_str());

  if(str_type_temperature == "F"){
    //Temperatura dia atual    
    str_Temperature = doc_API_Wheater["current"]["temp_f"].as<String>();
    str_Feels_Like = doc_API_Wheater["current"]["feelslike_f"].as<String>();
    str_Temp_Max = doc_API_Wheater["forecast"]["forecastday"][0]["day"]["maxtemp_f"].as<String>();
    str_Temp_Min = doc_API_Wheater["forecast"]["forecastday"][0]["day"]["mintemp_f"].as<String>();

    //Temperatura dos 7 dias da semana
    str_Temp_Max1 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][1].as<String>();
    str_Temp_Min1 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][1].as<String>();
    str_Temp_Max2 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][2].as<String>();
    str_Temp_Min2 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][2].as<String>();
    str_Temp_Max3 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][3].as<String>();
    str_Temp_Min3 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][3].as<String>();
    str_Temp_Max4 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][4].as<String>();
    str_Temp_Min4 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][4].as<String>();
    str_Temp_Max5 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][5].as<String>();
    str_Temp_Min5 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][5].as<String>();
    str_Temp_Max6 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][6].as<String>();
    str_Temp_Min6 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][6].as<String>();
    str_Temp_Max7 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][7].as<String>();
    str_Temp_Min7 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][7].as<String>();

  }else{
    //Temperatura dia atual
    str_Temperature = doc_API_Wheater["current"]["temp_c"].as<String>();
    str_Feels_Like = doc_API_Wheater["current"]["feelslike_c"].as<String>();
    str_Temp_Max = doc_API_Wheater["forecast"]["forecastday"][0]["day"]["maxtemp_c"].as<String>();
    str_Temp_Min = doc_API_Wheater["forecast"]["forecastday"][0]["day"]["mintemp_c"].as<String>();

    //Temperatura dos 7 dias da semana
    str_Temp_Max1 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][1].as<String>() + "°";
    str_Temp_Min1 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][1].as<String>() + "°";
    str_Temp_Max2 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][2].as<String>() + "°";
    str_Temp_Min2 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][2].as<String>() + "°";
    str_Temp_Max3 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][3].as<String>() + "°";
    str_Temp_Min3 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][3].as<String>() + "°";
    str_Temp_Max4 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][4].as<String>() + "°";
    str_Temp_Min4 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][4].as<String>() + "°";
    str_Temp_Max5 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][5].as<String>() + "°";
    str_Temp_Min5 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][5].as<String>() + "°";
    str_Temp_Max6 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][6].as<String>() + "°";
    str_Temp_Min6 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][6].as<String>() + "°";
    str_Temp_Max7 = doc_API_Open_Meteor["daily"]["temperature_2m_max"][7].as<String>() + "°";
    str_Temp_Min7 = doc_API_Open_Meteor["daily"]["temperature_2m_min"][7].as<String>() + "°";
  }
  lv_label_set_text(objects.label_temperature, str_Temperature.c_str());
  lv_label_set_text(objects.label_feels_like, str_Feels_Like.c_str());
  lv_label_set_text(objects.label_temp_max, str_Temp_Max.c_str());
  lv_label_set_text(objects.label_temp_min, str_Temp_Min.c_str());
  lv_label_set_text(objects.label_maxtemperature1, str_Temp_Max1.c_str());
  lv_label_set_text(objects.label_mintemperature1, str_Temp_Min1.c_str());
  lv_label_set_text(objects.label_maxtemperature2, str_Temp_Max2.c_str());
  lv_label_set_text(objects.label_mintemperature2, str_Temp_Min2.c_str());
  lv_label_set_text(objects.label_maxtemperature3, str_Temp_Max3.c_str());
  lv_label_set_text(objects.label_mintemperature3, str_Temp_Min3.c_str());
  lv_label_set_text(objects.label_maxtemperature4, str_Temp_Max4.c_str());
  lv_label_set_text(objects.label_mintemperature4, str_Temp_Min4.c_str());
  lv_label_set_text(objects.label_maxtemperature5, str_Temp_Max5.c_str());
  lv_label_set_text(objects.label_mintemperature5, str_Temp_Min5.c_str());
  lv_label_set_text(objects.label_maxtemperature6, str_Temp_Max6.c_str());
  lv_label_set_text(objects.label_mintemperature6, str_Temp_Min6.c_str());
  lv_label_set_text(objects.label_maxtemperature7, str_Temp_Max7.c_str());
  lv_label_set_text(objects.label_mintemperature7, str_Temp_Min7.c_str());

  str_Humidity = doc_API_Wheater["current"]["humidity"].as<String>() + "%"; 
  str_Wind_Speed = doc_API_Wheater["current"]["wind_kph"].as<String>()  + " km/h";
  lv_label_set_text(objects.label_humidity, str_Humidity.c_str());
  lv_label_set_text(objects.label_wind, str_Wind_Speed.c_str());
}

void update_Weather_Data(){ 
  // Imagens Fixas
  lv_img_set_src(objects.image_icon_city, &img_icon_location_11p);
  lv_img_set_src(objects.image_icon_temp, &img_icon_temp_22p);
  lv_img_set_src(objects.image_icon_temp_max, &img_icon_max_11p);
  lv_img_set_src(objects.image_icon_temp_min, &img_icon_min_11p);
  lv_img_set_src(objects.image_icon_humidity, &img_icon_humd_22p);
  lv_img_set_src(objects.image_icon_wind, &img_icon_wind_11p);

  //Imagens dinamicas   
  //Imagens do dia dinamico  
  str_Day_Night = doc_API_Wheater["current"]["is_day"];
  type_clima = doc_API_Wheater["current"]["condition"]["text"].as<String>();  
  lv_img_set_src(objects.image_icon_weather, &img_icon_09d_09n_72p); 
  //lv_img_set_src(objects.image_icon_weather, get_weather_day_icon(type_clima, str_Day_Night)); 
  // if(strcmp(str_Weather_Description.c_str(), "Nublado") == 0){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_04d_04n_72p);
  // }else if(strcmp(str_Weather_Description.c_str(), "Encoberto") == 0){
  //    lv_img_set_src(objects.image_icon_weather, &img_icon_03d_03n_72p);
  // }else if(str_Weather_Description == "Névoa"){
  // lv_img_set_src(objects.image_icon_weather, &img_icon_50d_50n_72p);
  // }else if(str_Weather_Description == "Chuva forte"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_09d_09n_72p);
  // }else if(str_Weather_Description == "Tempestade"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_11d_11n_72p);
  // }else if(str_Weather_Description == "Neve"){
  //   lv_img_set_src(objects.image_icon_weather, &img_icon_13d_13n_72p);
  // }else{
  //   str_Day_Night = doc_API_Wheater["current"]["is_day"];
  //   if(str_Day_Night == 1){
  //     //Dia
  //     if(str_Weather_Description == "Céu limpo"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01d_72p);
  //     }else if(str_Weather_Description == "Parcialmente nublado"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_02d_72p);
  //     }else if(str_Weather_Description == "Possibilidade de chuva irregular"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_10d_72p);
  //     }else{
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01d_72p);
  //     }
  //   }else{
  //     //Noite
  //     if(str_Weather_Description == "Céu limpo"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01n_72p);
  //     }else if(str_Weather_Description == "Parcialmente nublado"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_02n_72p);
  //     }else if(str_Weather_Description == "Possibilidade de chuva irregular"){
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_10n_72p);
  //     }else{
  //       lv_img_set_src(objects.image_icon_weather, &img_icon_01n_72p);
  //     }
  //   }
  // }

  
  //Imagens dos dias da semana dinamico - (Imagens de Dia)   
  cod_type_clima1 = doc_API_Open_Meteor["daily"]["weather_code"][1];  
  cod_type_clima2 = doc_API_Open_Meteor["daily"]["weather_code"][2];
  cod_type_clima3 = doc_API_Open_Meteor["daily"]["weather_code"][3];
  cod_type_clima4 = doc_API_Open_Meteor["daily"]["weather_code"][4];
  cod_type_clima5 = doc_API_Open_Meteor["daily"]["weather_code"][5];
  cod_type_clima6 = doc_API_Open_Meteor["daily"]["weather_code"][6];
  cod_type_clima7 = doc_API_Open_Meteor["daily"]["weather_code"][7]; 
  lv_img_set_src(objects.image_icon_weather1, get_weather_days_week_icon(cod_type_clima1));
  lv_img_set_src(objects.image_icon_weather2, get_weather_days_week_icon(cod_type_clima2));
  lv_img_set_src(objects.image_icon_weather3, get_weather_days_week_icon(cod_type_clima3));
  lv_img_set_src(objects.image_icon_weather4, get_weather_days_week_icon(cod_type_clima4));
  lv_img_set_src(objects.image_icon_weather5, get_weather_days_week_icon(cod_type_clima5));
  lv_img_set_src(objects.image_icon_weather6, get_weather_days_week_icon(cod_type_clima6));
  lv_img_set_src(objects.image_icon_weather7, get_weather_days_week_icon(cod_type_clima7));  
}


/**************************************************************************************/
/********************************* INICIANDO O SETUP  *********************************/
/**************************************************************************************/
void setup() {
  setlocale(LC_ALL, "Portuguese");
  // Inicializa o serial
  Serial.begin(115200); 
  delay(500);

  //Inicializar sensor
  setupSensor();

  // Inicializando leds
  pinMode(LED_ONOFF, OUTPUT);
  digitalWrite(LED_ONOFF, HIGH);

  //Inicializando modulo RTC
  rtc.begin();

  // Inicilizando Pins para luz de fundo da tela
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // Inicialização Gráfica
  tft.init();  

  //Inicializar e configurando a biblioteca Gráfica da UI 
  lv_init();
  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  if (!draw_buf) {
    Serial.println("Erro ao alocar draw_buf!");
  }  
  disp = lv_tft_espi_create(SCREEN_HEIGHT, SCREEN_WIDTH, draw_buf, DRAW_BUF_SIZE);  
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);  
  ui_init();

  // Inicializa o váriaveis Gráficos em tela  
  set_initial_display_of_labels();
  delay(1000);

  // Conectando ao WiFi e chamando API Principal
  connect_wifi();

  //Buscando dados da API  
  doc_API_Open_Meteor = dados_API_Open_Meteor();
  doc_API_Wheater = dados_API_Weather();

  //Iniciando Relogio: Data e hora de acordo com o servidor 
  update_DateTime();  

  // Atualizando dados do Tempo
  update_Data(); 

  // Atualizar imagens do clima  
  //free(objects.image_icon_weather);
  update_Weather_Data();
  
}


/**************************************************************************************/
/********************************* INICIANDO O LOOP  *********************************/
/**************************************************************************************/
void loop() {
  unsigned long currentMill = millis();
  if (currentMill - prevMill >= intervalMill) {
    prevMill = currentMill;
    //Chamar a sub-rotina update_DateTime()
    update_DateTime();
    cnt++;
    if (cnt > INTERVAL_UPDATING_WEATHER_DATA) {
      cnt = 0;      
      //Chamar a sub-rotina update_Weather_Data() e update_Data();
      update_Weather_Data();
      update_Data(); 
    }
  }
 
  acionarLEDsSequenciais();
  update_UI();
  delay(1);  
}
