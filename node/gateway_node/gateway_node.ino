// Combined mqtt_reconnect_nonblocking and mqtt_esp8266 examples
// To be used only on node1

#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

// WiFi connection parameters
const char* ssid = "Connor Rubin iPhone"; //"201B"; //"P";
const char* password = "csr215259"; //"wemoved10beds"; //"gerbillhamstar";

// Miscellaneous parameters
const char* mqtt_server = "broker.hivemq.com"; //"broker.mqtt-dashboard.com";

// Global variables
static const uint32_t GPSBaud = 9600;
long lasMsg = 0;
long lastReconnectAttempt = 0;
char msg[50];
int value = 0;
String nodeID = String(EEPROM.read(0));
char topic[] = "capstone_test";
String dat, lati, longi, mon, d, yr, hr, m, s, alt, sats;

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

void setup() {
  Serial.begin(9600);
  Serial.print(nodeID);
  
  Serial.println("Initializing WiFi...");
  setup_wifi();

  Serial.println("Initializing MQTT protocol...");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("Initializing hardware serial port for GPS...");
  hs.begin(GPSBaud, SERIAL_8N1, 16, 17); 

  lastReconnectAttempt = 0;
}

void loop() {
  while (hs.available() > 0){

    // Read soil sensor data
    SoilMoistureValue = analogRead(A0);
    MoisturePercent = SoilMoistureValue/DryValue;
    String MP = String(MoisturePercent);
    
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
        // Listen for topic w/ Python script and write data to .csv
        
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
        client.publish(topic, DAT);
        Serial.println(DAT);
        delay(500);
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


void displayInfo(){
  // Location
  Serial.print(F("Location: ")); 
  if (gps.location.isValid()){
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } 
  else{
    Serial.print(F("INVALID"));
  }

  // Date/Time
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()){
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else{
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()){
    if (gps.time.hour() < 10) Serial.print(F("0"));{
      Serial.print(gps.time.hour());
      Serial.print(F(":"));
    }
    if (gps.time.minute() < 10) Serial.print(F("0"));{
      Serial.print(gps.time.minute());
      Serial.print(F(":"));
    }
    if (gps.time.second() < 10) Serial.print(F("0"));{
      Serial.print(gps.time.second());
      Serial.print(F("."));
    }
    if (gps.time.centisecond() < 10) Serial.print(F("0"));{
      Serial.print(gps.time.centisecond());
    }
  }
  else{
    Serial.print(F("INVALID"));
  }
  
  // Altitude
  Serial.print(F("  Altitude: "));
  if (gps.altitude.isValid()){
    Serial.print(gps.altitude.meters());
    Serial.print(F("m"));
  }
  else{
    Serial.print(F("INVALID"));
  }

  // Number of Satellites
  Serial.print(F("  # Sats: "));
  if (gps.satellites.isValid()){
    Serial.print(gps.satellites.value());
  }
  else{
    Serial.print(F("INVALID"));
  }

  // Newline + timing delay --> change for nonblocking? 
  Serial.println();
  delay(1000);
}
