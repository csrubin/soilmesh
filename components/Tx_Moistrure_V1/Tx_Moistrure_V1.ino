
// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <HardwareSerial.h> //included from Sensing Code

#define RFM95_INT 33
#define RFM95_CS  14
#define RFM95_RST 32

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// constants for moisture sensor
const float DryValue = 2910;   //maximum value, completely dry
const float WetValue = 1340;  //minimum value, completely wet
float SoilMoistureValue = 0.0;
float MoisturePercent = 0.0;

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
 
  delay(100);
 
  Serial.println("Feather LoRa TX Test!");
 
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
  int x = 153;

}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
 
void loop()
{
//Moisture Sensor Code

SoilMoistureValue = analogRead(A0);  //replace soilmoisture with reading from analog pin
MoisturePercent = 1-((SoilMoistureValue-WetValue)/(DryValue-WetValue)); //Calculated the percentage value of the moisture 
//Serial.println(MoisturePercent); //print the value if wanted
delay(1000); //Take readings every second
int MoisturePercent100 = 100*MoisturePercent; //Converts the moisture decimal into a set of 100 for easier transmiting


//**********************************************************************************
  //Transmission Code
  delay(2000); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server
  
  char radiopacket = 5;
  uint8_t nodeData[12];
  nodeData[0] = 1; //node
  nodeData[1] = MoisturePercent100+6; //moisture level in % provided +6 in order to ensure proper transmission, loses 5% when transmtiting 
  nodeData[2] = 42; // latitude 
  nodeData[3] = -30; // Longitude
  nodeData[4] = 5; // altitude
  nodeData[5] = 45;  //sats
  nodeData[6] = 2025;
  nodeData[7] = 12;
  nodeData[8] = 30;
  nodeData[9] = 23;
  nodeData[10] = 59;
  nodeData[11] = 59;
  //Serial.println(MoisturePercent100); //Giving value to compare to uint8_t of moisture
  //Serial.println(nodeData[1]); //testing to see what the value is when converted to uint8_t
  
  Serial.println("Sending...");
  delay(10);
  rf95.send(nodeData, sizeof(nodeData));

  // The request for a followup signal from a reciever.
 /*
  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
 */
}
