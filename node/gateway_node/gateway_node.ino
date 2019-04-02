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
  Serial.begin(9600);

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
    
        // Publish message here
        // Send data to mqtt broker 
        
        // Read soil sensor data
        SoilMoistureValue = analogRead(A0);
        MoisturePercent = SoilMoistureValue/DryValue;
        MP = String(MoisturePercent);

        // Read GPS data
        dat = readGPS();
        
        // Convert to char array for PubSubClient functions
        char DAT[70];
        dat.toCharArray(DAT,60);

        // Publish node1 data to topic 
        client.publish(topic1, DAT);
        Serial.println(DAT);
        delay(10000);

        /*
        if (rf95.available()){
          // Read data from node2
          
          // Publish data from node2
          client.publish(topic2,XXX);
          
          // Read data from node3
          
          // Publish data from node3
          client.publish(topic3,XXX);
          
        }*/
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

void radioReset(){
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
}

