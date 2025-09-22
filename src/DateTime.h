#ifndef DATETIME_H
#define DATETIME_H

// Create DS1302 RTC object & Control pins
#define DS1302_CLK_PIN      PC6
#define DS1302_IO_PIN       PC7
#define DS1302_CE_PIN       PC8

#include <Arduino.h>

void rtcCheck(void);
void cmdPrintDateTime(void);
void setDateTime(int hour, int min, int sec, int mday, int mon, int year, int wday);
void setEpochTime(int epoch);
int getEpochTime();
String getDateTime();

#endif