#include "EEPROM.h"

int addr = 0;
#define EEPROM_SIZE 300
#define EEPROM_ADDRESS_SIZE 100
#define EEPROM_MAX_ADDRESSES 3

char ssid[10] = "InGateway";

typedef struct Setup
{
  char ssid[100];
  char pwd[100];
  char uid[100];
  boolean initialized = false;
};

Setup system_setup;
typedef struct message {
  uint8_t state;
  uint8_t mac[6];
}; //Define a estrutura de mensagens

message send_message;

DynamicJsonDocument doc(1024);


struct Setup load_setup_variables() {
  Setup s;
  read_string(0).toCharArray(s.ssid, 100);
  read_string(1).toCharArray(s.pwd, 100);
  read_string(2).toCharArray(s.uid, 100);
  
  if (String(s.ssid).length() > 0 && String(s.uid).length() > 0){
    s.initialized = true;
  }
  
  return s;
}


void store_setup_variables() {
  write_string(system_setup.ssid, 0);
  write_string(system_setup.pwd, 1);
  write_string(system_setup.uid, 2);
}


void initilize_eeprom()
{
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to init EEPROM");
    delay(1000);
  }  
}

void erase_eeprom() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void write_string(char value[EEPROM_ADDRESS_SIZE], int addr)
{
  Serial.print("Writing to EEPROM value:'");
  Serial.print(value);
  Serial.print("' => address:");
  Serial.println(addr);
  for (int i = 0; i < EEPROM_ADDRESS_SIZE; i++)
  {
    EEPROM.write(addr*EEPROM_ADDRESS_SIZE +i, value[i]);
  }
  EEPROM.commit();  
}

String read_string(int addr) {
  // reading byte-by-byte from EEPROM
  String value;
  char buf[EEPROM_ADDRESS_SIZE];

  for (int i = 0; i < EEPROM_ADDRESS_SIZE; i++)
  {
    byte readValue = EEPROM.read(addr*EEPROM_ADDRESS_SIZE+i);
    buf[i] = char(readValue);
    if (readValue == 0)
    {

      break;

    }
  }
  value = String(buf);
  Serial.print("Read from EEPROM value:'");
  Serial.print(value);
  Serial.print("' => addr: "); 
  Serial.println(addr);
  return value;
}

void eeprom_set()
{
  Serial.println("starting now...");

  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to init EEPROM");
    delay(1000000);
  }

  // writing byte-by-byte to EEPROM
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(addr, ssid[i]);
    addr += 1;
  }
  EEPROM.commit();

  // reading byte-by-byte from EEPROM
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    byte readValue = EEPROM.read(i);

    if (readValue == 0)
    {
      break;
    }

    char readValueChar = char(readValue);
    Serial.print(readValueChar);
  }
}
