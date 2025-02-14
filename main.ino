#include <Wire.h>          // For I2C communication
#include <MPU6050.h>
#include "MIDIUSB.h"

MPU6050 mpu1(0x68); 
MPU6050 mpu2(0x69); 

unsigned long prevTime1 = 0;
unsigned long prevTime2 = 0;

const int snareNote = 38;     
const int bassNote = 36;      
const int hi_hat = 42;     
const int crashNote = 49;     
const int velocity = 100;     
const float threshold = 1.50;
const float cymbalThreshold = 1.75;
bool notePlaying1 = false;
bool notePlaying2 = false;

float filteredX1 = 0.0, filteredY1 = 0.0, filteredZ1 = 0.0;
float filteredGyroX1 = 0.0, filteredGyroY1 = 0.0, filteredGyroZ1 = 0.0;
float filteredX2 = 0.0, filteredY2 = 0.0, filteredZ2 = 0.0;
float filteredGyroX2 = 0.0, filteredGyroY2 = 0.0, filteredGyroZ2 = 0.0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Initializing MPU6050 sensors...");
  Wire.begin();

  mpu1.initialize();
  mpu2.initialize();

  if (!mpu1.testConnection()) {
    Serial.println("MPU6050-1 connection failed!");
    while (1)
      ;
  } else {
    Serial.println("MPU6050-1 connected successfully.");
  }

  if (!mpu2.testConnection()) {
    Serial.println("MPU6050-2 connection failed!");
    while (1)
      ;
  } else {
    Serial.println("MPU6050-2 connected successfully.");
  }
}

void loop() {
  int16_t ax1, ay1, az1;
  int16_t gx1, gy1, gz1;

  int16_t ax2, ay2, az2;
  int16_t gx2, gy2, gz2;

  mpu1.getAcceleration(&ax1, &ay1, &az1);
  mpu1.getRotation(&gx1, &gy1, &gz1);

  mpu2.getAcceleration(&ax2, &ay2, &az2);
  mpu2.getRotation(&gx2, &gy2, &gz2);

  float accelX1 = ax1 / 16384.0;
  float accelY1 = ay1 / 16384.0;
  float accelZ1 = az1 / 16384.0;

  float accelX2 = ax2 / 16384.0;
  float accelY2 = ay2 / 16384.0;
  float accelZ2 = az2 / 16384.0;

  float gyroX1 = gx1 / 131.0;
  float gyroY1 = gy1 / 131.0;
  float gyroZ1 = gz1 / 131.0;

  float gyroX2 = gx2 / 131.0;
  float gyroY2 = gy2 / 131.0;
  float gyroZ2 = gz2 / 131.0;

  filteredGyroX1 = abs(0.3 * gyroX1 + 0.7 * filteredGyroX1);
  filteredGyroY1 = abs(0.3 * gyroY1 + 0.7 * filteredGyroY1);
  filteredGyroZ1 = abs(0.3 * gyroZ1 + 0.7 * filteredGyroZ1);

  filteredX1 = abs(0.3 * accelX1 + 0.7 * filteredX1);
  filteredY1 = 0.3 * accelY1 + 0.7 * filteredY1;
  filteredZ1 = abs(0.3 * accelZ1 + 0.7 * filteredZ1);

  filteredGyroX2 = abs(0.3 * gyroX2 + 0.7 * filteredGyroX2);
  filteredGyroY2 = abs(0.3 * gyroY2 + 0.7 * filteredGyroY2);
  filteredGyroZ2 = abs(0.3 * gyroZ2 + 0.7 * filteredGyroZ2);

  filteredX2 = abs(0.3 * accelX2 + 0.7 * filteredX2);
  filteredY2 = 0.3 * accelY2 + 0.7 * filteredY2;
  filteredZ2 = abs(0.3 * accelZ2 + 0.7 * filteredZ2);

  float accelMagnitude1 = abs(filteredY1);

  float accelMagnitude2 = abs(filteredY2);

  unsigned long currentTime = millis();


  if (filteredX1 > 0.3 && abs(filteredGyroY1) > 180.0 && accelMagnitude1 > cymbalThreshold && !notePlaying1 && currentTime - prevTime1 > 175) {
    prevTime1 = currentTime;
    notePlaying1 = true;
    sendNoteOn(hi_hat, velocity); 
  } else if (accelMagnitude1 > threshold && abs(filteredGyroY1) < 100.0 && !notePlaying1 && currentTime - prevTime1 > 175) {
    prevTime1 = currentTime;
    notePlaying1 = true;
    sendNoteOn(snareNote, velocity);
  }


  if (filteredX2 > 0.3 && abs(filteredGyroY2) > 180.0 && accelMagnitude2 > cymbalThreshold && !notePlaying2 && currentTime - prevTime2 > 175) {
    prevTime2 = currentTime;
    notePlaying2 = true;
    sendNoteOn(crashNote, velocity);
  } else if (accelMagnitude2 > threshold && abs(filteredGyroY2) < 100.0 && !notePlaying2 && currentTime - prevTime2 > 175) {
    prevTime2 = currentTime;
    notePlaying2 = true;
    sendNoteOn(bassNote, velocity);
  }

  if (accelMagnitude1 < threshold && notePlaying1) {
    notePlaying1 = false;
    sendNoteOff(snareNote, 0);
    sendNoteOff(hi_hat, 0);
  }

  if (accelMagnitude2 < threshold && notePlaying2) {
    notePlaying2 = false;
    sendNoteOff(bassNote, 0);
    sendNoteOff(crashNote, 0);
  }
}

void sendNoteOn(byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void sendNoteOff(byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}
