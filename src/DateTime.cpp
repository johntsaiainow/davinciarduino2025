#include <DateTime.h>
#include <ErriezDS1302.h>

//struct datetime
struct tm dt;
struct tm startDT;

ErriezDS1302 rtc = ErriezDS1302(DS1302_CLK_PIN, DS1302_IO_PIN, DS1302_CE_PIN);

// Define days of the week in flash
const char day_0[] PROGMEM = "Sunday";
const char day_1[] PROGMEM = "Monday";
const char day_2[] PROGMEM = "Tuesday";
const char day_3[] PROGMEM = "Wednesday";
const char day_4[] PROGMEM = "Thursday";
const char day_5[] PROGMEM = "Friday";
const char day_6[] PROGMEM = "Saturday";

const char* const dayWeekTable[] PROGMEM = {
        day_0, day_1, day_2, day_3, day_4, day_5, day_6
};

// Define months in flash
const char month_0[] PROGMEM = "January";
const char month_1[] PROGMEM = "February";
const char month_2[] PROGMEM = "March";
const char month_3[] PROGMEM = "April";
const char month_4[] PROGMEM = "May";
const char month_5[] PROGMEM = "June";
const char month_6[] PROGMEM = "July";
const char month_7[] PROGMEM = "August";
const char month_8[] PROGMEM = "September";
const char month_9[] PROGMEM = "October";
const char month_10[] PROGMEM = "November";
const char month_11[] PROGMEM = "December";

const char* const monthTable[] PROGMEM = {
        month_0, month_1, month_2, month_3, month_4, month_5,
        month_6, month_7, month_8, month_9, month_10, month_11
};

void rtcCheck(void)
{
      // Initialize RTC
  while (!rtc.begin()) {
      Serial.println(F("RTC not found"));
      delay(3000);
  }
  // Read RTC date/time
  if (!rtc.read(&dt)) 
  {
    Serial2.println("Error: RTC read failed");
    delay(3000);
  }
  // Machine Start Date/Time
  startDT = dt;

}

void cmdPrintDateTime(void)
{
    struct tm dt;
    char buf[11];

    // Read date/time from RTC
    if (!rtc.read(&dt)) {
        Serial2.println(F("Error: Read date/time failed"));
        return;
    }

    // Print day of the week, day month, month and year
    strncpy_P(buf, (char *)pgm_read_dword(&(dayWeekTable[dt.tm_wday])), sizeof(buf));
    Serial2.print(buf);
    Serial2.print(F(" "));
    Serial2.print(dt.tm_mday);
    Serial2.print(F(" "));
    strncpy_P(buf, (char *)pgm_read_dword(&(monthTable[dt.tm_mon])), sizeof(buf));
    Serial2.print(buf);
    Serial2.print(F(" "));
    Serial2.print(dt.tm_year + 1900);
    Serial2.print(F(" "));
    snprintf(buf, sizeof(buf), "%d:%02d:%02d", dt.tm_hour, dt.tm_min, dt.tm_sec);
    Serial2.print(buf);
}

String getDateTime()
{
    struct tm dt;
    char buf[11];

    // Read date/time from RTC
    if (!rtc.read(&dt)) {
        //Serial2.println(F("Error: Read date/time failed"));
        return "Error: Read date/time failed";
    }

    strncpy_P(buf, (char *)pgm_read_dword(&(dayWeekTable[dt.tm_wday])), sizeof(buf));
    String dateTimeString = String(buf) + ' ' + String(dt.tm_mday) + ' ';
    strncpy_P(buf, (char *)pgm_read_dword(&(monthTable[dt.tm_mon])), sizeof(buf));
    const int year = dt.tm_year + 1900;
    dateTimeString.concat(String(buf) + ' ' + String(year) + ' ' );
    snprintf(buf, sizeof(buf), "%d:%02d:%02d", dt.tm_hour, dt.tm_min, dt.tm_sec);
    dateTimeString.concat(String(buf));
    // Print day of the week, day month, month and year
    return dateTimeString;
}

void setDateTime(int hour, int min, int sec, int mday, int mon, int year, int wday)
{
    // Write RTC date/time: 13:45:09  31 December 2019  0=Sunday, 2=Tuesday
if (!rtc.setDateTime(hour, min, sec,  mday, mon, year,  wday))
{

    // Error: RTC write failed
    Serial2.println("dateTime FAILED");
}
else
{
    Serial2.println("dateTime set");
}
}

void setEpochTime(int epoch)
{
    rtc.setEpoch(epoch);
}

int getEpochTime()
{
    return rtc.getEpoch();
}