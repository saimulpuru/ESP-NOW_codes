#include <WiFi.h>
#include <esp_now.h>

const int lm1 = 25;
const int lm2 = 26;
const int rm1 = 32;
const int rm2 = 33;

void setup() {
  Serial.begin(115200);

  // Set up motor pins as outputs
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);

  // Set ESP32 in STA mode
  WiFi.mode(WIFI_MODE_STA);
  delay(1000);
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  //Enter this MAC address in the transmitter remote control's code

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback function
  esp_now_register_recv_cb(receiveCallback);
}

void receiveCallback(const esp_now_recv_info_t *info, const uint8_t *data, int dataLen) {
  // Assume the received data is a single byte representing the command
  uint8_t btn = data[0];

  if (btn == 1) {
    front();
  } else if (btn == 2) {
    back();
  } else if (btn == 3) {
    left();
  } else if (btn == 4) {
    right();
  } else {
    stop();
  }
}

void front() {
  // Move forward
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
}

void back() {
  // Move backward
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
}

void left() {
  // Turn left
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
}

void right() {
  // Turn right
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
}

void stop() {
  // Stop
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, LOW);
}

void loop() {
  // Nothing to do here, everything is handled in the receive callback
}
