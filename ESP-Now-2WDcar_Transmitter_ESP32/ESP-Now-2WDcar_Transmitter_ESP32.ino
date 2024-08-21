#include <WiFi.h>
#include <esp_now.h>

// Define pushbutton pins
#define BUTTON_FRONT 32
#define BUTTON_BACK 33
#define BUTTON_LEFT 25
#define BUTTON_RIGHT 26

void setup() {
  Serial.begin(115200);

  // Set button pins as inputs with internal pull-up resistors
  pinMode(BUTTON_FRONT, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  // Set ESP32 in STA mode
  WiFi.mode(WIFI_STA);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer (receiver's MAC address must be known)
  uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xD4, 0x60, 0x48};
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  uint8_t command = 0;

  // Read button states// As ESP32 GPIO has pullup pins, we read the low value when pressed:
  if (digitalRead(BUTTON_FRONT) == LOW) {
    command = 1;
  } else if (digitalRead(BUTTON_BACK) == LOW) {
    command = 2;
  } else if (digitalRead(BUTTON_LEFT) == LOW) {
    command = 3;
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    command = 4;
  } else {
    command = 0;  // Stop
  }

  // Send the command
  esp_now_send(NULL, &command, sizeof(command));

  // Small delay to avoid flooding the receiver with commands
  delay(100);
}
