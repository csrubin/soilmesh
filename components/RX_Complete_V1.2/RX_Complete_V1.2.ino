// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
// GPS Stuff
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <EEPROM.h>


//********************************************************
#include <SPI.h>
#include <RH_RF95.h>
#include <HardwareSerial.h>

 
#define RFM95_INT 33
#define RFM95_CS  14
#define RFM95_RST 32
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
  
// Blinky on receipt
#define LED 13

TinyGPSPlus gps; //defines the gps ?variable?
HardwareSerial hs(1);

//GPS Constants
/*
static const uint32_t GPSBaud = 9600;
long lasMsg = 0;
long lastReconnectAttempt = 0;
char msg[50];
int value = 0;
String nodeID = String(EEPROM.read(0));
char topic[] = "capstone_test";
String full, lati, longi, mon, d, yr, hr, m, s, alt, sats;
*/
 
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(100);
 
  Serial.println("Feather LoRa RX Test!");
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}
 
void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    Serial.println("Searching... ");
    
 
    if (rf95.recv(buf, &len))
    {
      

      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      /*
    buf[0] = 1; //node
  buf[1] = MoisturePercent100; //moisture level in % provided +6 in order to ensure proper transmission, loses 5% when transmtiting 
  buf[2] = negLati; //The overall data for the latitude
  buf[3] = Lati1;
  buf[4] = Lati2;
  buf[5] = Lati3;
  buf[6] = Lati4;



  
  buf[7] = negLong;
  buf[8] = Long1;
  buf[9] = Long2;
  buf[10] = Long3;
  buf[11] = Long4;


  
  buf[12] = negAlt1; // altitude
  buf[13] = Alt1
  buf[14] = Alt2
  buf[15] = Sats;  //sats
  buf[16] = Year;
  buf[17] = Month;
  buf[18] = Day;
  buf[19] = Hour;
  buf[20] = Minute;
  buf[21] = Second;


       */

      // Conversion from 4 bytes of latitude and Longitude data to double
      int Lati1 = buf[3]*16777216; //First byte
      int Lati2 = buf[4]*65536; //Second byte
      int Lati3 = buf[5]*256; // third byte
      int Lati4 = buf[6]; // Forth byte
    
      double preLati = Lati1+Lati2+Lati3+Lati4;
      double Lati = preLati/1000000;
      if(buf[2] == 1){
        Lati = Lati*-1;
      }

      
      int Long1 = buf[8]*16777216; //First byte
      int Long2 = buf[9]*65536; //Second byte
      int Long3 = buf[10]*256; // third byte
      int Long4 = buf[11]; // Forth byte

      double preLong = Long1+Long2+Long3+Long4;
      double Long = preLong/1000000;
      if(buf[7] == 1){
        Long = Long*-1;
      }

      Serial.println(Lati,6);
      Serial.println(Long,6);

      int alt1 = buf[13]*256;
      int alt2 = buf[14];
      int alt = alt1+alt2;

      if(buf[12] == 1){
        alt = alt*-1;
      }
      
      Serial.println(alt);
      
      
      
     // Serial.println(Lati); //testing to see if moisture is correct value
       Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
 
 /*     // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
   */ 
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
