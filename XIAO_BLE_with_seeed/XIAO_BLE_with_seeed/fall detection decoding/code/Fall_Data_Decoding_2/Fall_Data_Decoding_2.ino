#include <falldetectionradar.h>

FallDetectionRadar radar;

void setup()
{
  radar.SerialInit();
  Serial.begin(9600);
  delay(1500);
  Serial.println("Ready");
}

void loop()
{
  radar.recvRadarBytes();  // Receive and parse data from radar

  if (radar.newData == true) {
    byte dataMsg[radar.dataLen + 2] = {0x00};
    dataMsg[0] = 0x55;

    for (byte n = 0; n < radar.dataLen; n++) {
      dataMsg[n + 1] = radar.Msg[n];
    }

    radar.newData = false;

    // FALL/SLEEP INTERPRETATION
    byte presenceState = dataMsg[4];  // Byte 4: presence info (0 = none, 3 = present)
    byte motionState   = dataMsg[5];  // Byte 5: possibly fall/sleep/motion category
    byte bodySign      = dataMsg[8];  // Byte 8: activity level

    Serial.print("Body Sign Value: ");
    Serial.println(bodySign);

    // Human-readable interpretation
    if (presenceState == 0) {
      Serial.println("No one detected in the environment.");
    } else {
      if (motionState == 1) {
        Serial.println("Human detected: Standing or moving normally.");
      } else if (motionState == 2) {
        Serial.println("Human detected: Possibly lying down.");
      } else if (motionState == 3) {
        Serial.println("Human detected: Person has fallen!");
      } else if (motionState == 4) {
        Serial.println("Human detected: Person is sleeping.");
      } else if (motionState == 6) {
        Serial.println("Human detected: Slight motion.");
      } else {
        Serial.print("Human detected: Unknown motion state ");
        Serial.println(motionState);
      }
    }

    Serial.println();  // Spacing
  }
}
