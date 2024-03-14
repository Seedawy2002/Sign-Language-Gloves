// // #include <SoftwareSerial.h>
// // #include <ESP8266WiFi.h>
// // #include <Firebase_ESP_Client.h>
// // #include <ArduinoJson.h>
// // #include "addons/RTDBHelper.h"
// // #include "addons/TokenHelper.h"

// // #define API_KEY "AIzaSyBK9R_Mf22n28Y0_vYdRhPom958CCxHtyM"
// // #define DATABASE_URL "https://sign-language-glove-zc-default-rtdb.europe-west1.firebasedatabase.app/"

// // #define   ssid          "Youssef"
// // #define   pass          "12345567890"

// // FirebaseData stream;
// // FirebaseData fbdo;
// // FirebaseAuth auth;
// // FirebaseConfig config;
// // const char* path = "/esp-stream";


// // bool signupok     =     false;

// // // Define the pins for SoftwareSerial communication
// // const int rxPin = D3;  // RX pin of ESP8266 connected to TX pin of Arduino Nano
// // const int txPin = D2;  // TX pin of ESP8266 connected to RX pin of Arduino Nano

// // SoftwareSerial arduinoSerial(rxPin, txPin); // Create a SoftwareSerial object


// // void streamTimeoutCallback(bool timeout) {
// //   if (timeout)
// //     Serial.println("stream timed out, resuming...\n");

// //   if (!stream.httpConnected())
// //     Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
// // }

// // void streamCallback(FirebaseStream data) {
// //   // Serial.println("\nStream Data available...");
// //   // Serial.println("STREAM PATH: " + data.streamPath());
// //   // Serial.println("EVENT PATH: " + data.dataPath());
// //   // Serial.println("DATA TYPE: " + data.dataTfype());
// //   // Serial.println("EVENT TYPE: " + data.eventType());

// //   if (data.dataType() == "json") {
// //     DynamicJsonDocument doc(1024);
// //     DeserializationError error = deserializeJson(doc, data.jsonString());    
// //     if (error) {
// //       Serial.print(F("deserializeJson() failed: "));
// //       Serial.println(error.f_str());
// //       return;
// //     }
    
// //     //serializeJsonPretty(doc, Serial);    
// //     String command = doc["cmds"]["cmd"].as<String>();
// //     if (command.equals("writeOut")) {      
// //       float x = doc["cmds"]["x"];
// //       float y = doc["cmds"]["y"];
// //       Serial.printf("\n%s %d %d\n", command.c_str(), x, y);            
// //       // digitalWrite(pin, level);
// //     }
// //   }
// // }

// // void setup() {
// //   // Initialize serial communication
// //   Serial.begin(115200);
// //   arduinoSerial.begin(115200); // Start SoftwareSerial communication with Arduino Nano

// //   // Firebase Start
// //   WiFi.mode(WIFI_STA);
// //   WiFi.begin(ssid,pass);

// //   Serial.begin(115200);

// //   // while(WiFi.status() != WL_CONNECTED)
// //   // {
// //   //   delay(500);
// //   //   Serial.print(".");
// //   // }
// //   // Serial.println("");
// //   // Serial.print("Connected to: ");
// //   // Serial.print(ssid); 
// //   // Serial.println("");
// //   // Serial.print("IP Address: ");
// //   // Serial.println(WiFi.localIP());

// //   // config.api_key = API_KEY;
// //   // config.database_url= DATABASE_URL;
  
// //   // if(Firebase.signUp(&config,&auth,"",""))
// //   // {
// //   //   Serial.println("OK");
// //   //   signupok=true;
// //   // }
// //   // else
// //   // {
// //   //   Serial.println("Error");
// //   // }

// //   //config.token_status_callback = tokenStatusCallback;

// //   // Firebase.begin(&config,&auth);
// //   // Firebase.reconnectWiFi(true);
// //   // #if defined(ESP8266)
// //   //   //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
// //   //   stream.setBSSLBufferSize(2048, 2048);
// //   //   stream.setResponseSize(1024);
// //   // #endif

