#include <Arduino.h>
#include <arduino_homekit_server.h>;
#include <ESP8266WiFi.h>




  
//access the config defined in C code
extern "C" homekit_server_config_t config; 
extern "C" homekit_characteristic_t cha_humidity;

const char* ssid = "FRITZ!Box 7590 JB";
const char* password = "93777898162534130520";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  arduino_homekit_setup(&config);
}
  
void loop() {
  arduino_homekit_loop();
}

