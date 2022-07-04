#include <Arduino.h>
#include <arduino_homekit_server.h>;
#include <ESP8266WiFi.h>
#include <DHT.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
#define DHTPIN 2
#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

  
//access the config defined in C code
extern "C" homekit_server_config_t config; 
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_humidity;

const char* ssid = "FRITZ!Box 7590 JB";
const char* password = "93777898162534130520";

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  arduino_homekit_setup(&config);
}

void loop() {
  my_homekit_loop();
  delay(10);
}

//==============================
// Homekit setup and loop
//==============================

void my_homekit_setup() {
  // accessory_init();
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    // LOG_D("Free heap: %d, HomeKit clients: %d",
    //     ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}

void my_homekit_report() {
  /*
  float temperature_value = 0.1; 
  float cha_current_temperature = 0.1;
  float humidity_value = 0.1;
  float cha_humidity = 0.1;
  // LOG_D("Current temperature: %.1f", temperature_value);
  // LOG_D("Current Humidity: %.1f", humidity_value);
  homekit_characteristic_notify('0', 0);
  // homekit_characteristic_notify(1, 1);
  */
  
  float temperature_value = dht.readTemperature();  
  cha_current_temperature.value.float_value = temperature_value;
  float humidity_value = dht.readHumidity();
  cha_humidity.value.float_value = humidity_value;
  LOG_D("Current temperature: %.1f", temperature_value);
  LOG_D("Current Humidity: %.1f", humidity_value);
  homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
  
}

int random_value(int min, int max) {
  return min + random(max - min);
}
