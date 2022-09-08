#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// config Rede
const char *ssid = "PROF.RAFAEL"; // Roteador
const char *password = "";
IPAddress ip(192, 168, 1, 111);     // ip do dispositivo
IPAddress gateway(192, 168, 1, 1);  // gateway
IPAddress subnet(255, 255, 255, 0); // mascara

ESP8266WebServer server(80);
String page;

// porta digital
const int LED = 5;
const int sensor = 4;
unsigned long evento = 0;
int counter = 0;
int statec = 0;
String state;

void setup()
{

  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // habilitar porta

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  server.on("/", []()
            {
     page = "<!DOCTYPE html> \
             <html> \
             <head> <meta http-equiv='refresh' content='0' </head> \
             <body> \
             <h1>THE CLAPPER</h1><h3>Status:</h3> <h4> "+String(state)+"</h4> \
             </body>\
             </html>";
    server.send(200, "text/html", page); });
}

void loop()
{

  int output = digitalRead(sensor);
  if (output == HIGH)
  {
    if (millis() - evento > 25)
    {
      Serial.println("Reconheci o som");
      Serial.println(millis());
      Serial.println(evento);
      Serial.println(millis() - evento);
      Serial.println(counter);
      if ((counter & 2) == 0)
      {
        digitalWrite(LED, LOW);
        counter++;
        statec = 0;
      }
      else
      {
        digitalWrite(LED, HIGH);
        counter = 1;
        statec = 1;
      }
      if (statec == 0)
      {
        state = "Off";
      }
      else if (statec == 1)
      {
        state = "ON";
      }
    }
    evento = millis();
  }
  server.handleClient();
}