// //   // if (!Firebase.RTDB.beginStream(&stream, path)) {
// //   //   Serial.println("\nCan't begin stream connection...");
// //   //   Serial.println("REASON: " + stream.errorReason());
// //   //   Serial.println("------------------------------------\n");
// //   // }

// //   // // Set callback functions for stream data
// //   // Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

// //   // Firebase End


// // }

// // void loop() {
// //   if (arduinoSerial.available()) {
// //     // Read data from Arduino Nano
// //     String data = arduinoSerial.readStringUntil('\n');
    
// //     // Parse received data
// //     String values[10]; // Assuming there are 10 values separated by tabs
// //     int i = 0;
// //     int pos = 0;
// //     while ((pos = data.indexOf(',')) != -1) {
// //       values[i++] = data.substring(0, pos);
// //       data = data.substring(pos + 1);
// //     }
// //     values[i] = data; // Store the last value
    
// //     // Convert and print parsed data
// //     float euler[3];
// //     float flex[5];
// //     float contact[2];
// //     for (int j = 0; j < 3; j++) {
// //       euler[j] = values[j].toFloat();
// //     }
// //     for (int j = 3; j < 8; j++) {
// //       flex[j] = values[j].toFloat();
// //     }
// //     for (int j = 8; j < 10; j++) {
// //       contact[j] = values[j].toFloat();
// //     }
// //     Serial.print("Euler:\t");
// //     Serial.print(euler[0]);
// //     Serial.print("\t");
// //     Serial.print(euler[1]);
// //     Serial.print("\t");
// //     Serial.println(euler[2]);

// //     Serial.print("Flex:\t");
// //     Serial.print(flex[0]);
// //     Serial.print("\t");
// //     Serial.print(flex[1]);
// //     Serial.print("\t");
// //     Serial.print(flex[2]);
// //     Serial.print("\t");
// //     Serial.print(flex[3]);
// //     Serial.print("\t");
// //     Serial.println(flex[4]);

// //     Serial.print("Contact:\t");
// //     Serial.print(contact[0]);
// //     Serial.print("\t");
// //     Serial.println(contact[1]);

// //     // Add data to firebase
// //   //   String jsonStr;

// //   //   DynamicJsonDocument doc(1024);
// //   //   JsonArray gyros = doc.createNestedArray("gyros");
// //   //   JsonObject gyro = gyros.createNestedObject();
// //   //   gyro["x"] = euler[0];
// //   //   gyro["y"] = euler[1];
// //   //   gyro["z"] = euler[2];

// //   //   gyro["flex1"] = flex[0];
// //   //   gyro["flex2"] = flex[1];
// //   //   gyro["flex3"] = flex[2];
// //   //   gyro["flex4"] = flex[3];
// //   //   gyro["flex5"] = flex[4];

// //   //   gyro["contact1"] = contact[0];
// //   //   gyro["contact2"] = contact[1];
    
// //   //   serializeJson(doc, jsonStr); 
// //   //   if (Firebase.ready()) {    
// //   //   FirebaseJson fb_json;
// //   //   fb_json.setJsonData(jsonStr);
// //   //   if (!Firebase.RTDB.setJSONAsync(&fbdo, path, &fb_json)) {
// //   //     Serial.println("FAILED");
// //   //     Serial.println("REASON: " + fbdo.errorReason());
// //   //     Serial.println();
// //   //   }
// //   // }
// //     // if(Firebase.ready() )
// //     // {
// //     //   Firebase.RTDB.setFloatAsync(&fbdo, "/gyro/x", euler[0]);
// //     //   Firebase.RTDB.setFloatAsync(&fbdo, "/gyro/y", euler[1]);
// //     //   Firebase.RTDB.setFloatAsync(&fbdo, "/gyro/z", euler[2]);
// //     // }
// //   }
// // }

// #include <SoftwareSerial.h>
// // #include <ESP8266WiFi.h>
// // #include <Firebase_ESP_Client.h>
// // #include <ArduinoJson.h>
// // #include "addons/RTDBHelper.h"
// // #include "addons/TokenHelper.h"

