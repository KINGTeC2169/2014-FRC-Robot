#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN1 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN1, NEO_GRB + NEO_KHZ800);
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

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
  
  strip.begin();
  strip.setBrightness(64);
  strip.show(); // Initialize all pixels to 'off'


}
void loop() {
  // Listen for incoming clients
  EthernetClient client = server.available();
  
    int received = 0, elapsedTime = 0;
  if (client) {
    String clientMsg ="";
    while (client.connected() && (received == 0) && (elapsedTime <= 1000)) {
      if (client.available()) {
        char c = client.read();
        clientMsg+=c;
        Serial.println("c: "+clientMsg);
        if (c == ',') {
          robotState = clientMsg;
          clientMsg="";
          received=1;
        } 
      }
      delay(10);
      elapsedTime+=10;
      Serial.println("elapsedTime: "+elapsedTime);
    }
    // Give the Client time to receive the data
    delay(50);
    // Close the connection:
    client.stop();
  }
  
        Serial.println("robotState: "+robotState);
  if (robotState == "0,") {
    disabled(1000);
   }
  if (robotState == "1,") {
    teleop(5);
   }
  if (robotState == "2,") {
    colorChase(strip.Color(0,255,0),100);
    fillStrip(0,10);
    shooting(1000);
   } 
  if (robotState == "3,") {
    autonomous(3);
   } else {
    Serial.print(robotState + "\n");
   }
  delay(100);
}

void disabled(uint16_t wait) {
  fillStrip(strip.Color(255,0,0),0);
  delay(wait);
  fillStrip(0,0);
  delay(wait);
}
void teleop(uint16_t wait) {
  rainbow(wait);
}
void autonomous(uint16_t wait){
  ktWave(3, wait);  
}
void shooting(uint16_t wait){
  dashChase(strip.Color(0,255,0),wait);
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void fillStrip(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
      delay(wait);
}
void colorChase(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
void dashChase(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      if(i>=5) {strip.setPixelColor(i-5, 0);}
      strip.show();
      delay(wait);
  }
}

void chasingDashedLine(uint32_t c,int cycles, uint8_t wait) {
  ktWave(cycles, wait);  
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



void ktWave( int cycles, uint8_t wait) {
  float y;
  byte  r, g, b, r2, g2, b2;

  // Need to decompose color into its r, g, b elements
  g = (strip.Color(0,127,0) >> 16) & 0x7f;
  r = (strip.Color(0,127,0) >>  8) & 0x7f;
  b =  strip.Color(0,127,0)        & 0x7f; 

  for(int x=0; x<(strip.numPixels()*5); x++)
  {
    for(int i=0; i<strip.numPixels(); i++) {
      y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
      if(y >= 0.0) {
        // Peaks of sine wave are white
        y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
        r2 = 1.5* (127 - (byte)((float)(127 - r) * y));
        g2 = .8 * (127 - (byte)((float)(127 - g) * y));
        b2 = 0;
      } else {
        // Troughs of sine wave are black
        y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
        r2 = (byte)((float)r * y);
        g2 = (byte)((float)g * y);
        b2 = (byte)((float)b * y);
      }
      strip.setPixelColor(i, r2, g2, b2);
    }
    strip.show();
    delay(wait);
  }
}

// Sine wave effect
#define PI 3.14159265
void wave(uint32_t c, int cycles, uint8_t wait) {
  float y;
  byte  r, g, b, r2, g2, b2;

  // Need to decompose color into its r, g, b elements
  g = (c >> 16) & 0x7f;
  r = (c >>  8) & 0x7f;
  b =  c        & 0x7f; 

  for(int x=0; x<(strip.numPixels()*5); x++)
  {
    for(int i=0; i<strip.numPixels(); i++) {
      y = sin(PI * (float)cycles * (float)(x + i) / (float)strip.numPixels());
      if(y >= 0.0) {
        // Peaks of sine wave are white
        y  = 1.0 - y; // Translate Y to 0.0 (top) to 1.0 (center)
        r2 = 127 - (byte)((float)(127 - r) * y);
        g2 = 127 - (byte)((float)(127 - g) * y);
        b2 = 127 - (byte)((float)(127 - b) * y);
      } else {
        // Troughs of sine wave are black
        y += 1.0; // Translate Y to 0.0 (bottom) to 1.0 (center)
        r2 = (byte)((float)r * y);
        g2 = (byte)((float)g * y);
        b2 = (byte)((float)b * y);
      }
      strip.setPixelColor(i, r2, g2, b2);
    }
    strip.show();
    delay(wait);
  }
}


