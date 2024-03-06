#define CONTACT_SENSOR_COUNT 7

const int contactPins[CONTACT_SENSOR_COUNT] = {2, 3, 4, 5, 6, 7, 8}; // Pins for contact sensors

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  for (int i = 0; i < CONTACT_SENSOR_COUNT; i++) {
    pinMode(contactPins[i], INPUT_PULLUP); // Set pin mode to input with internal pull-up resistor
  }
}

void loop() {
    int contact1 = digitalRead(contactPins[0]); // Read the state of the contact sensor
    int contact2 = digitalRead(contactPins[1]); // Read the state of the contact sensor
    int contact3 = digitalRead(contactPins[2]); // Read the state of the contact sensor
    int contact4 = digitalRead(contactPins[3]); // Read the state of the contact sensor
    int contact5 = digitalRead(contactPins[4]); // Read the state of the contact sensor
    int contact6 = digitalRead(contactPins[5]); // Read the state of the contact sensor
    int contact7 = digitalRead(contactPins[6]); // Read the state of the contact sensor
    
    Serial.print("Contact Sensor 1: ");
    Serial.print(contact1);
    Serial.print(", Contact Sensor 2: ");
    Serial.print(contact2);
    Serial.print(", Contact Sensor 3: ");
    Serial.print(contact3);
    Serial.print(", Contact Sensor 4: ");
    Serial.print(contact4);
    Serial.print(", Contact Sensor 5: ");
    Serial.print(contact5);
    Serial.print(", Contact Sensor 6: ");
    Serial.print(contact6);
    Serial.print(", Contact Sensor 7: ");
    Serial.print(contact7);
    Serial.println();
    delay(100); // Add a small delay for stability
}
