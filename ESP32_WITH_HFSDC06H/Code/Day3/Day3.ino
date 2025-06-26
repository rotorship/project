#define RXD2 21  // Sensor TX → ESP32 GPIO 16
#define TXD2 5  // Sensor RX → ESP32 GPIO 17 (optional)

void setup() {
  Serial.begin(115200);                         // Serial monitor
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // UART2 to sensor
  Serial.println("MR24D11C10 Presence Detection Started");
}

void loop() {
  static uint8_t buffer[8];
  static int index = 0;

  while (Serial2.available()) {
    uint8_t byteRead = Serial2.read();

    if (index == 0 && byteRead != 0x55) continue; // Wait for 0x55
    if (index == 1 && byteRead != 0xAA) {
      index = 0;
      continue; // Wait for 0xAA
    }

    buffer[index++] = byteRead;

    if (index == 8) {
      index = 0;

      uint8_t presence = buffer[2]; // 0x01 = detected, 0x00 = no presence

      if (presence == 0x01) {
        Serial.println("🔵 Presence Detected");
      } else if (presence == 0x00) {
        Serial.println("⚪ No Presence");
      } else {
        Serial.print("Unknown Status Byte: ");
        Serial.println(presence, HEX);
      }
    }
  }
}
