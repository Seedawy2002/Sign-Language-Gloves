#include <SPI.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// ======================================
//            Sensor readinngs
// ======================================
const int FLEX1_PIN = A1;
const int FLEX2_PIN = A2;
const int FLEX3_PIN = A3;
const int FLEX4_PIN = A6;
const int FLEX5_PIN = A7;


const int CONTACT1_PIN = 3;
const int CONTACT2_PIN = 4;


int flex1_reading;
int flex2_reading;
int flex3_reading;
int flex4_reading;
int flex5_reading;


float flex1;
float flex2;
float flex3;
float flex4;
float flex5;


int contact1;
int contact2;


float gyro_x;
float gyro_y;
float gyro_z;

const setup_pins() {
    pinMode(FLEX1_PIN, INPUT);
    pinMode(FLEX2_PIN, INPUT);
    pinMode(FLEX3_PIN, INPUT);
    pinMode(FLEX4_PIN, INPUT);
    pinMode(FLEX5_PIN, INPUT);

    pinMode(CONTACT1_PIN, INPUT);
    pinMode(CONTACT2_PIN, INPUT);
}

const read_sensors() {
  flex1_reading = analogRead(FLEX1_PIN);
  flex2_reading = analogRead(FLEX2_PIN);
  flex3_reading = analogRead(FLEX3_PIN);
  flex4_reading = analogRead(FLEX4_PIN);
  flex5_reading = analogRead(FLEX5_PIN);

  flex1 = (flex1_reading * 5.0) / 1023.0;
  flex2 = (flex2_reading * 5.0) / 1023.0;
  flex3 = (flex3_reading * 5.0) / 1023.0;
  flex4 = (flex4_reading * 5.0) / 1023.0;
  flex5 = (flex5_reading * 5.0) / 1023.0;

  contact1 = digitalRead(CONTACT1_PIN);
  contact2 = digitalRead(CONTACT2_PIN);
}
// ======================================
//                 MPU
// ======================================
MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container

void setup_mpu(){
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif


    // initialize device
    mpu.initialize();
    devStatus = mpu.dmpInitialize();


    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);


    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        dmpReady = true;
        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}


// ======================================
//                 SPI
// ======================================
class ESPSafeMaster {
private:
  uint8_t _ss_pin;
  void _pulseSS() {
    digitalWrite(_ss_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_ss_pin, LOW);
  }

public:
  ESPSafeMaster(uint8_t pin)
    : _ss_pin(pin) {}
  void begin() {
    pinMode(_ss_pin, OUTPUT);
    _pulseSS();
  }


  uint32_t readStatus() {
    _pulseSS();
    SPI.transfer(0x04);
    uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
    _pulseSS();
    return status;
  }


  void writeStatus(uint32_t status) {
    _pulseSS();
    SPI.transfer(0x01);
    SPI.transfer(status & 0xFF);
    SPI.transfer((status >> 8) & 0xFF);
    SPI.transfer((status >> 16) & 0xFF);
    SPI.transfer((status >> 24) & 0xFF);
    _pulseSS();
  }


  void readData(uint8_t *data) {
    _pulseSS();
    SPI.transfer(0x03);
    SPI.transfer(0x00);
    for (uint8_t i = 0; i < 32; i++) { data[i] = SPI.transfer(0); }
    _pulseSS();
  }


  void writeData(uint8_t *data, size_t len) {
    uint8_t i = 0;
    _pulseSS();
    SPI.transfer(0x02);
    SPI.transfer(0x00);
    while (len-- && i < 32) { SPI.transfer(data[i++]); }
    while (i++ < 32) { SPI.transfer(0); }
    _pulseSS();
  }


  String readData() {
    char data[33];
    data[32] = 0;
    readData((uint8_t *)data);
    return String(data);
  }


  void writeData(const char *data) {
    writeData((uint8_t *)data, strlen(data));
  }
};


ESPSafeMaster esp(SS);


void send_to_esp(String data) {
  const int chunkSize = 32;
    for (int i = 0; i < data.length(); i += chunkSize) {
        String chunk = data.substring(i, min(i + chunkSize, data.length()));
        // Check if this is the last chunk
        if (i + chunkSize >= data.length()) {
            chunk += 'x'; // Append 'x' to indicate new line
        }
        const char *message = chunk.c_str();
        esp.writeData(message);
        Serial.println(message);
    }
    Serial.println(data);
    delay(10); // Essential delay
  // const char *message = data.c_str();
  // esp.writeData(message);
  // Serial.println(message);
  // delay(10); // Essential delay
}


// ======================================
//                 Setup
// ======================================
void setup() {
  Serial.begin(115200);
  SPI.begin();
  esp.begin();
  setup_mpu();
  setup_pins();
}




String concat_vars(float gyro_x, float gyro_y, float gyro_z,
                    float flex1, float flex2, float flex3, float flex4, float flex5,
                    int contact1, int contact2) {
  return String(gyro_x) + "," + String(gyro_y) + "," + String(gyro_z) + "," +
         String(flex1) + "," + String(flex2) + "," + String(flex3) + "," +
         String(flex4) + "," + String(flex5) + "," + String(contact1) + "," +
         String(contact2);
}




// ======================================
//                 Loop
// ======================================


void loop() {
    if (!dmpReady) return;
    // read a packet from FIFO
    read_sensors();
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
            gyro_x = euler[0] * 180/M_PI;
            gyro_y = euler[1] * 180/M_PI;
            gyro_z = euler[2] * 180/M_PI;
            String data = concat_vars(gyro_x, gyro_y, gyro_z, flex1, flex2, flex3, flex4, flex5, contact1, contact2);
            send_to_esp(data);
    }
   
}


