#define HFS_PIN 23      // HFS-DC06H OUT pin
#define ZH100_PIN 18    // ZH-100 OUT pin

void setup() {
  Serial.begin(115200);
  pinMode(HFS_PIN, INPUT);
  pinMode(ZH100_PIN, INPUT);
}

void loop() {
  int hfsStatus = digitalRead(HFS_PIN);
  int zhStatus = digitalRead(ZH100_PIN);

  if (hfsStatus == HIGH) {
    Serial.println("Motion detected: Possibly a human or toy moved.");
    delay(1000);  // Wait for things to settle
    zhStatus = digitalRead(ZH100_PIN); // Check breathing
    if (zhStatus == HIGH) {
      Serial.println("Breathing detected: It's a HUMAN.");
    } else {
      Serial.println("No breathing: Could be a TOY or doll.");
    }
  } else {
    Serial.println("No motion detected.");
    Serial.print("Breathing check: ");
    if (zhStatus == HIGH) {
      Serial.println("Breathing detected. HUMAN.");
    } else {
      Serial.println("No micro-movement. Probably TOY.");
    }
  }

  delay(5000);  // Check every 5 seconds
}
