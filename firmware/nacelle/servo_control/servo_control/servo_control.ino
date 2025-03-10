#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6    // Pin for WS2812B LED
#define NUM_LEDS 1   // Number of LEDs
#define SERVO_PIN 2  // Pin for the servo
#define ONBOARD_LED 25 // Onboard LED for Pi Pico
#define SWITCH1 10

int loading_mode = 0;


void set_servo_angle(int i)
{
  if(i > 100)
    i = 100;
  if(i<0)
    i = 0;

  myServo.write(i);
}



int closed = 0;
Servo myServo;
Adafruit_NeoPixel ledStrip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(ONBOARD_LED, OUTPUT); // Set onboard LED as output
    myServo.attach(SERVO_PIN);
    ledStrip.begin();
    ledStrip.show(); // Initialize LED strip
    
   set_servo_angle(0); // Set initial servo position

    Serial1.begin(115200); // Initialize UART (RX: GP1, TX: GP0 by default)
    ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0));
    ledStrip.show();
    pinMode(SWITCH1,INPUT_PULLUP);
    
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

void launch_procedure()
{
  set_servo_angle(90);
  ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0));
}


void loop() {

  if(!closed && !digitalRead(SWITCH1) && loading_mode)
  {
    closed = 1;
    set_servo_angle(90);
    Serial1.write("loaded");
    ledStrip.setPixelColor(0, ledStrip.Color(255, 0, 0));
  }
    

  if (Serial1.available()) {
      digitalWrite(ONBOARD_LED, HIGH); // Turn on onboard LED when receiving data
      String received = Serial1.readStringUntil('\n');
      received.trim(); // Remove extra spaces and newlines
      
      if(received == "load")
        loading_mode = 1;
      if(received == "launch")
        launch_procedure()
      
      
      
      // Set LED color based on position (example: red at 180, green at 0)
      // int red = map(servoPos, 0, 180, 0, 255);
      // int green = map(servoPos, 0, 180, 255, 0);
      // ledStrip.setPixelColor(0, ledStrip.Color(red, green, 0));
      // ledStrip.show();
  }
}
