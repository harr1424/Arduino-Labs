/*
Author: John Harrington

This program was created for a course in Embedded Systems, Lab 2.

This program uses the Arduino to control the brightness of an LED light dependent upon the 
degrees a sensor is turned.
*/


#define SENSOR A0
#define ADC_REF 5
#define VCC 5
#define FULL_ANGLE 300


void setup() {
  Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  pinMode(10, OUTPUT);
}

void loop() {

  float voltage;
  int sensor_value = analogRead(SENSOR);
  voltage = (float)sensor_value * ADC_REF / 1023;
  float degrees = (voltage * FULL_ANGLE) / VCC;
  Serial.println(degrees);

  //angle of 300 corresponds to writing 255
  if (degrees >= 255) {
    analogWrite(10, 255);
  }
  else {
    analogWrite(10, degrees);
  }
}
