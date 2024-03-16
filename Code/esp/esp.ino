#include "SPISlave.h"
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"

#define API_KEY "AIzaSyBK9R_Mf22n28Y0_vYdRhPom958CCxHtyM"
#define DATABASE_URL "https://sign-language-glove-zc-default-rtdb.europe-west1.firebasedatabase.app/"

#define   ssid          "Islam - Main"
#define   pass          "yassin5464shab"

FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
const char* path = "/esp-stream";

bool signupok     =     false;


void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void streamCallback(FirebaseStream data) {
  // Serial.println("\nStream Data available...");
  // Serial.println("STREAM PATH: " + data.streamPath());
  // Serial.println("EVENT PATH: " + data.dataPath());
  // Serial.println("DATA TYPE: " + data.dataTfype());
  // Serial.println("EVENT TYPE: " + data.eventType());

  if (data.dataType() == "json") {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, data.jsonString());    
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    
    //serializeJsonPretty(doc, Serial);    
    String command = doc["cmds"]["cmd"].as<String>();
    if (command.equals("writeOut")) {      
      float x = doc["cmds"]["x"];
      float y = doc["cmds"]["y"];
      Serial.printf("\n%s %d %d\n", command.c_str(), x, y);            
      // digitalWrite(pin, level);
    }
  }
}


struct sensors_data {
  float gyro_x;
  float gyro_y;
  float gyro_z;

  float flex1;
  float flex2;
  float flex3;
  float flex4;
  float flex5;

  int contact1;
  int contact2;
  int contact3;
};

String receivedData;
struct sensors_data output;

void parseInput(String input_str){

    if (input_str.indexOf('x') != -1) {
      // Append packet to receivedData
        receivedData += input_str.substring(0, input_str.indexOf('x'));
       
        // Process receivedData
        const int NUM_OF_VARS = 11;
        float values[NUM_OF_VARS]; // Assuming there are ten float values separated by commas
        int i = 0;
        char* value = strtok(const_cast<char*>(receivedData.c_str()), ",");
        while (value != NULL && i < NUM_OF_VARS) {
          values[i++] = atof(value);
          value = strtok(NULL, ",");
        }

        // Print parsed data
          output.gyro_x = values[0];
          output.gyro_y = values[1];
          output.gyro_z = values[2];

          output.flex1 = values[3];
          output.flex2 = values[4];
          output.flex3 = values[5];
          output.flex4 = values[6];
          output.flex5 = values[7];

          output.contact1 = ceil(values[8]);
          output.contact2 = ceil(values[9]);
          output.contact3 = ceil(values[10]);

          // Serial Output
          Serial.print(String(output.gyro_x));
          Serial.print('\t');
          Serial.print(String(output.gyro_y));
          Serial.print('\t');
          Serial.print(String(output.gyro_z));
          Serial.print('\t');
          Serial.print(String(output.flex1));
          Serial.print('\t');
          Serial.print(String(output.flex2));
          Serial.print('\t');
          Serial.print(String(output.flex3));
          Serial.print('\t');
          Serial.print(String(output.flex4));
          Serial.print('\t');
          Serial.print(String(output.flex5));
          Serial.print('\t');
          Serial.print(String(output.contact1));
          Serial.print('\t');
          Serial.print(String(output.contact2));
          Serial.print('\t');
          Serial.println(String(output.contact3));

          // Reset receivedData for next transmission
          receivedData = "";
      } else {
        // Append packet to receivedData
        receivedData += input_str;
    }
}


void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,pass);

    while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to: ");
    Serial.print(ssid); 
    Serial.println("");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  config.api_key = API_KEY;
  config.database_url= DATABASE_URL;
  
  if(Firebase.signUp(&config,&auth,"",""))
  {
    Serial.println("OK");
    signupok=true;
  }
  else
  {
    Serial.println("Error");
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
  #if defined(ESP8266)
    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    stream.setBSSLBufferSize(2048, 2048);
    stream.setResponseSize(1024);
  #endif

  if (!Firebase.RTDB.beginStream(&stream, path)) {
    Serial.println("\nCan't begin stream connection...");
    Serial.println("REASON: " + stream.errorReason());
    Serial.println("------------------------------------\n");
  }

  // Set callback functions for stream data
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

  // data has been received from the master. Beware that len is always 32
  // and the buffer is autofilled with zeroes if data is less than 32 bytes long
  // It's up to the user to implement protocol for handling data length
  SPISlave.onData([](uint8_t *data, size_t len) {
    String message = String((char *)data);
    (void)len;
    // if (message.equals("Hello Slave!")) {
    //   SPISlave.setData("Hello Master!");
    // } else if (message.equals("Are you alive?")) {
    //   char answer[33];
    //   sprintf(answer, "Alive for %lu seconds!", millis() / 1000);
    //   SPISlave.setData(answer);
    // } else {
    //   SPISlave.setData("Say what?");
    // }
    // Serial.println((char *)data);
    parseInput(message);

  });


  // status has been received from the master.
  // The status register is a special register that bot the slave and the master can write to and read from.
  // Can be used to exchange small data or status information
  SPISlave.onStatus([](uint32_t data) {
    SPISlave.setStatus(millis());  // set next status
  });

  // Setup SPI Slave registers and pins
  SPISlave.begin();

  // Set the status register (if the master reads it, it will read this value)
  SPISlave.setStatus(millis());

}

void loop() {
    String jsonStr;
    DynamicJsonDocument doc(1024);
    JsonArray gyros = doc.createNestedArray("gyros");
    JsonObject gyro = gyros.createNestedObject();
    
    gyro["x"] = output.gyro_x;
    gyro["y"] = output.gyro_y;
    gyro["z"] = output.gyro_z;

    gyro["flex1"] = output.flex1;
    gyro["flex2"] = output.flex2;
    gyro["flex3"] = output.flex3;
    gyro["flex4"] = output.flex4;
    gyro["flex5"] = output.flex5;

    gyro["contact1"] = output.contact1;
    gyro["contact2"] = output.contact2;
    gyro["contact3"] = output.contact3;

    serializeJson(doc, jsonStr); 

    if (Firebase.ready()) {    
    FirebaseJson fb_json;
    fb_json.setJsonData(jsonStr);
    if (!Firebase.RTDB.setJSONAsync(&fbdo, path, &fb_json)) {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      Serial.println();
    }
  }
}
