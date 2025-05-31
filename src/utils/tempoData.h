#ifndef TEMPODATA_H
#define TEMPODATA_H

#include <string>
#include "RTClib.h"

boolean dataTime_connect(RTC_DS1307 &rtc, int gmtOffset_sec);
std::string data(RTC_DS1307 &rtc);
std::string time(RTC_DS1307 &rtc);
int dia_semana(RTC_DS1307 &rtc, String data);
std::string format_data(String data);


#endif
