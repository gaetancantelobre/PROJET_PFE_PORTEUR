#include <Servo.h>
#include <Adafruit_NeoPixel.h>

class Grabber {        // The class
  public:          // Access specifier
    int id;  // Attribute
    const int servo_pin;  // Attribute
    const int led_pin;
    const int detector_pin;
    Servo servo_controller;
    int closing_range = 90;
    Adafruit_NeoPixel control_led;

// Constructor definition outside the class
  Grabber::Grabber(int id, int servo_pin, int led_pin, int det_pin) {
    id = id;
    servo_pin = servo_pin;
    led_pin = led_pin;
    detector_pin = det_pin;
    pinMode(detector_pin,INPUT_PULLUP);
    control_led(1, servo_pin, NEO_GRB + NEO_KHZ800);
    control_led.begin();
    control_led.show();

    strip1.fill(strip1.Color(255, 0, 0));
    strip1.show();
    delay(100);
    strip1.fill(strip1.Color(0, 255, 0));
    strip1.show();

    servo_controller.attach(servo_pin);
    servo_controller.write(0);
    Serial.print("Grabber module ");
    Serial.print(id);
    Serial.println(" setup succesful!");
  }


  void Grabber::close_module()
  {
    servo_controller.write(closing_range);
    strip1.fill(strip1.Color(255, 255, 0));
  }

  void Grabber::open_module()
  {
    servo_controller.write(closing_range);
    strip1.fill(strip1.Color(0, 255, 0));
  }

  bool Grabber::load()
  {
    if(!digitalRead(detector_pin))
    {
      return True;
    }
    else
      return False;
  }
}



class Nacelle {        // The class
  public:          // Access specifier
    Grabber grabbers[4];

  Nacelle::Nacelle(Grabber g1,Grabber g2,Grabber g3, Grabber g4)
  {
    grabbers[0] = g1;
    grabbers[1] = g2;
    grabbers[2] = g3;
    grabbers[3] = g4;
    Serial.println("Setup done");
  }

  Nacelle::close_grabber(int grabber_id)
  {
    grabbers[grabber_id].close_module();
  }

  Nacelle::open_grabber(int grabber_id)
  {
    grabbers[grabber_id].open_module();
  }

}

  
