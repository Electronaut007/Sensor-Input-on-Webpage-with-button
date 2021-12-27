#include<Arduino.h>
#include<DHT.h>
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<Adafruit_Sensor.h>
#include "WiFiManager.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#define DHTTYPE DHT11
const int button= 16;
int temp= 0;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

ESP8266WebServer Server(80);

#define DHTPIN 14

DHT dht(DHTPIN, DHTTYPE);

float Temp;
float Hum;

void Setup()
{
  Serial.begin(9600);
  delay(100);
   WiFiManager wifiManager;
   wifiManager.setAPCallback(configModeCallback);
    if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  } 
  Serial.println("connected...yeey :)");
  pinMode(DHTPIN, INPUT);
  pinMode(button, INPUT);
  dht.begin();

}

void loop()
{
temp= digitalRead(button);
if(temp == HIGH)
{
  Server.handleClient();
}
delay(1000);
}

void handleonconnect()
{
  Temp= dht.readTemperature();
  Hum= dht.readHumidity();
  Server.send(404, "text/plain", "Not Found");
}

String SendHTML(float Tempdata, float Humdata)
{
  String ptr= "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name= \"viewport\" content= \"width= device-width, initial-scale= 1.0, user-scalable= no\">\n";
  ptr += "<title>NodeMCU with DHT11</title>\n";
  ptr +="<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{ margin-top:50px;} h1{color:#444444; margin:50px auto 30px;}\n";
  ptr +="p {font-size= 38px; color: #444444; margin-bottom: 20px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1> NodeMCU Sensor Data</h1>\n";
  ptr +="<p>Temperature:  ";
  ptr +=(int)Tempdata;
  ptr +="Deg Celcius</p>";
  ptr +="<p>Humidity:  ";
  ptr +=(int)Humdata;
  ptr +="</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
