#define SENSOR_PIN 23

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  int value = digitalRead(SENSOR_PIN);
  Serial.println(value);
  delay(500);
}
