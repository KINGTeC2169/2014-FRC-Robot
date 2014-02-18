#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>

// Ethernet Config
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte gateway[] = {10,21,69,1};
byte subnet[] = {255,255,255,0};
IPAddress ip(10,21,69,7);
EthernetServer server(1180);

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

