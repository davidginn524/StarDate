#include <NTPtimeESP.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>   
#include <DNSServer.h>
#define PIN            D8

NTPtime NTPch("pool.ntp.org"); // Choose server pool as required
strDateTime dateTime;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
short jan = 0;
short feb = 31;
short mar = 59;
short apr = 90;
short may = 120;
short jun = 151;
short jul = 181;
short aug = 212;
short sep = 243;
short oct = 273;
short nov = 304;
short dec = 334;
int stardate;// final date to display
short stardateunit = 1000; // 1 year = 1000 star units
int basedate = 58000; //this starts at year 2005
short baseyear = 2005;//base year based on new movies time. Not all timelines sync up
short starday; //current day 
short staryear; //current year
short starmonth;//current month 
//change update values if debugging is on to make debugging quicker
const unsigned long period = 10000;
byte second, minute, hour, dayOfWeek, month, year, day;
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

int flag = 0; //used for display effects to stop it showing more than once

int dayBrightness = 100;
int nightBrightness = 30;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");
// We start by connecting to a WiFi network
  WiFiManager MyWifiManager;
  MyWifiManager.autoConnect("Word Clock");
  Serial.println("WiFi connected");

  startMillis = millis();  //initial start time

}

void loop()
{

  displayTime(); // display the real-time clock data on the Serial Monitor  and the LEDS,
}

void TimeOfDay() {
  //Used to set brightness dependant of time of day - lights dimmed at night

  //monday to thursday and sunday

  if ((dayOfWeek == 6) | (dayOfWeek == 7)) {
    if ((hour > 0) && (hour < 8)) {
    //  pixels.setBrightness(nightBrightness);
    }
    else {
      //pixels.setBrightness(dayBrightness);
    }
  }
  else {
    if ((hour < 6) | (hour >= 22)) {
     // pixels.setBrightness(nightBrightness);
    }
    else {
      //pixels.setBrightness(dayBrightness);
    }
  }
}

void displayTime()
{
  //rate limit the time check 
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println("Time Check rate reached");
    readtime(&second, &minute, &hour, &dayOfWeek, &month, &year, &day);  
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current state.

  }
    if (hour < 10) 
    {
      Serial.print("0");
    }
    Serial.print(hour);
    Serial.print(":");
  
    if (minute < 10) 
    {
      Serial.print("0");
    }
    Serial.println(minute);
    delay(500);

}

void readtime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *month, byte *year, byte *day) 
{

      dateTime = NTPch.getNTPtime(0.0, 1);
      if(dateTime.valid)
      {
        *day = dateTime.day;
        *second = dateTime.second;
        *minute = dateTime.minute;
        *hour = dateTime.hour;
        *dayOfWeek = dateTime.dayofWeek;
        *month = dateTime.month;
        *year = dateTime.year; 
      }
}
