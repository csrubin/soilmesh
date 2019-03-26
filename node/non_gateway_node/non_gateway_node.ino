// Combined mqtt_reconnect_nonblocking and mqtt_esp8266 examples
// To be used only on node1

#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_INT 33
#define RFM95_CS 14
#define RFM95_RST 32
#define RF95_FREQ 915.0

// Global variables
static const uint32_t GPSBaud = 9600;
String nodeID = String(EEPROM.read(0));
String dat, lati, longi, mon, d, yr, hr, m, s, alt, sats;
int16_t packetnum = 0;  // packet counter, we increment per xmission

// Moisture sensor vals
const float DryValue = 3100;   //maximum value, completely dry
const float WetValue = 1000;  //minimum value, completely wet
float SoilMoistureValue = 0.0;
float MoisturePercent = 0.0;

// Object initialization
TinyGPSPlus gps;      // GPS
HardwareSerial hs(0); // Serial port for GPS
RH_RF95 rf95(RFM95_CS, RFM95_INT);  // Radio driver

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  
  Serial.begin(9600);
  while(!serial){
    delay(1);
  }
  delay(100);

  // Radio reset
  Serial.println("Manually resetting radio...");
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // Testing for proper radio initialization
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Testing selected frequency
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
  int x = 153;
  
  // Initializing hardware serial port for GPS
  Serial.println("Initializing hardware serial port for GPS...");
  hs.begin(GPSBaud, SERIAL_8N1, 16, 17); 
  
}

void loop() {
  while (hs.available() > 0){
    
    // Read soil sensor data
    SoilMoistureValue = analogRead(A0);
    MoisturePercent = SoilMoistureValue/DryValue;
    String MP = String(MoisturePercent);
    
    if (gps.encode(hs.read())){
      //displayInfo();
      
      //publish
      lati = String(gps.location.lat());
      longi = String(gps.location.lng());
      yr = String(gps.date.year());
      mon = String(gps.date.month());
      d = String(gps.date.day());
      hr = String(gps.time.hour());
      m = String(gps.time.minute());
      s = String(gps.time.second());
      alt = String(gps.altitude.meters());
      sats = String(gps.satellites.value());

      dat = nodeID + ", " + yr + ", " + mon + ", " + d + ", " + hr + ", " 
      + m + ", " + s + ", " + MP + ", " + lati + ", " + longi + ", " + alt + ", " + sats; 

      char DAT[70];
      dat.toCharArray(DAT,60);
      Serial.println(DAT);
      delay(500);

      // Send over radio

      
    }
  }
}

