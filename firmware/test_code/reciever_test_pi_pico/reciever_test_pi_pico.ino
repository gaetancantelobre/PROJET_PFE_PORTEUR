// Raspberry Pi Pico - Receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h> // Include the Servo library


#define CE_PIN 22 // Adjust if needed
#define CSN_PIN 17  // Adjust if needed

#include <FastLED.h>


#define NUM_LEDS 1    // Number of LEDs in your strip (now 1)
#define DATA_PIN 23  // Pin connected to the data line of the LED strip
CRGB leds[NUM_LEDS];

RF24 radio(CE_PIN, CSN_PIN); // CE, CSN

const byte addresses[][6] = {"00001"}; // Same address as the transmitter
Servo servo; // Create a servo object

const int servoPin = 15; // Define the GPIO pin connected to the servo


void rainbow() {
  static uint8_t hue = 0; // Start hue at 0

  leds[0] = CHSV(hue, 255, 255); // Set the color of the single LED

  hue++; // Increment hue for animation
}


void setup() {

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // Initialize FastLED
  FastLED.setBrightness(100); // Set brightness (0-255)
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
  rainbow();
  FastLED.show(); // Update the LEDs
  delay(10);      // Adjust delay for speed
}