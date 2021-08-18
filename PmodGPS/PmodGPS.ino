#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//librerias wifi

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
 
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

 float latitud;
 float longitud;
 float velocidad;

 String datos="";
  
  ESP8266WebServer server(80);

void handleRoot() 
{
//   server.send(200, "text/plain", "Hola mundo!");
//    server.send(200, "text/plain", "---->");
    datos=String(latitud,6)+","+String(longitud,6)+","+String(velocidad,6);
     server.send(200, "text/plain",datos );
}

void handleNotFound() 
{
   server.send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();


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
    // Start the server
  
  // Ruteo para '/'
   server.on("/", handleRoot);
 
   // Ruteo para '/inline' usando función lambda
   server.on("/inline", []() {
      server.send(200, "text/plain", "Esto tambien funciona");
   });
 
   // Ruteo para URI desconocida
   server.onNotFound(handleNotFound);
 
   // Iniciar servidor
   server.begin();
   Serial.println("HTTP server started");
//}
}

void loop()
{
  
  server.handleClient();

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
    latitud=gps.location.lat();
    Serial.print(latitud, 6);
    Serial.print(F(","));
    longitud=gps.location.lng();
    Serial.print(longitud, 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

   Serial.print(F("  Velocidad: ")); 
  if (gps.speed.isValid())
  {   // datos de velocidad
    velocidad=gps.speed.kmph();
    Serial.print(velocidad, 6);
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
