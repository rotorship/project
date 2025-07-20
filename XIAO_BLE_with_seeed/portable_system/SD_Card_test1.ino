#include <falldetectionradar.h>
#include <SPI.h>
#include <SD.h>

FallDetectionRadar radar;

const int chipSelect = 2; // CS pin for SD card

void setup() {
  radar.SerialInit();           // Sets up Serial1 for radar
  Serial.begin(9600);
  delay(1500);
  Serial.println("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (true); // Halt if SD fails
  }

  Serial.println("SD card initialized.");
  Serial.println("System Ready");
}

void loop() {
  radar.recvRadarBytes();  // Receives data from radar

  if (radar.newData == true) {
    byte dataMsg[radar.dataLen + 1] = {0x00};
    dataMsg[0] = 0x55;

    for (byte n = 0; n < radar.dataLen; n++) {
      dataMsg[n + 1] = radar.Msg[n];
    }

    radar.newData = false;

    // Show on Serial
    radar.ShowData(dataMsg);
    radar.Fall_inf(dataMsg);

    // Log to SD card
    File logFile = SD.open("falllog.txt", FILE_WRITE);
    if (logFile) {
      unsigned long now = millis();
      logFile.print("Time: ");
      logFile.print(now);
      logFile.print(" ms | Frame: ");
      for (byte i = 0; i < radar.dataLen + 1; i++) {
        logFile.print("0x");
        if (dataMsg[i] < 16) logFile.print("0");
        logFile.print(dataMsg[i], HEX);
        logFile.print(" ");
      }
      logFile.println();
      logFile.close();
    } else {
      Serial.println("Failed to open SD file.");
    }
  }
}
