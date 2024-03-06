#include <SoftwareSerial.h>

// Define the pins for SoftwareSerial communication
const int rxPin = D5;  // RX pin of ESP8266 connected to TX pin of Arduino Nano
const int txPin = D6;  // TX pin of ESP8266 connected to RX pin of Arduino Nano

SoftwareSerial arduinoSerial(rxPin, txPin); // Create a SoftwareSerial object

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  arduinoSerial.begin(115200); // Start SoftwareSerial communication with Arduino Nano
}

void loop() {
  if (arduinoSerial.available()) {
    // Read data from Arduino Nano
    String data = arduinoSerial.readStringUntil('\n');
    
    // Print received data
    Serial.println("Received data: " + data);
  }
}
