#if defined(ESP32)                                                            
  #include <WiFi.h>
  #include <esp_now.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  extern "C" {
    #include <espnow.h>
  }
#endif


#define RELAY_PIN 26

typedef struct message {
  uint8_t state;
  uint8_t mac[6];
}; //Define a estrutura de mensagens

message income_message;

void setup() {
  Serial.begin(115200);
  //Coloca o pino do relê como saída e coloca com sinal baixo
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  //pinMode(LED_BUILTIN, OUTPUT);


  //Reseta as configurações da WiFi
  WiFi.disconnect();
  //Coloca em modo AP
  WiFi.mode(WIFI_AP);

#if defined(ESP32)
  WiFi.setSleep(WIFI_PS_NONE); // Evitar que entre em modo de economia de energia, senão aumentaria a latência, desligando o rádio do wifi
#endif

  setupESPNow();
  WiFi.printDiag(Serial);
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
}

void setupESPNow() {
  //digitalWrite(LED_BUILTIN, HIGH);
  //Inicializa o ESPNOW
  if (esp_now_init() != 0) {
    Serial.println("EspNow init failed");
    ESP.restart();
  }

  //0=IDLE, 1=MASTER, 2=SLAVE and 3=MASTER+SLAVE
  //esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  //Registra a função que será executada quando novos
  //dados chegarem
  esp_now_register_recv_cb(receiveCallback); //É a função que trata o dado recebido pelo espnow, //responsável por atribui o método que será chamado após interrupção (do mcu)
}

//Função que será executada quando chegarem novos dados
void receiveCallback(const uint8_t *mac_addr, const uint8_t *data, int len) {

  // Exibe o mac que envioou o dado
  char macStr[18];
  Serial.print("Packet received from: ");
//  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
//           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//  Serial.println(macStr);
  prntmac(mac_addr);

  memcpy(&income_message, data, sizeof(income_message)); // copia dados recebidos para a estrutura de armazenamento.
  Serial.print("Destination: ");
  prntmac(income_message.mac);
  Serial.println();
  set_output(income_message.state);
  
  
 
}

//Nada para fazer no loop já que as informações
//são recebidas pelo receiveCallback
//automaticamente quando novos dados chegam
void loop() {
  yield(); //Garante não travamento do código. Executa um bloco.
}

void set_output(int state){
  Serial.print("Estado: ");Serial.println(state);
  //digitalWrite(LED_BUILTIN, state == 0? HIGH : LOW);
  digitalWrite(RELAY_PIN, state == 0? HIGH : LOW);
  }
  
String mac_to_string(uint8_t * mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  return macStr;
}

//Print endereço mac
void prntmac(const uint8_t *mac_addr) {
  Serial.print("MAC Address: {0x");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mac_addr[i], HEX);
    if (i < 5)
      Serial.print(",0x");
  }
  Serial.println("};");
  }
