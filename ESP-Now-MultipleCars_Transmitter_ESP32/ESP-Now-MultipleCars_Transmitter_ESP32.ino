#include <WiFi.h>
#include <esp_now.h>

// Define button pins
#define BUTTON_FRONT 32
#define BUTTON_BACK  33
#define BUTTON_LEFT  25
#define BUTTON_RIGHT 26

typedef struct struct_message {
    int btn;
} struct_message;

struct_message myData;

// MAC addresses of the two robot cars
uint8_t broadcastAddress1[] = {0xD8, 0xBC, 0x38, 0xD4, 0x60, 0x48};  // Car 1
uint8_t broadcastAddress2[] = {0xB0, 0xB2, 0x1C, 0xA7, 0x86, 0x58};  // Car 2

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

  // Register first peer (Car 1)
  esp_now_peer_info_t peerInfo1 = {};
  memcpy(peerInfo1.peer_addr, broadcastAddress1, 6);
  peerInfo1.channel = 0;
  peerInfo1.encrypt = false;

  if (esp_now_add_peer(&peerInfo1) != ESP_OK) {
    Serial.println("Failed to add peer 1");
    return;
  }

  // Register second peer (Car 2)
  esp_now_peer_info_t peerInfo2 = {};
  memcpy(peerInfo2.peer_addr, broadcastAddress2, 6);
  peerInfo2.channel = 0;
  peerInfo2.encrypt = false;

  if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
    Serial.println("Failed to add peer 2");
    return;
  }
}

void loop() {
  // Read button states
  if (digitalRead(BUTTON_FRONT) == LOW) {
    myData.btn = 1;  // Front
  } else if (digitalRead(BUTTON_BACK) == LOW) {
    myData.btn = 2;  // Back
  } else if (digitalRead(BUTTON_LEFT) == LOW) {
    myData.btn = 3;  // Left
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    myData.btn = 4;  // Right
  } else {
    myData.btn = 0;  // Stop
  }

  // Send the same data to both cars
  esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));

  if (result1 == ESP_OK && result2 == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error sending the data");
  }

  delay(100);  // Small delay to debounce buttons
}
