#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6    // Pin for WS2812B LED
#define NUM_LEDS 1   // Number of LEDs
#define SERVO_PIN 2  // Pin for the servo
#define ONBOARD_LED 25 // Onboard LED for Pi Pico

Servo myServo;
Adafruit_NeoPixel ledStrip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(ONBOARD_LED, OUTPUT); // Set onboard LED as output
    myServo.attach(SERVO_PIN);
    ledStrip.begin();
    ledStrip.show(); // Initialize LED strip
    
    myServo.write(45);

    Serial1.begin(115200); // Initialize UART (RX: GP1, TX: GP0 by default)
    ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0));
    ledStrip.show();
    // Check if Serial1 starts properly, blink onboard LED if it fails
    if (!Serial1) {
        while (true) {
            digitalWrite(ONBOARD_LED, HIGH);
            delay(500);
            digitalWrite(ONBOARD_LED, LOW);
            delay(500);
        }
    }
    ledStrip.setPixelColor(0, ledStrip.Color(255, 255, 255));
    ledStrip.show();
}

void loop() {
    if (Serial1.available()) {
        digitalWrite(ONBOARD_LED, HIGH); // Turn on onboard LED when receiving data
        String received = Serial1.readStringUntil('\n');
        received.trim(); // Remove extra spaces and newlines
        Serial.println(received);

        if (received == "on") {
            myServo.write(180);
            ledStrip.setPixelColor(0, ledStrip.Color(255, 0, 0)); // Red
            ledStrip.show();
        } else if (received == "off") {
            myServo.write(0);
            ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0)); // Green
            ledStrip.show();
        }
        delay(100); // Briefly keep LED on
        digitalWrite(ONBOARD_LED, LOW); // Turn off onboard LED after processing
    }
}
