
#include <EEPROM.h>

// change this to be the ID of your node in the mesh network
uint8_t nodeId = 3;

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to be available

  Serial.println("setting nodeId...");
  EEPROM.begin(1);
  EEPROM.write(0, nodeId);
  EEPROM.commit();
  Serial.print(F("set nodeId = "));
  Serial.println(nodeId);

  uint8_t readVal = EEPROM.read(0);

  Serial.print(F("read nodeId: "));
  Serial.println(readVal);

  if (nodeId != readVal) {
    Serial.println(F("*** FAIL ***"));
  } else {
    Serial.println(F("SUCCESS"));
  }
}

void loop() {

}
