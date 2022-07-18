#include <Arduino.h>
#include <arduino_homekit_server.h>;
#include <ESP8266WiFi.h>
#include <MAX17043.h>
#include <DHT.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
DHT dht(D4, DHT22);


//access the config defined in C code
extern "C" homekit_server_config_t config; 
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_humidity;
extern "C" homekit_characteristic_t cha_battery;


const char* ssid = "SSID";
const char* password = "password";

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;



void setup() {
  Serial.begin(9600);
  
  
  pinMode(D8, INPUT);
  pinMode(A0, INPUT);
  // Wire.begin(4, VOLTAGE_PIN);
  // FuelGauge.begin();
  dht.begin();
  
  wifi_connect();
  my_homekit_setup();
}

void loop() {
  if (digitalRead(D8) == HIGH){homekit_storage_reset();delay(3000);ESP.restart();}
  
  my_homekit_loop();
  
  if (!WiFi.isConnected()){
    ESP.restart();
  }
  delay(10);
}

//==============================
// Button for parring

//==============================

//==============================
// Homekit setup and loop
//==============================

void my_homekit_setup() {
  // homekit_storage_reset();
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
    LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}

void my_homekit_report() {
  int raw = 0;
  float volt = 0.0;
  uint8_t battery_value = 0;
  
  // Temperature
  float temperature_value = dht.readTemperature();
  if (isnan(temperature_value)) {
    cha_current_temperature.value.float_value = 0;
  }else {
    cha_current_temperature.value.float_value = temperature_value;
  }  
  // Humidity
  float humidity_value = dht.readHumidity();
  if (isnan(humidity_value)) {
    cha_humidity.value.float_value = 0;
  }else {
    cha_humidity.value.float_value = humidity_value;
  }
  
  // Battery Level
  raw = analogRead(A0);
  volt=raw/1023.0;
  volt=volt*4.2;
  battery_value = raw;
  cha_battery.value.uint8_value = raw;
  
  // Logging
  LOG_D("Current temperature: %.1f", temperature_value);
  LOG_D("Current Humidity: %.1f", humidity_value);
  LOG_D("Current Battery Level: %u", (uint8_t)battery_value);
  LOG_D("Current Battery Voltage: %.1f", volt);

  // Send to homekit
  homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
  homekit_characteristic_notify(&cha_battery, cha_battery.value);
}

void wifi_connect() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  Serial.println("WiFi connecting...");
  while (!WiFi.isConnected()) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}
