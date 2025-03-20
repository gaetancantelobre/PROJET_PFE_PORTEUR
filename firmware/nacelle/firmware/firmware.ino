#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "hardware/watchdog.h" 
#define LED_PIN 6    // Pin for WS2812B LED
#define NUM_LEDS 1   // Number of LEDs
#define SERVO_PIN 2  // Pin for the servo
#define ONBOARD_LED 25 // Onboard LED for Pi Pico
#define SWITCH1 10

#define OPEN_ANGLE 90
#define CLOSE_ANGLE 5

#define RESTART_BUT 14

int loading_mode = 0;
Servo myServo;


void set_servo_angle(int i)
{
  if(i > 100)
    i = 100;
  if(i<0)
    i = 0;

  myServo.write(i);
}

int closed = 0;
Adafruit_NeoPixel ledStrip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() { 
    loading_mode = 0;
    closed = 0;
    myServo.attach(SERVO_PIN);
    pinMode(ONBOARD_LED, OUTPUT); // Set onboard LED as output
    ledStrip.begin();
    ledStrip.show(); // Initialize LED strip
    
   set_servo_angle(OPEN_ANGLE); // Set initial servo position

    Serial1.begin(115200); // Initialize UART (RX: GP1, TX: GP0 by default)
    ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0));
    ledStrip.show();
    pinMode(SWITCH1,INPUT_PULLUP);
    pinMode(RESTART_BUT,INPUT_PULLUP);

    
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

void open_procedure()
{
  set_servo_angle(OPEN_ANGLE);
  ledStrip.setPixelColor(0, ledStrip.Color(0, 255, 0));
}

void check_restart()
{
  if(!digitalRead(RESTART_BUT))
  {
    setup();
    watchdog_reboot(0, 0, 0);
  }
}

void close_procedure()
{
  set_servo_angle(CLOSE_ANGLE);
  ledStrip.setPixelColor(0, ledStrip.Color(255, 0, 0));
  ledStrip.show();
}

void loop() {
  check_restart();
  if(!closed && !digitalRead(SWITCH1) && loading_mode)
  {
    closed = 1;
    Serial1.write("loaded\n");
    close_procedure();
  }
  
  if (Serial1.available()) {
      digitalWrite(ONBOARD_LED, HIGH); // Turn on onboard LED when receiving data
      String received = Serial1.readStringUntil('\n');
      received.trim(); // Remove extra spaces and newlines
      if(received == "load")
      { 
        loading_mode = 1;
        Serial.println(" entered loading mode");
        ledStrip.setPixelColor(0, ledStrip.Color(0, 0, 255));
        ledStrip.show();
      }
      if(received == "launch" && closed)
        open_procedure();
      if(received == "open_force")
        open_procedure();
      if(received = "cycle_force")
      { 
        open_procedure();
        delay(1000);
        close_procedure();
        delay(1000);
      }
      ledStrip.show();
  }
}
