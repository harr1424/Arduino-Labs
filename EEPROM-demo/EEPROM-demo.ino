/*
Embedded Systems Programming Assignment 3 
Author: John Harrington

Much of this code was provided in lecture slides. 

It has been modified to meet the requirements of programming assignment 3. 

Notably, it has been modified to incorporate a button. The first time this button is pressed, 
the print logic (which was mostly provided by the skeleton code) will execute in order to 
print to Serial console the existing EEPROM entries describing temperature measurements. 

Additionally, a second button press will erase the entire EEPROM address space, wait 5 seconds, and 
resume the program writing the first sample collected to EEPROM address 0. 

It is generally very bad practice to place EEPROM reads / writes inside loops. This is described in 
lecture 06_Memory slide 41. EEPROM has a limited lifespan of ~10,000 write / erase cycles. 

Considering this, I attempted to find a more efficient way to erase the EEPROM on the second button 
press, but realized this wasn't possible as if the scheduled write operations had cycled through the 
entirety of EEPROM address space, and then a new cycle began, it would be necessary to erase the entirety 
of EEPROM address space in order to avoid leaving left-over values stored there. 
*/


#include <math.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define SAMPLE_TIME 500

const int B = 4275;
const int R0 = 100000;
const int pinTempSensor = A0;

int address = 0;   //EEPROM address counter
int nSamples = 0;  //Sample counter

unsigned long write_timer;

const int buttonPin = 2;
volatile int button_val = 0;

void printTemp2Monitor();
void writeTemp2EEPROM();
void pin_ISR();
void clear_EEPROM();

void setup() {
  Serial.begin(9600);
  write_timer = millis();

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, RISING);
}

void loop() {
  if (millis() - write_timer > SAMPLE_TIME) {
    writeTemp2EEPROM();
    write_timer = millis();
  }

  if (button_val == 1) {
    printTemp2Monitor();
    button_val = 2;
  }

  if (button_val == 3) {
    clear_EEPROM();
    button_val = 0;
  }

  delay(1);
}

void printTemp2Monitor() {
  Serial.print("Number of stored samples = ");
  Serial.println(nSamples);

  for (int i = 0; i < nSamples; i++) {  // NOTE EEPROM reads / writes should generally not be placed in loops
    float temperature = 0.00f;
    EEPROM.get(i * sizeof(float), temperature);
    Serial.print("At address ");
    Serial.print(i * sizeof(float));
    Serial.print(", temperature = ");
    Serial.println(temperature);
  }
  address = 0;
  nSamples = 0;
}

void writeTemp2EEPROM() {
  int a = analogRead(pinTempSensor);

  float R = R0 * (1023.0 / a - 1.0);
  float temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15;
  EEPROM.put(address, temperature);

  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.print(" stored at address ");
  Serial.println(address);

  nSamples++;
  address += sizeof(float);

  if (address == EEPROM.length()) {
    address = 0;
  }
}

void pin_ISR() {
  noInterrupts();
  button_val++;
  interrupts();
}

void clear_EEPROM() {
  Serial.println("Erasing EEPROM, please wait...");
  for (int i = 0; i < EEPROM.length(); i++) {  // NOTE EEPROM reads / writes should generally not be placed in loops
    EEPROM.write(i, 0);
  }
  Serial.println("EEPROM has been erased. Resuming program in five seconds.");

  address = 0;
  nSamples = 0;

  delay(5000);
}