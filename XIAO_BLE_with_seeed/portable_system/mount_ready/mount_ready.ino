#include <falldetectionradar.h>
#include <SPI.h>
#include <SD.h>

// An instance of the radar library
FallDetectionRadar radar;

// --- Configuration ---
const int chipSelect = 2;         // The pin for the SD card's CS line.
const int ledPin = LED_BUILTIN;   // Use the built-in LED for visual feedback.
const int maxSamples = 1000;       // The total number of samples to collect.
const unsigned long sampleInterval = 1000; // The delay between samples (500 ms).

// --- Global Variables ---
File logFile;                     // Global variable for the log file.
int sampleCount = 0;              // A counter for the samples collected.
unsigned long lastSampleTime = 0; // Stores the time the last sample was taken.

void setup() {
  // Initialize the LED pin and turn it off initially
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Start serial communication for initial debugging
  Serial.begin(9600);

  // Initialize the radar sensor
  radar.SerialInit();
  delay(1500); // Wait for the sensor to stabilize
  Serial.println("Radar sensor initialized.");

  // --- SD Card Initialization ---
  Serial.print("Initializing SD card... ");
  if (!SD.begin(chipSelect)) {
    Serial.println("FAILED!");
    Serial.println("Check wiring, SD card format (must be FAT32), or try a different card.");
    // Blink the LED rapidly to indicate a critical error
    while (1) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
  }
  Serial.println("SUCCESS.");

  // --- File Creation ---
  logFile = SD.open("timedlog.csv", FILE_WRITE);
  if (logFile) {
    logFile.println("Sample,Time_ms,Raw_Bytes");
    logFile.flush();
    Serial.println("timedlog.csv is open and the header has been written.");
  } else {
    Serial.println("Failed to create or open timedlog.csv!");
    while (1); // Halt execution if the file cannot be opened
  }

  Serial.println("Starting data collection...");
}

void loop() {
  // First, check if the data collection is complete
  if (sampleCount >= maxSamples) {
    Serial.println("-------------------------------------------------");
    Serial.println("✅ Sample collection complete: 300 samples.");
    
    logFile.close();
    Serial.println("Log file closed. Safe to remove SD card.");
    
    digitalWrite(ledPin, HIGH); // Turn LED on permanently to show completion
    while (1); // Stop the program
  }

  // Always listen for new data from the radar in the background
  radar.recvRadarBytes();

  // Check if it's time to take a new sample
  if (millis() - lastSampleTime >= sampleInterval) {
    lastSampleTime = millis(); // Reset the timer

    // Check if new, valid data is available from the radar at this moment
    if (radar.newData && radar.dataLen >= 6 && radar.dataLen <= 12) {
      sampleCount++; // Increment the sample counter
      
      // Blink the LED to confirm a sample was taken
      digitalWrite(ledPin, HIGH);
      
      // --- CHANGE: Print sample count and raw data to Serial Monitor ---
      Serial.print("Sample ");
      Serial.print(sampleCount);
      Serial.print("/");
      Serial.print(maxSamples);
      Serial.print(" -> 55 "); // Print the static start byte
      for (int i = 0; i < radar.dataLen; i++) {
        Serial.print(radar.Msg[i]);
        Serial.print(" ");
      }
      Serial.println();

      // Log data to SD Card
      logFile.print(sampleCount);
      logFile.print(",");
      logFile.print(millis());
      logFile.print(",55");
      for (int i = 0; i < radar.dataLen; i++) {
        logFile.print(" ");
        logFile.print(radar.Msg[i]);
      }
      logFile.println();
      
      // Optional: Flush data to the card every 10 samples for safety
      if (sampleCount % 10 == 0) {
        logFile.flush();
      }

      radar.newData = false; // Reset the data flag
      
      // Keep the LED on for a short time, then turn it off
      delay(50);
      digitalWrite(ledPin, LOW);

    } 
    // --- CHANGE: The "else" block that printed "No new data..." has been removed.
  }
}