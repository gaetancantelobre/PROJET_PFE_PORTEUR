#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "hardware/watchdog.h" 

#define NUM_LEDS 1   

#define LED_PIN_1 6    
#define LED_PIN_2 7    
#define LED_PIN_3 8    
#define LED_PIN_4 9    

#define SERVO_PIN_1 2  
#define SERVO_PIN_2 3  
#define SERVO_PIN_3 4  
#define SERVO_PIN_4 5  

#define SWITCH_1 10
#define SWITCH_2 11
#define SWITCH_3 12
#define SWITCH_4 13

#define ONBOARD_LED 25 

#define OPEN_ANGLE 90
#define CLOSE_ANGLE 5

#define RESTART_BUT 14



class GrabberModule {
  private:
      int pwmPin;       // GPIO for PWM control (Servo)
      int buttonPin;    // GPIO for Button input
      int ledPin;       // GPIO for LED control
      int active;
      Servo servo;      // Servo object
      Adafruit_NeoPixel status_led; // NeoPixel LED object
  
      int openAngle = OPEN_ANGLE; // Define the angle when open
      int closeAngle = CLOSE_ANGLE; // Define the angle when closed
      int currentAngle;   // Track the current servo angle
  
  public:
      // Constructor
      GrabberModule(int pwm, int button, int led)
          : pwmPin(pwm), buttonPin(button), ledPin(led), status_led(NUM_LEDS, led, NEO_GRB + NEO_KHZ800) {}
  
      // Initialize hardware
      void begin() {
          active = 0;
          pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up
          servo.attach(pwmPin);
          status_led.begin();
          status_led.show(); // Initialize all pixels to 'off'
          openServo();
      }


      void openServo()
      {
        servo.write(openAngle);
        status_led.setPixelColor(0, status_led.Color(0, 255, 0));
        status_led.show();

        currentAngle = openAngle;
      }


      bool getActive()
      {
        return active;
      }

      void setActive(int i)
      {
        i = constrain(i,0,1);
        active = 1;
      }

      void closeServo()
      {
        servo.write(closeAngle);
        currentAngle = closeAngle;
        status_led.setPixelColor(0, status_led.Color(125, 255, 0));
        status_led.show();

      }
  
      // Set servo angle (0-180 degrees)
      void setServoAngle(int angle) {
          angle = constrain(angle, 0, 180);
          servo.write(angle);
          currentAngle = angle; // Update angle tracking
      }
  
      // Read button state (returns true if pressed)
      bool isButtonPressed() {
          return digitalRead(buttonPin) == LOW; // Assuming active-low button
      }
  
      // Control LED color (RGB)
      void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
          status_led.setPixelColor(0, status_led.Color(r, g, b));
          status_led.show();
      }
  
      // Get servo status: true if open, false if closed
      bool getServoState() {
          return currentAngle == openAngle;
      }


      void open_procedure() {
        setServoAngle(90); // Open the grabber
        setLedColor(0, 255, 0); // Green LED for open
      }

      void close_procedure() {
          setServoAngle(0); // Close the grabber
          setLedColor(255, 0, 0); // Red LED for closed
      }

      bool launch_procedure()
      {
        if(getActive())
        {
          open_procedure();
          setActive(0);
          return true;
        }
      
        return false;
      }

      bool load_procedure(unsigned long duration_ms) {
        unsigned long startMillis = millis(); // Start timer
        while (millis() - startMillis < duration_ms) {
            if (isButtonPressed()) { 
                setActive(1);
                return true;  
            }
        }
        return false;  // Return false if button not pressed within the duration
      }
  };


int loading_mode = 0;
Servo myServo;

std::vector<GrabberModule> grabbers;

void start_modules()
{
  grabbers.push_back(GrabberModule(2, 10, 6)); 
  grabbers.push_back(GrabberModule(3, 11, 7));
  grabbers.push_back(GrabberModule(4, 12, 8));
  grabbers.push_back(GrabberModule(5, 13, 9));

  for (auto &grabber : grabbers) {
    grabber.begin();
  }
}


