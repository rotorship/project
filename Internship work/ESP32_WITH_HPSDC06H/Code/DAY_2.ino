#define SENSOR_PIN 21

unsigned long startTime = 0;
int motionCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);
  startTime = millis();
}

void loop() {
  int motion = digitalRead(SENSOR_PIN);

  if (motion == HIGH) {
    motionCount++;
    Serial.println("Motion detected");
    delay(200); // basic debounce
  }

  if (millis() - startTime >= 30000) { // 30 second check
    if (motionCount >= 1) {
      Serial.println("✅ Person is ALIVE (Breathing or micro-motion detected)");
    } else {
      Serial.println("❌ No life sign detected (Still for 30s)");
    }

    // Reset
    motionCount = 0;
    startTime = millis();
  }

  delay(50); // sampling interval
}
