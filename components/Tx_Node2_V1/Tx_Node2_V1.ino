
// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <HardwareSerial.h> //included from Sensing Code
#include <TinyGPS++.h> 
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

//GPS Constants
static const uint32_t GPSBaud = 9600;
long lasMsg = 0;
long lastReconnectAttempt = 0;
char msg[50];
int value = 0;
String nodeID = String(EEPROM.read(0));
char topic[] = "capstone_test";
String full, lati, longi, mon, d, yr, hr, m, s, alt, sats;
double preLati, preLong;
int negLong, negLati, negAlt;

//GPS Object
TinyGPSPlus gps;

HardwareSerial hs(0);

 
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(115200);
   hs.begin(9600,SERIAL_8N1, 16, 17); // for GPS using Tx Rx pins
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
//GPS Code

while(hs.available() > 0) {
    if (gps.encode(hs.read())){
        gps.encode(hs.read());
 /*     
        lati = String(gps.location.lat());
        longi = String(gps.location.lng());
        yr = String(gps.date.year());
        mon = String(gps.date.month());
        d = String(gps.date.day());
        hr = String(gps.time.hour());
        m = String(gps.time.minute());
        s = String(gps.time.second());
        alt = String(gps.altitude.value());
        sats = String(gps.satellites.value());
*/




// FOR LATITUDE
//converting Lati to poitive if negative
if (gps.location.lat() < 0){
  negLati = 1;
  preLati = gps.location.lat() * -1;
}
else{
  negLati = 0;
  preLati = gps.location.lat();
}
// converting to bigger number to convert to 4 bytes
int Lati = preLati*1000000;
int Lati1 = Lati/16777216; //First byte
int Lati2 = Lati/65536; //Second byte
int Lati3 = Lati/256; // third byte
int Lati4 = Lati; // Forth byte

Serial.println(Lati);
Serial.println(negLati);




// FOR LONGITUDE
//converting Longi to poitive if negative
if (gps.location.lng() < 0){
  negLong = 1;
  preLong = gps.location.lng() * -1;
}
else{
  negLong = 0;
  preLong = gps.location.lng();
}
// converting to bigger number to convert to 4 bytes
int Long = preLong*1000000;
int Long1 = Long/16777216; //First byte
int Long2 = Long/65536; //Second byte
int Long3 = Long/256; // third byte
int Long4 = Long; // Forth byte

Serial.println(Long);
Serial.println(negLong);




//uint8_t testLati =  LATI;
// account for uint8_t being able to only go to 
int Alt1 = gps.altitude.meters()/256;
int Alt2 = gps.altitude.meters();
if(gps.altitude.meters() < 0){
  negAlt = 1;
}
else{
  negAlt = 0;
}
Serial.println(gps.altitude.meters());

int Sats = gps.satellites.value();
int Year = gps.date.year() - 2000;
int Month = gps.date.month();
int Day = gps.date.day();
int Hour = gps.time.hour();
int Minute = gps.time.minute();
int Second = gps.time.second();


  //  }
//}   
//**************************************************************************************  
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
  uint8_t nodeData[22];
  nodeData[0] = 2; //node
  nodeData[1] = MoisturePercent100; //moisture level in % provided +6 in order to ensure proper transmission, loses 5% when transmtiting 
  nodeData[2] = negLati; //The overall data for the latitude
  nodeData[3] = Lati1;
  nodeData[4] = Lati2;
  nodeData[5] = Lati3;
  nodeData[6] = Lati4;



  
  nodeData[7] = negLong;
  nodeData[8] = Long1;
  nodeData[9] = Long2;
  nodeData[10] = Long3;
  nodeData[11] = Long4;


  
  nodeData[12] = negAlt; // altitude
  nodeData[13] = Alt1;
  nodeData[14] = Alt2;
  nodeData[15] = Sats;  //sats
  nodeData[16] = Year;
  nodeData[17] = Month;
  nodeData[18] = Day;
  nodeData[19] = Hour;
  nodeData[20] = Minute;
  nodeData[21] = Second;

  //Serial.println(MoisturePercent100); //Giving value to compare to uint8_t of moisture
  //Serial.println(nodeData[1]); //testing to see what the value is when converted to uint8_t
  Serial.println(nodeData[0]);
  Serial.println("Sending...");
  delay(10);
  rf95.send(nodeData, sizeof(nodeData));
 // rf95.send((const uint8_t*) testLati, sizeof(testLati));
  //testing what the testLati is


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
    } // added forGPS stuff
} // added for GPS stuff
}
