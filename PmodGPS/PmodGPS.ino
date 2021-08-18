#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//librerias wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
#define Rx D3// TXPin
#define Tx D4// RXPin

//static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(Rx, Tx);

//Default is AP mode.
int wifiType = 0; // 0:Station  1:AP

IPAddress staticIP(192,168,1,22);
IPAddress gateway(192,168,1,0);
IPAddress subnet(255,255,255,0);

//Station mode you should put your ssid and password
const char *ssid = "TP-LINK_F90A14"; // Put your SSID here
const char *password = "11135999"; // Put your PASSWORD here

  ESP8266WebServer server(80);

//void handleNotFound(){
//  String message = "Server is running!\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET)?"GET":"POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  server.send(200, "text/plain", message);
//  
//  }

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

//  if (wifiType == 0){
//    if(!strcmp(ssid,"TP-LINK_F90A14")){
//       //Serial.println("Please set your SSID");
//       while(1);
//    }
//    if(!strcmp(password,"11135999")){
//       //Serial.println("Please set your PASSWORD");
//       while(1);
//    }
     WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.config(staticIP, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print(".");
    }
   Serial.println("WiFi connected");
//    Serial.println("");
    Serial.println(WiFi.localIP());
    delay(5000);
//}
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
  //Serial.println(ss.read());
    if (gps.encode(ss.read())){
      Serial.println("-");
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid() ) // && gps.location.isUpdated()
  {   // datos del gps
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

   Serial.print(F("  Velocidad: ")); 
  if (gps.speed.isValid())
  {   // datos de velocidad
    Serial.print(gps.speed.kmph(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {                    // Fecha del gps
    Serial.print(gps.date.month());      
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {      //hora del gps
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
