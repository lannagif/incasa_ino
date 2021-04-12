/*
     espnow master 
     https://www.esp32.com/viewtopic.php?f=19&t=12958
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define WIFI_CHANNEL 10
#define FIXED_CHANNEL    1

   //FC:F5:C4:19:A5:F9

uint8_t slaveDeviceMac[] = {0xFC, 0xF5, 0xC4, 0x19, 0xA5, 0xF9};  
const byte maxDataFrameSize = 200;
uint8_t dataToSend[maxDataFrameSize];
byte cnt = 0;
int dataSent = 0;

esp_now_peer_info_t slave;
const esp_now_peer_info_t *peer = &slave;

void WiFiReset() {
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}


void setup_espnow()
{
  Serial.print("\r\n\r\n");
  WiFiReset();
  
  Serial.print("Connecting to ");
  Serial.println( WiFi.softAPmacAddress() );

  Serial.println( WiFi.macAddress() );

//  Force espnow to use a specific channel
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  uint8_t primaryChan = FIXED_CHANNEL;
  wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  esp_wifi_set_channel(primaryChan, (wifi_second_chan_t)FIXED_CHANNEL);
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(false));

  WiFi.disconnect();

  Serial.print("Wifi Channel: "); Serial.println(WiFi.channel());

  if (esp_now_init() == ESP_OK)
  {
    Serial.println("ESPNow Init Success!");
  }
  else
  {
    Serial.println("ESPNow Init Failed....");
  }

  connect_to_ap();

  //Add the slave node to this master node
  memcpy( &slave.peer_addr, &slaveDeviceMac[0], 6 );
  slave.channel = FIXED_CHANNEL;
  slave.encrypt = 0;
  //slave.ifidx = ESP_IF_WIFI_STA;

  if ( esp_now_add_peer(peer) == ESP_OK)
  {
    Serial.print("Added Peer: ");
    prntmac(slave.peer_addr);
  }

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
}


void send_data(uint8_t *data)
{

    esp_err_t sendResult = esp_now_send(slave.peer_addr, data, sizeof(data)); // maxDataFrameSize);
    if (sendResult == ESP_OK) {
      Serial.print("Success delivering response");
    } else if (sendResult == ESP_ERR_ESPNOW_NOT_INIT) {
      // How did we get so far!!
      Serial.println("ESPNOW not Init.");
    } else if (sendResult == ESP_ERR_ESPNOW_ARG) {
      Serial.println("Invalid Argument");
    } else if (sendResult == ESP_ERR_ESPNOW_INTERNAL) {
      Serial.println("Internal Error");
    } else if (sendResult == ESP_ERR_ESPNOW_NO_MEM) {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (sendResult == ESP_ERR_ESPNOW_NOT_FOUND) {
      Serial.println("Peer not found.");
    }
    else if (sendResult == ESP_ERR_ESPNOW_IF) {
      Serial.println("Interface Error.");
    }   else {
      Serial.printf("\r\nNot sure what happened\t%d", sendResult);
    }
  

  delay(1000);
  yield();
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  if (strncmp((const char *)data, "Calling Master",14) == 0 ) {
    Serial.printf("\r\nReceived\t%d Bytes\t%s\n", data_len, data);
    strcpy((char*)dataToSend, "Greeting from Master");

    esp_err_t sendResult = esp_now_send(slave.peer_addr, dataToSend,sizeof(dataToSend)); // maxDataFrameSize);
    if (sendResult == ESP_OK) {
      Serial.print("Success delivering response");
    } else if (sendResult == ESP_ERR_ESPNOW_NOT_INIT) {
      // How did we get so far!!
      Serial.println("ESPNOW not Init.");
    } else if (sendResult == ESP_ERR_ESPNOW_ARG) {
      Serial.println("Invalid Argument");
    } else if (sendResult == ESP_ERR_ESPNOW_INTERNAL) {
      Serial.println("Internal Error");
    } else if (sendResult == ESP_ERR_ESPNOW_NO_MEM) {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (sendResult == ESP_ERR_ESPNOW_NOT_FOUND) {
      Serial.println("Peer not found.");
    }
    else if (sendResult == ESP_ERR_ESPNOW_IF) {
      Serial.println("Interface Error.");
    }   else {
      Serial.printf("\r\nNot sure what happened\t%d", sendResult);
    }
  } else {
    Serial.printf("\r\nReceived\t%d Bytes\t%d\n", data_len, data[0]);
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.print("Data sent to ");
  prntmac(mac_addr);
  dataSent = 1; //Sent executed
}
