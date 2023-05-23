/*
Author: John Harrington 

This program is an Arduino sketch that continuously reads data from a 9-axis IMU 
and serves this data to a Python client program via a Bluetooth connection. 
*/

#include "ICM_20948.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define SERIAL_PORT Serial
#define WIRE_PORT Wire
#define AD0_VAL 1
ICM_20948_I2C myICM;  // Otherwise create an ICM_20948_I2C object

const int BLT_BR = 9600;
const int BLT_RX = 10;
const int BLT_TX = 9;
SoftwareSerial bluetooth(BLT_RX, BLT_TX);

void setup() {

  SERIAL_PORT.begin(115200);
  bluetooth.begin(BLT_BR);

  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);

  //myICM.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  bool initialized = false;
  while (!initialized) {
    myICM.begin(WIRE_PORT, AD0_VAL);

    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok) {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    } else {
      initialized = true;
    }
  }
}

void loop() {

  if (myICM.dataReady()) {
    myICM.getAGMT();          // The values are only updated when you call 'getAGMT'
    printScaledAGMT(&myICM);  // This function takes into account the scale settings from when the measurement was made to calculate the values with units
    delay(30);
  } else {
    SERIAL_PORT.println("Waiting for data");
    delay(500);
  }
}

void printScaledAGMT(ICM_20948_I2C *sensor) {
  StaticJsonDocument<82> doc;

  doc["a_x"] = sensor->accX();
  doc["a_y"] = sensor->accY();
  doc["a_z"] = sensor->accZ();
  doc["g_x"] = sensor->gyrX();
  doc["g_y"] = sensor->gyrY();

  serializeJson(doc, bluetooth);
  serializeJson(doc, SERIAL_PORT);
  SERIAL_PORT.println();
}