// // #define API_KEY "AIzaSyBK9R_Mf22n28Y0_vYdRhPom958CCxHtyM"
// // #define DATABASE_URL "https://sign-language-glove-zc-default-rtdb.europe-west1.firebasedatabase.app/"

// // #define   ssid          "Youssef"
// // #define   pass          "12345567890"

// // FirebaseData stream;
// // FirebaseData fbdo;
// // FirebaseAuth auth;
// // FirebaseConfig config;
// // const char* path = "/esp-stream";


// // // String Data       =     "0";
// // bool signupok     =     false;

// // Define the pins for SoftwareSerial communication
// const int rxPin = D3;  // RX pin of ESP8266 connected to TX pin of Arduino Nano
// const int txPin = D2;  // TX pin of ESP8266 connected to RX pin of Arduino Nano

// SoftwareSerial arduinoSerial(rxPin, txPin); // Create a SoftwareSerial object


// // void streamTimeoutCallback(bool timeout) {
// //   if (timeout)
// //     Serial.println("stream timed out, resuming...\n");

// //   if (!stream.httpConnected())
// //     Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
// // }

// // void streamCallback(FirebaseStream data) {
// //   Serial.println("\nStream Data available...");
// //   Serial.println("STREAM PATH: " + data.streamPath());
// //   Serial.println("EVENT PATH: " + data.dataPath());
// //   Serial.println("DATA TYPE: " + data.dataType());
// //   Serial.println("EVENT TYPE: " + data.eventType());

// //   if (data.dataType() == "json") {
// //     DynamicJsonDocument doc(1024);
// //     DeserializationError error = deserializeJson(doc, data.jsonString());    
// //     if (error) {
// //       Serial.print(F("deserializeJson() failed: "));
// //       Serial.println(error.f_str());
// //       return;
// //     }
    
// //     //serializeJsonPretty(doc, Serial);    
// //     String command = doc["cmds"]["cmd"].as<String>();
// //     if (command.equals("writeOut")) {      
// //       float x = doc["cmds"]["x"];
// //       float y = doc["cmds"]["y"];
// //       Serial.printf("\n%s %d %d\n", command.c_str(), x, y);            
// //       // digitalWrite(pin, level);
// //     }
// //   }
// // }

// void setup() {
//   // Initialize serial communication
//   Serial.begin(115200);
//   arduinoSerial.begin(115200); // Start SoftwareSerial communication with Arduino Nano

//   // Firebase Start
//   // WiFi.mode(WIFI_STA);
//   // WiFi.begin(ssid,pass);

//   // Serial.begin(115200);

//   // while(WiFi.status() != WL_CONNECTED)
//   // {
//   //   delay(500);
//   //   Serial.print(".");
//   // }
//   // Serial.println("");
//   // Serial.print("Connected to:");
//   // Serial.print(ssid); 
//   // Serial.println("");
//   // Serial.print("IP Address: ");
//   // Serial.print(WiFi.localIP());

//   // config.api_key = API_KEY;
//   // config.database_url= DATABASE_URL;
  
//   // if(Firebase.signUp(&config,&auth,"",""))
//   // {
//   //   Serial.println("OK");
//   //   signupok=true;
//   // }
//   // else
//   // {
//   //   Serial.println("Error");
//   // }

//   // //config.token_status_callback = tokenStatusCallback;

//   // Firebase.begin(&config,&auth);
//   // Firebase.reconnectWiFi(true);
//   // #if defined(ESP8266)
//   //   //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
//   //   stream.setBSSLBufferSize(2048, 2048);
//   //   stream.setResponseSize(1024);
//   // #endif

//   // if (!Firebase.RTDB.beginStream(&stream, path)) {
//   //   Serial.println("\nCan't begin stream connection...");
//   //   Serial.println("REASON: " + stream.errorReason());
//   //   Serial.println("------------------------------------\n");
//   // }

//   // // Set callback functions for stream data
//   // Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

//   // Firebase End


// }

// void loop() {
//   if (arduinoSerial.available()) {
//     // Read data from Arduino Nano
//     String data = arduinoSerial.readStringUntil('\n');
    
