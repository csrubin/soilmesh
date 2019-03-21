// Combined mqtt_reconnect_nonblocking and mqtt_esp8266 examples
// To be used only on node1

#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

// WiFi connection parameters
const char* ssid = "201B";
const char* password = "wemoved10beds";

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
String full, lati, longi, mon, d, yr, hr, m, s, alt, sats;

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
    if (gps.encode(hs.read())){
      displayInfo();
      
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
        alt = String(gps.altitude.value());
        sats = String(gps.satellites.value());

        //full = nodeID + ", " + yr + ", " + mon + ", " + d + ", " + hr + ", " 
        //+ m + ", " + s + ", " + lati + ", " + longi + ", " + alt + ", " + sats; 

        full = lati + ', ' + longi;
        char dat[sizeof(full)];
        full.toCharArray(dat,sizeof(full));
        
        
        client.publish(topic, dat);
    }
  }
  
    
 /* while (hs.available()>0){  // Ensure hardware serial port is available 
    if (gps.encode(hs.read())){   // Read data from GPS
      
  /*
      if (gps.location.isValid()){
        lati = char(gps.location.lat());
        longi = char(gps.location.lng());
      }
      else{
        lati = "INVALID";
        longi = "INVALID";
      }
    
      if (gps.date.isValid()){
        yr = char(gps.date.year());
        mon = char(gps.date.month());
        d = char(gps.date.day());
      }
      else{
        yr = "INVALID";
        mon = "INVALID";
        d = "INVALID";
      }

      if (gps.time.isValid()){
        hr = char(gps.time.hour());
        m = char(gps.time.minute());
        s = char(gps.time.second());
      }
      else{
        hr = "INVALID";
        m = "INVALID";
        s = "INVALID";
      }
    
      if (gps.altitude.isValid()){
        alt = char(gps.altitude.value());
      }
      else{
        alt = "INVALID";
      }
    
      if (gps.satellites.isValid()){
        sats = char(gps.satellites.value());
      }
      else{
        sats = "INVALID";
      }
    
      // Add moisture reading 

      // Create data string for one line of .csv
      dat = nodeID + ", " + yr + ", " + mon + ", " + d + ", " + hr + ", " 
        + m + ", " + s + ", " + lati + ", " + longi + ", " + alt + ", " + sats;

      */
      
    
    
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

/*char getData(){
  char dat[], lati[10], longi[10], mon[10], d[10], yr[10], hr[10], m[10], s[10], alt[10], sats[10];
  if (hs.available()){  // Ensure hardware serial port is available 
    if (gps.encode(hs.read())){   // Read data from GPS
      
  
      if (gps.location.isValid()){
        lati = char(gps.location.lat());
        longi = char(gps.location.lng());
      }
      else{
        lati = "INVALID";
        longi = "INVALID";
      }
    
      if (gps.date.isValid()){
        yr = char(gps.date.year());
        mon = char(gps.date.month());
        d = char(gps.date.day());
      }
      else{
        yr = "INVALID";
        mon = "INVALID";
        d = "INVALID";
      }

      if (gps.time.isValid()){
        hr = char(gps.time.hour());
        m = char(gps.time.minute());
        s = char(gps.time.second());
      }
      else{
        hr = "INVALID";
        m = "INVALID";
        s = "INVALID";
      }
    
      if (gps.altitude.isValid()){
        alt = char(gps.altitude.value());
      }
      else{
        alt = "INVALID";
      }
    
      if (gps.satellites.isValid()){
        sats = char(gps.satellites.value());
      }
      else{
        sats = "INVALID";
      }
    
      // Add moisture reading 

      // Create data string for one line of .csv
      dat = nodeID + ", " + yr + ", " + mon + ", " + d + ", " + hr + ", " 
        + m + ", " + s + ", " + lati + ", " + longi + ", " + alt + ", " + sats;
    
      return dat;
    }
  }
  else{
    dat = "GPS Serial unavailable";
    return dat;
  } 
}
*/

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
