

#include <esp_now.h>
#include <esp_wifi.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>                    

#include <ArduinoJson.h>


// Variable to store the HTTP request
String header;





void setup()
{
  Serial.begin(115200);
  //  load_configuration;

  initialize();
  //  initialize_ap();

  setup_espnow();

  setup_firebase();
 
}

void loop()
{
  
//  loop_espnow();
  firebase_loop();
//  yeld();
}


