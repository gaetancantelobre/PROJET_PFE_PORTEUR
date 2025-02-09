// Raspberry Pi Pico - Receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> // Include the Servo library

#define CE_PIN 22 // Adjust if needed
#define CSN_PIN 17  // Adjust if needed

RF24 radio(CE_PIN, CSN_PIN); // CE, CSN

const byte addresses[][6] = {"00001"}; // Same address as the transmitter
Servo servo; // Create a servo object

const int servoPin = 15; // Define the GPIO pin connected to the servo

void setup() {
  Serial.begin(115200);

  delay(1000);
  // while (!Serial) {
  //   // some boards need to wait to ensure access to serial over USB
  // }
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
 
  
  radio.openReadingPipe(0, addresses[0]); // Setting the address for reception
  radio.startListening(); // Setting the module as receiver

  servo.attach(servoPin); // Attach the servo to the specified pin
  pinMode(25,OUTPUT);
}

void loop() {
  if (radio.available()) {
    unsigned int receivedAngle;
    radio.read(&receivedAngle, sizeof(receivedAngle)); // Read the angle.  Crucial!
    Serial.println(receivedAngle);
    if(receivedAngle > 0)
    {
      digitalWrite(25,1);
    }
    else
    {
      digitalWrite(25,0);
    }
    servo.write(receivedAngle);
  }
}