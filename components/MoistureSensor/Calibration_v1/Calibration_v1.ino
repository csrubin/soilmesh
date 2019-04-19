//taken from https://www.dfrobot.com/wiki/index.php/Capacitive_Soil_Moisture_Sensor_SKU:SEN0193


void setup() 
{
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps
}

void loop() 
{
  int val; 
  val = analogRead(A1); //connect sensor to Analog 0
  Serial.println(val); //print the value to serial port
  delay(2000);
}
//max(1) = 2900 
//min(1) = 1300 

//max(2) = 2647
//min(2) = 1200

//max(3) = 3100
//min(3) = 1400
