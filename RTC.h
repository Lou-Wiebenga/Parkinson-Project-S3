#ifndef RTC_H
#define RTC_H

#include <RTClib.h>
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
enum unit_t {Year, Month, Day, Hour, Minutes, Seconds};

void RTC_init();
void RTC_setTimeAuto();
void RTC_setTimeManual(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second);
uint16_t RTC_readTime (unit_t time);
char* RTC_getHhMmString(); //returns a char* storing current hours and minutes in "hh:mm" format

#endif //RTC_H