void serial_check()
{
    // Check if Serial1 starts properly, blink onboard LED if it fails
  if (!Serial1) { 
    while (true) {
        digitalWrite(ONBOARD_LED, HIGH);
        delay(500);
        digitalWrite(ONBOARD_LED, LOW);
        delay(500);
    }
}
}

void setup() { 
    start_modules();
    loading_mode = 0;
    Serial.begin(115200);
    Serial1.begin(115200); // Initialize UART (RX: GP1, TX: GP0 by default)
    pinMode(RESTART_BUT,INPUT_PULLUP);
    serial_check();
}



void check_restart()
{
  if(!digitalRead(RESTART_BUT))
  {
    setup();
    watchdog_reboot(0, 0, 0);
  }
}

void loop() {
  if (Serial1.available() > 0) {
      String received = Serial.readString(); // Read the input command
      int moduleNum = -1;
      Serial.println(received);
      // Check if the received string has "load X" or similar
      if (received.startsWith("load")) {
          // Parse the number after "load"
          moduleNum = received.substring(5).toInt();
          if (moduleNum >= 1 && moduleNum <= grabbers.size()) {
              // Perform load action for the correct module
              Serial1.println("Entered loading mode for module " + String(moduleNum));
              bool loaded = grabbers[moduleNum - 1].load_procedure(15000); // Open the specified grabber
              if(loaded)
                Serial1.println("Loaded : " + String(moduleNum));
              else
                Serial1.println("failed to load");
          }
      }
      else if (received.startsWith("status_req")) {
        // Parse the number after "load"
        String allStatuses = "";
        int grabberId = 1; // Assuming grabbers are numbered starting from 1
        for (auto &grabber : grabbers) {
            // Get the status of the current grabber
            String status = (grabber.getServoState() == 1) ? "open" : "closed"; // Modify based on status logic
    
            // Append grabber ID and status to the allStatuses string
            allStatuses += "Grabber " + String(grabberId) + " : " + status + "  ";
    
            // Increment the grabber ID for the next grabber
            grabberId++;
        }
        Serial1.println(allStatuses);
        }
      else if (received.startsWith("open")) {
        // Parse the number after "load"
        moduleNum = received.substring(5).toInt();
        if (moduleNum >= 1 && moduleNum <= grabbers.size()) {
            // Perform load action for the correct module
            
           grabbers[moduleNum - 1].open_procedure(); // Open the specified grabber
          Serial1.println("open_success");
        }
    }
      else if (received.startsWith("close")) {
        // Parse the number after "load"
        moduleNum = received.substring(5).toInt();
        if (moduleNum >= 1 && moduleNum <= grabbers.size()) {
            // Perform load action for the correct module
            
          grabbers[moduleNum - 1].close_procedure(); // Open the specified grabber
          Serial1.println("close_success");
        }
    }
      // Handle other commands for modules
      else if (received.startsWith("launch")) {
        // Parse the number after "load"
        moduleNum = received.substring(5).toInt();
        if (moduleNum >= 1 && moduleNum <= grabbers.size()) {
            // Perform load action for the correct module
            bool launch = grabbers[moduleNum - 1].launch_procedure(); // Open the specified grabber
            if(launch)
              Serial1.println("Launch succesful");
            else
              Serial1.println("Module " + String(moduleNum) +" not loaded.");
        }
      }

      else if (received.startsWith("status_req")) {
        Serial1.println("reset confirmation");
        setup();
        watchdog_reboot(0, 0, 0);
      }

      else if (received == "force_open") {
          for (auto &grabber : grabbers) {
              grabber.open_procedure();
          }
        Serial1.println("open_forced");

      }

      else if (received == "force_close") {
          for (auto &grabber : grabbers) {
              grabber.close_procedure();
          }
        Serial1.println("close_forced");
      }

      else if (received == "cycle_force") {
          for (auto &grabber : grabbers) {
              grabber.open_procedure();
              delay(1000);
              grabber.close_procedure();
              delay(1000);
          }
      }

      else
        Serial1.println("Unknown command refer to \"help\" command.");
  }
}