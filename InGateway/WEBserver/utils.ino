char* mac_to_char(uint8_t ar[] ) {
  char macAddr[18];
  sprintf(macAddr, "%2X:%2X:%2X:%2X:%2X:%2X", ar[0], ar[1], ar[2], ar[3], ar[4], ar[5]);
  Serial.println(macAddr);

  macAddr[0] = '\0';
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%2X", ar[i]);
    strcat(macAddr, buf);
    if (i < 5) strcat(macAddr, ":");
  }
  macAddr[17] = '\0';
  Serial.print("Converted mac: ");
  Serial.println(macAddr);
  return macAddr;
}

String mac2String(byte ar[])
{
  String s;
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%2X", ar[i]);
    s += buf;
    if (i < 5) s += ':';
  }
  return s;
}


// Print mac
void prntmac(const uint8_t *mac_addr) {
  Serial.print("MAC Address: {0x");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mac_addr[i], HEX);
    if (i < 5)
      Serial.print(",0x");
  }
  Serial.println("};");
}
