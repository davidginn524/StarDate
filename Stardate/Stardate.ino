#include <NTPtimeESP.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>   
#include <DNSServer.h>
//#define PIN            D8
#include "DigitLedDisplay.h"



/* Arduino Pin to Display Pin
   7 to DIN,
   6 to CS,
   5 to CLK */
DigitLedDisplay ld = DigitLedDisplay(D1, D8, D5);

NTPtime NTPch("pool.ntp.org"); // Choose server pool as required
strDateTime dateTime;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
//numbercorrespondingwithmonth
int jan = 0;
int feb = 31;
int mar = 59;
int apr = 90;
int may = 120;
int jun = 151;
int jul = 181;
int aug = 212;
int sep = 243;
int oct = 273;
int nov = 304;
int dec = 334;
int stardate;// final date to display
float stardateunit = 1000; // 1 year = 1000 star units
int basedate = 58000; //this starts at year 2005
int baseyear = 2005;//base year based on new movies time. Not all timelines sync up
int currentdayofmonth; //current day of month in number form
int numbercorrespondingwithmonth;
float daysin1year; //does not account for leap year
int currentyear; //current year
byte currentmonth;//current month 
//change update values if debugging is on to make debugging quicker
const unsigned long period = 10000;
byte second, minute, hour, dayOfWeek, month,  day;
int year;

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
  MyWifiManager.autoConnect("Star Date Clock");
  Serial.println("WiFi connected");

  startMillis = millis();  //initial start time



  /* Set the brightness min:1, max:15 */
  ld.setBright(10);

  /* Set the digit count */
  ld.setDigitLimit(8);
}

void loop()
{
    /* Prints data to the display */


  
  displayTime(); // display the real-time clock data on the Serial Monitor  and the LEDS,
  //formula for stardate basedate + (stardateunit*(currentyear-baseyear)) + ((stardateunit/daysin1year)*(numbercorrespondingwithmonth + currentdayofmonth -1)) 
  currentyear = year;
  currentdayofmonth = day;
  currentmonth = month;
  //check if leap year
  if(year% 4 == 0 && year % 100 !=0 || year % 400 ==0)
  {
   Serial.println("Leap year");
   daysin1year = 366;
   
  }
  else
  {
    Serial.println("Not leap year");
    daysin1year = 365;
  }

  
  if (currentmonth == 1)
  {
    numbercorrespondingwithmonth = jan;
    //Serial.print("Jan");
    }
   else if(currentmonth == 2)
   {
    numbercorrespondingwithmonth = feb;
   // Serial.print("Feb");
    }
   else if(currentmonth == 3)
   {
    numbercorrespondingwithmonth = mar;
    //Serial.print("Mar");
    }
   else if(currentmonth == 4)
   {
    numbercorrespondingwithmonth = apr;
    //Serial.print("Apr");
    }
   else if(currentmonth == 5)
   {
    numbercorrespondingwithmonth = may;
   // Serial.print("May");
    }
   else if(currentmonth == 6)
   {
    numbercorrespondingwithmonth = jun;
    //Serial.print("Jun");
    }
   else if(currentmonth == 7)
   {
    numbercorrespondingwithmonth = jul;
    //Serial.print("Jul");
    }
   else if(currentmonth == 8)
   {
    numbercorrespondingwithmonth = aug;
    //Serial.print("Aug");
    }
   else if(currentmonth == 9)
   {
    numbercorrespondingwithmonth = sep;
   // Serial.print("Sep");
    }
   else if(currentmonth == 10 )
   {
    numbercorrespondingwithmonth = oct;
    //Serial.print("Oct");
    }
   else if(currentmonth == 11)
   {
    numbercorrespondingwithmonth = nov;
    //Serial.print("Nov");
    }
   else
   {
    numbercorrespondingwithmonth = dec;
    //Serial.print("Dec");
    }
     stardate = (basedate + (stardateunit*(currentyear-baseyear)) + ((stardateunit/daysin1year)*(numbercorrespondingwithmonth + currentdayofmonth -1)));
     Serial.println(stardate);
    
//    =  * dayofmonthtotal;

  ld.printDigit(stardate);
  delay(10000);
  Serial.println("display");
  ld.clear();

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

void readtime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *month, int *year, byte *day) 
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
