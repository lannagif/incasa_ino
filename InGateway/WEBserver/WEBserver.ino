

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


/* ----------------------------------------------------------------
  1- setup
  1.0 - Verifica se está configurado, se não continua
  1.1 - Habilita AP com nome =>  InGatewayWifi
  1.2 - Habilita server http
  1.2 - Flutter envia dados de login ( POST/GET http://GATEWAY_AP/config?uid={UID}&ssid={SSID}&pw={PASS_WORD} ) [gateway_ap=192.168.125.1]
    - UID => não necessita passwd e usermail
    - wifiSsid
    - wifi_passwd
  1.3 - Marca rotina de configuração OK
  1.4 - Inicializa sistema

  2- Coleta dispositivos via Firebase

  3- Atualiza portas do ESP32 com o dado do Firebase
  --------------------*/
