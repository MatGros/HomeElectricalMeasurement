/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

void setup() {
  // prepare GPIO2 (LED sur l'ESP12-2 du nodemcu V3)
  // pinMode(2, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(5, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(5, HIGH);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(50);                      // Wait for a second
  digitalWrite(5, LOW);  // Turn the LED off by making the voltage HIGH
  delay(4950);                      // Wait for two seconds (to demonstrate the active low LED)
  
//  digitalWrite(5, LOW);
//  delay(100);
//  digitalWrite(5, HIGH);
//  delay(100);
//  
//  digitalWrite(5, LOW);
//  delay(50);
//  digitalWrite(5, HIGH);
//  delay(500);

  
}
