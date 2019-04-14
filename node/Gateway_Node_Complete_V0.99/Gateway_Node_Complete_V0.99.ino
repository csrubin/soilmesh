// To be used only on node1
// More description here

// Libaries to be included
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <SPI.h>
#include <RH_RF95.h>

// LoRa Radio constants
#define RFM95_INT 33
#define RFM95_CS 14
#define RFM95_RST 32
#define RF95_FREQ 915.0

// WiFi connection parameters
const char* ssid = "Connor Rubin iPhone";
const char* password = "csr215259"; 

// MQTT parameters
const char* mqtt_server = "broker.hivemq.com"; //"broker.mqtt-dashboard.com";
long lasMsg = 0;
long lastReconnectAttempt = 0;

// Global variables
static const uint32_t GPSBaud = 9600;
String nodeID = String(EEPROM.read(0));
char topic1[] = "node1";
char topic2[] = "node2";
char topic3[] = "node3";
String dat, lati, longi, mon, d, yr, hr, m, s, alt, sats;
String MP;
double Lati, Long;
int ID, Moist, Sats, Year, Month, Day, Hour, Minute, Second, Alt;
int GPSFlag = 2;

// Unsure if needed
char msg[50];
int value = 0;

// Moisture sensor vals
const float DryValue = 3100;   //maximum value, completely dry
const float WetValue = 1000;  //minimum value, completely wet
float SoilMoistureValue = 0.0;
float MoisturePercent = 0.0;

// Object initialization
WiFiClient espClient;
PubSubClient client(espClient);
TinyGPSPlus gps;
HardwareSerial hs(0);
RH_RF95 rf95(RFM95_CS, RFM95_INT); // Radio driver

void setup() {
  // Serial monitor begin
  Serial.begin(115200);

  // Initialize WiFi
  Serial.println("Initializing WiFi...");
  setup_wifi();

  // Initialize MQTT
  Serial.println("Initializing MQTT protocol...");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Initialize hardware serial2
  Serial.println("Initializing hardware serial port for GPS...");
  hs.begin(GPSBaud, SERIAL_8N1, 16, 17); 

  // MQTT reconnect variable
  lastReconnectAttempt = 0;

  // Initialize reset pin
  pinMode(RFM95_RST, OUTPUT);
  
  // Manual reset
  radioReset();

  // Initialize LoRa
  Serial.println("Initializing LoRa radio...");
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
 
  // Ensure proper frequency for transmission
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); 
  Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);

  Serial.println("All hardware initialized. Entering loop.");
}

void loop() {
  while (hs.available() > 0){
    
    if (gps.encode(hs.read())){
      //displayInfo();
      
      if (!client.connected()) {
        long now = millis();
        if (now - lastReconnectAttempt > 5000) {
          lastReconnectAttempt = now;
          // Attempt to reconnect
          if (reconnect()) {
            lastReconnectAttempt = 0;
          }
        }
      } else {
        // Client connected
        client.loop();
        ID = 1;
        // Publish message here
        // Send data to mqtt broker 
        
        // Read soil sensor data
        SoilMoistureValue = analogRead(A0);
        MoisturePercent = SoilMoistureValue/DryValue;
        MP = String(MoisturePercent);

        // Read GPS data
        dat = readGPS();
        ID = 1;
        
        // Convert to char array for PubSubClient functions
        char DAT[70];
        dat.toCharArray(DAT,60);

        // Publish node1 data to topic 
        client.publish(topic1, DAT);
        Serial.println(DAT);
        delay(10000);

        
        if (rf95.available()){
          uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
          uint8_t len = sizeof(buf);
          if (rf95.recv(buf, &len)){
          // Read data from node2
            if (buf[0] == 2 and GPSFlag == 2){
      GPSFlag = 3;
      ID = buf[0];
      
      int Moist = buf[1];
      
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

      int alt1 = buf[13]*256;
      int alt2 = buf[14];
      int alt = alt1+alt2;

      if(buf[12] == 1){
        alt = alt*-1;
      }
      
      int Sats = buf[15];
      int Year = buf[16];
          Year = Year + 2000;
      int Month = buf[17];
      int Day = buf[18];
      int Hour = buf[19];
      int Minute = buf[20];
      int Second = buf[21];

      
      String node2Data = readNode();
      char node2DataChar[70];
      node2Data.toCharArray(node2DataChar,60);
      //Serial.println(node2Data);

      Serial.println(node2Data);
      Serial.println(node2DataChar);
          // Publish data from node2
          client.publish(topic2,node2DataChar);
          delay(1000);
          }
               if (buf[0] == 3 and GPSFlag == 3){

      GPSFlag = 2;
      ID = buf[0];
      ID = 3;
      Moist = buf[1];
      
      int Lati1 = buf[3]*16777216; //First byte
      int Lati2 = buf[4]*65536; //Second byte
      int Lati3 = buf[5]*256; // third byte
      int Lati4 = buf[6]; // Forth byte
    
      double preLati = Lati1+Lati2+Lati3+Lati4;
      Lati = preLati/1000000;
      if(buf[2] == 1){
        Lati = Lati*-1;
      }

      
      int Long1 = buf[8]*16777216; //First byte
      int Long2 = buf[9]*65536; //Second byte
      int Long3 = buf[10]*256; // third byte
      int Long4 = buf[11]; // Forth byte

      double preLong = Long1+Long2+Long3+Long4;
      Long = preLong/1000000;
      if(buf[7] == 1){
        Long = Long*-1;
      }

      int alt1 = buf[13]*256;
      int alt2 = buf[14];
      Alt = alt1+alt2;

      if(buf[12] == 1){
        Alt = Alt*-1;
      }
      
      Sats = buf[15];
      Year = buf[16];
      Year = Year + 2000;
      Month = buf[17];
      Day = buf[18];
      Hour = buf[19];
      Minute = buf[20];
      Second = buf[21];

      
      String node3Data = readNode();
      char node3DataChar[70];
      node3Data.toCharArray(node3DataChar,60);
      Serial.println(node3Data);
      Serial.println(node3DataChar);    
          // Read data from node3
          
          // Publish data from node3
          client.publish(topic3,node3DataChar);
          delay(1000);
          
               }
           
          }
        }
          
      }
    }
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Do something with received message
  // Listen for request to collect data from nodes w/ python dashboard 
  // Trigger data collection at nodes 

}

boolean reconnect() {
  if (client.connect("node1")) { //entering strings as arguments
    // Once connected, publish an announcement...
    //client.publish(topic,"hello world");
    // ... and resubscribe
    //client.subscribe("inTopic");
  }
  return client.connected();
}

String readGPS(){
  nodeID = ID;
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

  // Build string for single line in .csv
  String line = nodeID + ", " + yr + ", " + mon + ", " + d + ", " + hr + ", " 
  + m + ", " + s + ", " + MP + ", " + lati + ", " + longi + ", " + alt + ", " + sats; 

  return line;
}

String readNode(){
  nodeID = String(ID);
  lati = String(Lati);
  longi = String(Long);
  yr = String(Year);
  mon = String(Month);
  d = String(Day);
  hr = String(Hour);
  m = String(Minute);
  s = String(Second);
  alt = String(alt);
  sats = String(Sats);

  // Build string for single line in .csv
  String line = nodeID + ", " + yr + ", " + mon + ", " + d + ", " + hr + ", " 
  + m + ", " + s + ", " + Moist + ", " + lati + ", " + longi + ", " + alt + ", " + sats; 
  return line;

}

void radioReset(){
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
}
