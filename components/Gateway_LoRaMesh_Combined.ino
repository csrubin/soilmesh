
#include <EEPROM.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define RH_HAVE_SERIAL
#define LED 9
#define N_NODES 3

// WiFi connection information
const char* ssid     = "Connor Rubin's iPhone";
const char* password = "csr215259"; //put in credentials.h file

// MQTT connection information
const char* mqtt_server = "your-mqtt-server";
int mqtt_port = 8883;
const char* mqtt_username = "your-mqtt-username";
const char* mqtt_password = "your-mqtt-password";
const char* dataTopic = "mesh_gateway/data";

// PubSub initialization (MQTT)
WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);

// Mesh information
uint8_t nodeID;
uint8_t routes[N_NODES]; // full routing table for mesh
int16_t rssi[N_NODES]; // signal strength info

// Radio driver instance
RH_RF95 rf95;

// Class to manage message delivery and receipt, using the driver
RHMesh *manager;

// Message buffer
char buf[RH_MESH_MAX_MESSAGE_LEN];

// ??
char data[128];

void setup() {
  // Start serial connection, wait for it to successfully begin
  Serial.begin(115200);
  while (!Serial);

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection to successfully begin
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  
  // Print connection confirmation and IP address
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT server
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt.connect();

  // Initialize node (Mesh boilerplate)
  nodeID = EEPROM.read(0);
  Serial.print(F("initializing node "));
  manager = new RHMesh(rf95, nodeID);

  if (!manager->init()) {
    Serial.println(F("init failed"));
  } else {
    Serial.println("done");
  }
  rf95.setTxPower(23, false);
  rf95.setFrequency(915.0);
  rf95.setCADTimeout(500);

  // long range configuration requires for on-air time
  boolean longRange = false;
  if (longRange) {
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
    };
    rf95.setModemRegisters(&modem_config);
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096)) {
      Serial.println(F("set config failed"));
    }
  }

  Serial.println("RF95 ready");

  // Routing info
  for(uint8_t n=1;n<=N_NODES;n++) {
    routes[n-1] = 0;
    rssi[n-1] = 0;
  }

  // Free memory ???
  Serial.print(F("mem = "));
  Serial.println(freeMem());
  
}

void loop() {
  // put your main code here, to run repeatedly:

}


/* User-defined functions*/

// Connect to MQTT broker
void mqtt_connect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    log("Connecting to MQTT...");
    // Attempt to connect
    String mqtt_clientId = "mesh_gateway-";
    mqtt_clientId += String(random(0xffff), HEX);
    if (mqtt_client.connect(mqtt_clientId.c_str(), mqtt_username, mqtt_password)) {
      log("connected");
    } else {
      log("failed, rc=", false);
      log(mqtt_client.state());
      delay(2000);
    }
  }
}

int freeMem() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

const __FlashStringHelper* getErrorString(uint8_t error) {
  switch(error) {
    case 1: return F("invalid length");
    break;
    case 2: return F("no route");
    break;
    case 3: return F("timeout");
    break;
    case 4: return F("no reply");
    break;
    case 5: return F("unable to deliver");
    break;
  }
  return F("unknown");
}

void updateRoutingTable() {
  for(uint8_t n=1;n<=N_NODES;n++) {
    RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
    if (n == nodeId) {
      routes[n-1] = 255; // self
    } else {
      routes[n-1] = route->next_hop;
      if (routes[n-1] == 0) {
        // if we have no route to the node, reset the received signal strength
        rssi[n-1] = 0;
      }
    }
  }
}

// Create a JSON string with the routing info to each node
void getRouteInfoString(char *p, size_t len) {
  p[0] = '\0';
  strcat(p, "[");
  for(uint8_t n=1;n<=N_NODES;n++) {
    strcat(p, "{\"n\":");
    sprintf(p+strlen(p), "%d", routes[n-1]);
    strcat(p, ",");
    strcat(p, "\"r\":");
    sprintf(p+strlen(p), "%d", rssi[n-1]);
    strcat(p, "}");
    if (n<N_NODES) {
      strcat(p, ",");
    }
  }
  strcat(p, "]");
}

void printNodeInfo(uint8_t node, char *s) {
  Serial.print(F("node: "));
  Serial.print(F("{"));
  Serial.print(F("\""));
  Serial.print(node);
  Serial.print(F("\""));
  Serial.print(F(": "));
  Serial.print(s);
  Serial.println(F("}"));
}

