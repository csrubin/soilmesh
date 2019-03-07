#include <EEPROM.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#define RH_HAVE_SERIAL //????
#define N_NODES 3

uint8_t nodeID;
uint8_t routes[N_NODES]; // Routing table for mesh
uint8_t rssi[N_NODES];   //  Signal strength info

RH_RF95 driver;   // Radio driver instance
RHMesh *manager;  // Manager for driver usage

char buf[RH_MESH_MAX_MESSAGE_LEN]; // Message buffer

void setup() {
  Serial.begin(115200); // Initialize serial port
  while (!Serial);      // Wait for serial port 

  // Mesh node initialization
  nodeID =  EEPROM.read(0);
  Serial.print(F("Initializing node..."));
  manager = new RHMesh(driver, nodeID);

  // Error message if manager fails to initialize
  if (!manager->init()) {
    Serial.println(F("Initialization failed"));
  } else {
    Serial.println("Done.");
  }

  // Driver attribute settings
  driver.setTxPower(23, false);
  driver.setFrequency(915.0);
  driver.setCADTimeout(500);

  Serial.println("RF95 Ready.");

  // Initialize routes and signals to 0
  for(uint8_t n=1;n<=N_NODES;n++) {
    routes[n-1] = 0;
    rssi[n-1] = 0;
  }

  // Print free memory space
  /*Serial.print(F("mem = "));
  Serial.println(freeMem());
  */

}

void loop() {
  // Select self node
  /*for(uint8_t n=1;n<=N_NODES;n++) {
    if (n == nodeID) continue; // self
  }*/

  updateRoutingTable();
  getRouteInfoString(buf, RH_MESH_MAX_MESSAGE_LEN);

  uint8_t len = sizeof(nodeID);
  uint8_t gateway = 1;


  manager->sendtoWait((uint8_t *)nodeID, len, gateway);

}


/* User-defined functions */

/*int freeMem() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}*/

void updateRoutingTable() {
  for(uint8_t n=1;n<=N_NODES;n++) {
    RHRouter::RoutingTableEntry *route = manager->getRouteTo(n);
    if (n == nodeID) {
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
