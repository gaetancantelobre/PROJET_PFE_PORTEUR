#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


int ledPin=25;
struct Message {
  byte user_id;
  byte msg_type;
  int data;
};

class Nacelle {
  private:
    const int ce_pin;
    const int csn_pin;
    const byte address[6] = { '0', '0', '0', '0', '1', '\0' };
    const byte binding_request = 1;
    const byte id_request = 2;
    const byte status_request = 9;
    const int nc_id;
    int target_id = -1;
    long last_status_update;
    RF24 radio; // Radio object as a member

  public:
    // Constructor
    Nacelle(int ce, int csn) : ce_pin(ce), csn_pin(csn), radio(ce_pin, csn), nc_id(25) { // Initialize radio and nc_id correctly
    }

    bool set_up_radio_connection()
    {
      if (!radio.begin()) {
        Serial.println(F("radio hardware is not responding!!"));
        return false;    // Infinite loop if radio fails
      }
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_HIGH);  // Set power level
      radio.setDataRate(RF24_1MBPS);   // Set data rate
      radio.stopListening();
      Serial.println(F("NRF24 setup done"));
      return true;
    }

    bool binding_process()
    {
      radio.startListening();
      while (!radio.available())
      {
        delay(10);
      }

      Message recieved_msg;
      radio.read(&recieved_msg, sizeof(recieved_msg));

      if (recieved_msg.msg_type != 1) { // Check message type first
        Serial.println(F("Binding failed: incorrect message type"));
        return false;
      }

      target_id = recieved_msg.user_id;

      radio.stopListening();

      Message binding_message;
      binding_message.user_id = nc_id;
      binding_message.msg_type = binding_request;
      binding_message.data = 1;
      radio.write(&binding_message, sizeof(binding_message));
      Serial.print(F("Binding complete with :"));
      Serial.println(target_id);
      return true;
    }

    void print_nc_status()
    {
      Serial.print("nc_id = ");
      Serial.println(nc_id);
      Serial.print("target_id = ");
      Serial.println(target_id);
    }

    void test_recieve_message()
  {
    radio.startListening();
    if (radio.available()) {
         Message received_message;
         radio.read(&received_message, sizeof(received_message));

         Serial.println(F("Message received!"));
         Serial.print(F("User ID: "));
         Serial.println(received_message.user_id);
         Serial.print(F("Message Type: "));
         Serial.println(received_message.msg_type);
         Serial.print(F("Data: "));
         Serial.println(received_message.data);
    } 
    
  }



  void test_send_message()
  {

    radio.stopListening();
    Message msg;
    msg.user_id = 1;         // Assign user ID (can be any value)
    msg.msg_type = 2;        // Set message type (example value)
    msg.data = 1234;         // Set the data to be sent

    Serial.print("Sending user_id: ");
    Serial.println(msg.user_id);
    Serial.print("Sending msg_type: ");
    Serial.println(msg.msg_type);
    Serial.print("Sending data: ");
    Serial.println(msg.data);

    digitalWrite(ledPin, HIGH);  // Turn LED on while sending a message

    // Send the message
    bool success = radio.write(&msg, sizeof(msg));
    if (success) {
      Serial.println(F("Message sent successfully"));
    } else {
      Serial.println(F("Message failed to send"));
    }

    delay(500);  // Keep the LED on for a while to indicate sending
    digitalWrite(ledPin, LOW);  // Turn LED off after the message is sent

    delay(1000);  // Wait for 1 second before sending again
  }

};  

Nacelle ncs(9, 10); // Now this line works because the class is fully defined

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  ncs.set_up_radio_connection();
  ncs.print_nc_status();
  //ncs.binding_process();
}

void loop() {
  // Your loop code here
  ncs.test_send_message();
}
