#include <WiFi.h>
#include <WebServer.h>
#include "DFRobotDFPlayerMini.h"

const char* ssid = "ESP32_Car";    // SSID for your ESP32's Wi-Fi network
const char* password = "12345678";  // Password for the Wi-Fi network

WebServer server(80);

static const uint8_t PIN_MP3_TX = 17; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 16; // Connects to module's TX
HardwareSerial mp3Serial(1);

DFRobotDFPlayerMini player;

int lastCmdInt = 0; // Variable to store the last played command

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Route for root
  server.on("/", HTTP_handleRoot);

  // Start server
  server.begin();
  Serial.println("HTTP server started");

  mp3Serial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  if (player.begin(mp3Serial)) {
    Serial.println("OK");
    // Set volume to maximum (0 to 30).
    player.volume(30);
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

void loop() {
  server.handleClient();
  int cmdInt = server.arg("State").toInt(); // Convert "State" parameter to integer

  if (cmdInt > 0 && cmdInt != lastCmdInt) {
 
    lastCmdInt = cmdInt; // Update the last command

    player.play(cmdInt);
    Serial.println("Playing song: " + String(cmdInt));
  }
}

void HTTP_handleRoot() {
  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}
