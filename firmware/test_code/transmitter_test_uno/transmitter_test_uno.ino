// Pi pico - Transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 22  // Changed to a standard digital pin
#define CSN_PIN 17 // Changed to a standard digital pin

RF24 radio(CE_PIN, CSN_PIN); // CE, CSN

const byte addresses[][6] = {"00001"}; // Define a unique address

void setup() {
  Serial.begin(115200);
  delay(1000);
  // while (!Serial) {
  //   // some boards need to wait to ensure access to serial over USB
  // }
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}    // hold in infinite loop
  }
  radio.openWritingPipe(addresses[0]); // Setting the address for transmission
  radio.stopListening(); // Setting the module as transmitter
  pinMode(15, INPUT_PULLUP); // Keep this line, but the pin is not used for CE/CSN anymore
  pinMode(25, OUTPUT);
  Serial.println(F("SETUP DONE"));
}

void loop() {
  // Simulate angle readings (replace with your sensor readings)
  unsigned int angle; // Use an appropriate data type
  Serial.println(digitalRead(15));
  if (digitalRead(15) == LOW) { // Check for LOW since the button is pull-up
    angle = 180;
    digitalWrite(25,1);
  } else {
    angle = 0;
    digitalWrite(25,0);
  }

  radio.write(&angle, sizeof(angle)); // Send the angle using its size.  Crucial!
  delay(100);
}