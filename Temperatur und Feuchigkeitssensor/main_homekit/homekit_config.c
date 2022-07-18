#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*
const char* random_name(){
  char letters[] = "ABCDEFGHIJKLMNOPQURSUVWXYZ";
  char random_string[] = "0123456789";
  srand(time(NULL));
  for(int i=0; i<10; i++){
    int r = rand() % 26 + 1;
    random_string[i] = letters[r];
  }
  return random_string;
}
*/

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

// char rName[] = "";
// char rName[10];
// rName = random_name();

homekit_characteristic_t cha_current_temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
homekit_characteristic_t cha_humidity  = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);
homekit_characteristic_t cha_battery  = HOMEKIT_CHARACTERISTIC_(BATTERY_LEVEL, 0);


homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Sensor 5"), // rName),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "123456789"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),

        HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            &cha_current_temperature,
            HOMEKIT_CHARACTERISTIC(NAME, "Temperature"),
            NULL
        }),

        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Humidity"),
            &cha_humidity,
            NULL
        }),
        
        HOMEKIT_SERVICE(BATTERY_SERVICE, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Battery"),
            &cha_battery,
            NULL
        }),
        
        NULL

    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111"
  };
