#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int ledPin = 25;
struct Message {
  byte user_id;
  byte msg_type;
  int data;
};

class GroundControlStation {
private:
  const int ce_pin;
  const int csn_pin;
  const byte address[6] = "00001";
  const byte binding_request = 1;
  const byte status_request = 9;
  const int gc_id;
  int target_id = -1;
  long last_status_update;
  RF24 radio; // Radio object as a member

public:
  // Constructor
  GroundControlStation(int ce, int csn) : ce_pin(ce), csn_pin(csn), radio(ce, csn), gc_id(random(256)) { // Initialize radio and gc_id correctly
  //  
  }

  bool set_up_radio_connection()
  {
    if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
      return false;    // Infinite loop if radio fails
    }
    radio.openWritingPipe(address);
    radio.stopListening();
    Serial.println(F("NRF24 setup done"));
    return true;
  }

    bool binding_mode() 
  {
    radio.stopListening();
    Message binding_message;
    binding_message.user_id = gc_id;
    binding_message.msg_type = binding_request;
    binding_message.data = 0;
    radio.write(&binding_message, sizeof(binding_message));
    
  
    // Wait for answer

    radio.startListening();
    unsigned long start = millis();
    while (!radio.available() && millis() - start < 3000) { // Wait 3 seconds
      delay(10);
    }
    if (!radio.available()) {
      Serial.println(F("No response to binding request.")); // More descriptive message
      return false; // No response, failed
    }

    Message confirmation;
    radio.read(&confirmation, sizeof(confirmation));

    if (confirmation.msg_type != 1) { // Check message type first
      Serial.println(F("Binding failed: incorrect message type"));
      return false;
    }
    if (confirmation.data != 1) {
      Serial.println(F("Binding failed: Incorrect confirmation data"));
      return false;
    }

    target_id = confirmation.user_id; // Set target ID only after successful confirmation

    Serial.print(F("Binding successful! Target ID: "));
    Serial.println(target_id);

    radio.stopListening();
    return true; // Return true to indicate success
  }

   void print_gc_status()
  {
    Serial.print("gc_id = ");
    Serial.println(gc_id);
    Serial.print("target_id = ");
    Serial.println(target_id);
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

}; 



GroundControlStation gcs(22, 17); // Now a global object

void setup() {
    Serial.begin(9600);      // Initialize Serial FIRST in setup()
    delay(1000);
    Serial.println(F("Starting Ground Control..."));
    gcs.set_up_radio_connection();
    //gcs.print_gc_status(); // Now this will work!
    gcs.binding_mode();
}

void loop() {
 //gcs.test_recieve_message();
}