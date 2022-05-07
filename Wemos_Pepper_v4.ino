/*
  Malte Wessel
  Pepper (Softbank Robotics / Entrance Robotics über Knöpfe ansteuern
*/

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define P_SWITCH D4  // Schalter an Pin D4

// WLAN-Verbindung
WiFiClient client;             // WiFiClient Objekt erstellen
const char* ssid = "XXX";      // WiFi Netzwerk
const char* password = "XXX";  // WiFi Passwort

// Verbindung zu Pepper
char HOST_NAME[] = "XXX";              // Peppers IP [192.168.178.28]
int HTTP_PORT = 36000;                 // Port auf den Pepper lauscht [36000]
String HTTP_METHOD = "POST";           // "GET" oder ["POST"]
String HTTP_VERSION = "HTTP/1.1";      // Version 1.0 oder [1.1]
String PATH_NAME = "/";                // PATH-Part of URL
String QUERYSTRING = "value=button4";  // button 1, 2, 3, 4

// SETUP
void setup() {
  Serial.begin(115200);
  pinMode(P_SWITCH, INPUT);

  // Stelle WLAN-Verbindung her
  WiFi.begin(ssid, password);
  Serial.print("\n:: Connecting ");
  // Warte, bis die Verbindung geklappt hat
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Infos in der Konsole ausgeben
  Serial.print("\n:: Connected to WiFi network. \n>> ");
  Serial.print(WiFi.localIP());
}

// Hauptprogramm
void loop() {
  // Überprüfen ob Knopf gedrückt wurde
  if (!digitalRead(P_SWITCH)) {
    Serial.print("\n:: Button pushed");

    // mit Webserver von Pepper verbinden
    if (client.connect(HOST_NAME, HTTP_PORT)) {
      // Infos in der Konsole ausgeben
      Serial.print("\n:: Connected to webserver \n>> ");
      Serial.print(HOST_NAME);
      Serial.print("\n>> " + String(HTTP_PORT));

      // HTTP header an Webserver senden
      client.println(HTTP_METHOD + " " + PATH_NAME + " " + HTTP_VERSION);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Connection: close");
      client.println();
      // HTTP body an Webserver senden
      client.println(QUERYSTRING);
      // Infos in der Konsole ausgeben
      Serial.print("\n>> " + QUERYSTRING + "\n");

    } else {
      // Bei Verbindungsproblem, Info in die Konsole
      Serial.println("\n!! Connection failed");
    }

    // auf Antwort vom Webserver warten
    while (client.connected() && !client.available()) delay(1);
    // Antwort vom Webserver in der Konsole ausgeben
    while (client.available()) {
      Serial.print(char(client.read()));
    }

    // von Webserver trennen
    Serial.println("\n:: Disconnecting from webserver");
    client.stop();

    // 2 Sekunden warten
    delay(2000);
  }

  // kurze Wartezeit bis der Zustand des Schalters wieder geprüft wird
  delay(100);
}
