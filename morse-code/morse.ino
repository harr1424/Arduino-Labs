/*
Author: John Harrington

Embedded Systems Lab One

This program uses the Arduino to display serial output instructing the 
program user to input a capital english alphabet character. 

Once the character is input to the serial console and enter pressed, 
the Arduino will flash the built-in LED light to demonstrate the 
Morse encoding of the given alphabet character

Afterwards, the serial buffer is flushed, and the loop executes again, 
asking the program user for a new alphabet character.
*/

void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(400);
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
}

void error() {
  for (int i = 0; i < 10; i++) {

    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void setup() {
  // set up built in LED
  pinMode(LED_BUILTIN, OUTPUT);

  // setup serial reader and print message to user
  Serial.begin(9600);
  delay(2000);
  Serial.println("Enter the alphabet character (capitalized) to display using LED in Morse Code:");
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    Serial.print("Now demonstrating: ");
    Serial.println(input);
    delay(2000);

    switch (input) {
      case 'A':
        dot();
        dash();
        break;

      case 'B':
        dash();
        dot();
        dot();
        dot();
        break;

      case 'C':
        dash();
        dot();
        dash();
        dot();
        break;

      case 'D':
        dash();
        dot();
        dot();
        break;

      case 'E':
        dot();
        break;

      case 'F':
        dot();
        dot();
        dash();
        dot();
        break;

      case 'G':
        dash();
        dash();
        break;

      case 'H':
        dot();
        dot();
        dot();
        dot();
        break;

      case 'I':
        dot();
        dot();
        break;

      case 'J':
        dot();
        dash();
        dash();
        dash();
        break;

      case 'K':
        dash();
        dot();
        dash();
        break;

      case 'L':
        dot();
        dash();
        dot();
        dot();
        break;

      case 'M':
        dash();
        dash();
        break;

      case 'N':
        dash();
        dot();
        break;

      case 'O':
        dash();
        dash();
        dash();
        break;

      case 'P':
        dot();
        dash();
        dash();
        dot();
        break;

      case 'Q':
        dash();
        dash();
        dot();
        dash();
        break;

      case 'R':
        dot();
        dash();
        dot();
        break;

      case 'S':
        dot();
        dot();
        dot();
        break;

      case 'T':
        dash();
        break;

      case 'U':
        dot();
        dot();
        dash();
        break;

      case 'V':
        dot();
        dot();
        dot();
        dash();
        break;

      case 'W':
        dot();
        dash();
        dash();
        break;

      case 'X':
        dash();
        dot();
        dot();
        dash();
        break;

      case 'Y':
        dash();
        dot();
        dash();
        dash();
        break;

      case 'Z':
        dash();
        dash();
        dot();
        dot();
        break;

      default:
        Serial.println("Default execution case triggered");
        error();
        break;
    }
    Serial.end();
    Serial.begin(9600);
    Serial.println("Enter the alphabet character (capitalized) to display using LED in Morse Code:");
  }
}
