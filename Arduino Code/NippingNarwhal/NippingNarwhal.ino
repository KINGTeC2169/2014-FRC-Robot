#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>

// Ethernet Config
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte gateway[] = {10,21,69,1};
byte subnet[] = {255,255,255,0};
IPAddress ip(10,21,69,7);
EthernetServer server(1180);

// Robot State Config                                                        
//
// Note: This section is likely to change frequently up until
// the lights are installed on the competition robot. Confirm that
// any changes are reflected across both the LabVIEW broadcasting
// and Arduino receiving code before commiting.
//
// Higher values take precedence, e.g. if the robot is in
// autonomous (2) and shooting (4) the state will be set to
// shooting (4).
//
// "0" implies the robot is disabled
// "1" implies the robot is enabled in tele-op
// "2" implies the robot is enabled in autonomous
// "3" implies the robot is spinning
// "4" implies the robot is shooting
String robotState = "";


void setup() {
  // Configure default Serial COM to communicate at 57600 baud
  Serial.begin(57600);

  // Initialize Ethernet Server (TCP)
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  server.begin();
  Serial.print("Webserver is at ");
  Serial.println(Ethernet.localIP());  
}
void loop() {
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    String clientMsg ="";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == ',') {
          robotState = clientMsg;
          clientMsg = "";
        } else {
          clientMsg += c;
        }
        if (robotState == "10") {
          Serial.print("Test\n");
        } else {
          Serial.print("\n");
        }
        robotState = "";
      }
    }
    // Give the Client time to receive the data
    delay(50);
    // Close the connection:
    client.stop();
  }
  delay(100);
}