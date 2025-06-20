#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);  // USB Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // FTDI232
  Serial.println("ESP32 ready to receive.");
}

void loop() {
  if (Serial2.available()) {
    String msg = Serial2.readStringUntil('\n');
    Serial.print("FTDI sent: ");
    Serial.println(msg);

    // Echo it back
    Serial2.print("You typed: ");
    Serial2.println(msg);
  }
}