//     // Parse received data
//     String values[10]; // Assuming there are three values separated by tabs
//     int i = 0;
//     int pos = 0;
//     while ((pos = data.indexOf(',')) != -1) {
//       values[i++] = data.substring(0, pos);
//       data = data.substring(pos + 1);
//     }
//     values[i] = data; // Store the last value
    
//     // Convert and print parsed data
//     float euler[3];
//     float flex[5];
//     float contact[2];
//     for (int j = 0; j < 3; j++) {
//       euler[j] = values[j].toFloat();
//     }
//     for (int j = 3; j < 8; j++) {
//       flex[j] = values[j].toFloat();
//     }
//     for (int j = 8; j < 10; j++) {
//       contact[j] = values[j].toFloat();
//     }
//     Serial.print("Euler:\t");
//     Serial.print(euler[0]);
//     Serial.print("\t");
//     Serial.print(euler[1]);
//     Serial.print("\t");
//     Serial.println(euler[2]);

//     Serial.print("Flex:\t");
//     Serial.print(flex[0]);
//     Serial.print("\t");
//     Serial.print(flex[1]);
//     Serial.print("\t");
//     Serial.print(flex[2]);
//     Serial.print("\t");
//     Serial.print(flex[3]);
//     Serial.print("\t");
//     Serial.println(flex[4]);

//     Serial.print("Contact:\t");
//     Serial.print(contact[0]);
//     Serial.print("\t");
//     Serial.println(contact[1]);

//     // Add data to firebase
//   //   String jsonStr;

//   //   DynamicJsonDocument doc(1024);
//   //   JsonArray gyros = doc.createNestedArray("gyros");
//   //   JsonObject gyro = gyros.createNestedObject();
//   //   gyro["x"] = euler[0];
//   //   gyro["y"] = euler[1];
//   //   gyro["z"] = euler[2];
//   //   serializeJson(doc, jsonStr); 
//   //   if (Firebase.ready()) {    
//   //   FirebaseJson fb_json;
//   //   fb_json.setJsonData(jsonStr);
//   //   if (!Firebase.RTDB.setJSONAsync(&fbdo, path, &fb_json)) {
//   //     Serial.println("FAILED");
//   //     Serial.println("REASON: " + fbdo.errorReason());
//   //     Serial.println();
//   //   }
//   // }
//     // if(Firebase.ready() )
//     // {
//     //   Firebase.RTDB.setFloat(&fbdo, "/gyro/x", euler[0]);
//     //   Firebase.RTDB.setFloat(&fbdo, "/gyro/y", euler[1]);
//     //   Firebase.RTDB.setFloat(&fbdo, "/gyro/z", euler[2]);
//     // }
//   }
// }
#include <SoftwareSerial.h>

const int rxPin = D3;  // RX pin of ESP8266 connected to TX pin of Arduino Nano
const int txPin = D2;  // TX pin of ESP8266 connected to RX pin of Arduino Nano

SoftwareSerial arduinoSerial(rxPin, txPin);

void setup() {
  Serial.begin(115200);
  arduinoSerial.begin(115200);

  // Initialize Firebase or other libraries here
}

void loop() {
  if (arduinoSerial.available() > 0) {
    String data = arduinoSerial.readStringUntil('l');

    // Parse received data
    float values[10]; // Assuming there are ten float values separated by commas
    int i = 0;
    char* value = strtok(const_cast<char*>(data.c_str()), ",");
    while (value != NULL && i < 10) {
      values[i++] = atof(value);
      value = strtok(NULL, ",");
    }

    // Print parsed data
    // Flex
    for (int j = 3; j < 8; j++) {
      Serial.print(values[j]);
      Serial.print("\t");
    }
      Serial.print("\t");
    //Contact
    for (int j = 8; j < 10; j++) {
      Serial.print(values[j]);
      Serial.print("\t");
    }
      Serial.print("\t");
    // XYZ
    for (int j = 0; j < 3; j++) {
      Serial.print(values[j]);
      Serial.print("\t");
    }
    Serial.println();
    delay(10);
    // Add data to Firebase or perform other actions here
  }
}
