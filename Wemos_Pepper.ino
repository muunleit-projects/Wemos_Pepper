/*
  Control Pepper by wifi-connected d1 mini ESP8266
  from Malte Wessel
*/

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/*
 * for DEBUG-Mode set "true"
 */
#define DEBUGMODE false

// Switch is connected to Pin D4
#define P_SWITCH D4

/*
 * WiFi-Connection
 */
// Creating a WiFiClient object
WiFiClient client;
// SSID of WiFi-Network
const char* ssid = "XXX";
// Password (if needed) of WiFi-Network
const char* password = "XXX";

/*
 * Connection to Pepper
 */
// The IP address of Pepper
const char HOST_NAME[] = "XXX.XXX.XXX.XXX";
// The port on which Pepper listens
const int HTTP_PORT = 36000;
// use HTTP- "GET" or "POST"
const String HTTP_METHOD = "POST";
// The HTTP version "1.0" or "1.1"
const String HTTP_VERSION = "HTTP/1.1";
// The path to the script on Peppers webserver
const String PATH_NAME = "/";
// Sending a value to the server [1, 2, 3, 4]
const String QUERYSTRING = "value=button4";

/*
 * SETUP
 */
void setup() {
  // Set baud rate of the serial connection
  if (DEBUGMODE) Serial.begin(115200);

  // Set the pin mode of the switch to input
  pinMode(P_SWITCH, INPUT);

  // Connecting to the WiFi network
  WiFi.begin(ssid, password);
  if (DEBUGMODE) Serial.print("\n:: Connecting ");

  // Checking if the ESP8266 is connected to the WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEBUGMODE) Serial.print(".");
  }

  // print the IP address of the ESP8266 to the serial monitor
  if (DEBUGMODE) {
    Serial.print("\n:: Connected to WiFi network. \n>> ");
    Serial.print(WiFi.localIP())
  };
}

/*
 * Main-Program-Loop
 */
void loop() {
  // Checking if the button is pressed
  if (!digitalRead(P_SWITCH)) {
    if (DEBUGMODE) Serial.print("\n:: Button pushed");

    // Connecting to Peppers webserver
    if (client.connect(HOST_NAME, HTTP_PORT)) {
      // Printing the IP address and port number of Pepper
      if (DEBUGMODE) {
        Serial.print("\n:: Connected to webserver \n>> ");
        Serial.print(HOST_NAME);
        Serial.print("\n>> " + String(HTTP_PORT));
      }

      // Sending the HTTP HEADER to the webserver
      client.println(HTTP_METHOD + " " + PATH_NAME + " " + HTTP_VERSION);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("Connection: close");
      client.println();

      // Sending the HTTP BODY to the webserver
      client.println(QUERYSTRING);
      if (DEBUGMODE) Serial.print("\n>> " + QUERYSTRING + "\n");

    } else {
      // print a failure-message to the serial monitor if connection fails
      if (DEBUGMODE) Serial.println("\n!! Connection failed");
    }

    // Waiting for a response from the webserver
    while (client.connected() && !client.available()) delay(1);

    // print the response from the webserver to the serial monitor
    if (DEBUGMODE) {
      while (client.available()) {
        Serial.print(char(client.read()));
      }
    }

    // Closing the connection to the webserver
    if (DEBUGMODE) Serial.println("\n:: Disconnecting from webserver");
    client.stop();

    // Waiting for 2 seconds
    delay(2000);
  }

  // A short delay until the state of the switch is checked again
  delay(100);
}
