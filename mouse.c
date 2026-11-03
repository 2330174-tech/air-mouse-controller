#include <WiFi.h>      // Use <ESP8266WiFi.h> for ESP8266 boards
#include <WiFiUdp.h>
#include <string.h>

// --- Wi-Fi Configuration ---
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// --- Receiver Configuration ---
IPAddress rx_ip(192, 168, 1, 100);  // IP address of target PC running host receiver
const uint16_t udp_port = 4210;

// --- Pin Setup ---
#define BTN_LEFT  4    // GPIO pin for left click
#define BTN_RIGHT 5    // GPIO pin for right click

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());

  udp.begin(udp_port);
}

void loop() {
  // Replace dx, dy values with accelerometer/gyroscope readings (e.g., MPU6050) or joystick input
  int dx = 0;
  int dy = 0;

  // Active-LOW input (LOW = pressed)
  int leftClick  = (digitalRead(BTN_LEFT) == LOW) ? 1 : 0;
  int rightClick = (digitalRead(BTN_RIGHT) == LOW) ? 1 : 0;

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d", dx, dy, leftClick, rightClick);

  udp.beginPacket(rx_ip, udp_port);
  udp.write((const uint8_t*)buffer, strlen(buffer));
  udp.endPacket();

  delay(10); // ~100 Hz refresh rate
}
