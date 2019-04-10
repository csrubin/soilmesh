// To be used only on node1
// More description here

// Libaries to be included
#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

// WiFi connection parameters
const char* ssid = "Connor Rubin iPhone";
const char* password = "csr215259"; 

// MQTT parameters
const char* mqtt_server = "broker.hivemq.com"; //"broker.mqtt-dashboard.com";
long lasMsg = 0;
long lastReconnectAttempt = 0;

// Global variables
char topic[] = "node_receive";

// Unsure if needed
char msg[50];
int value = 0;

// Object initialization
WiFiClient espClient;
PubSubClient client(espClient);

int led = A0;

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

  // MQTT reconnect variable
  lastReconnectAttempt = 0;

  Serial.println("Testing LED...");
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  Serial.println("Setup complete.");
}

void loop() {      
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
    client.subscribe(topic);
    if (client.subscribe(topic)){
      Serial.println("Subscribed.");
    }
    else{
      Serial.println("Subscription failed.");
    }
  }

  delay(100);
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
  char trigger[length];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //strcat(trigger, (char)payload[i]);
    
  }
  Serial.println();

  // Do something with received message
  // Listen for request to collect data from nodes w/ python dashboard 
  // Trigger data collection at nodes '

  if (trigger == "ON"){
    digitalWrite(led, HIGH);
  }
  else if (trigger == "OFF"){
    digitalWrite(led, LOW);
  }
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

