
#define DEBUG true

#define WIFI_AP false


//  Definição das Credenciais de Wifi
#define WIFI_SSID "CASA"
#define WIFI_PASSWORD "32261474"

WiFiServer server(80);

void initialize() {
  initilize_eeprom();


  system_setup = load_setup_variables();
  if (system_setup.initialized) {
    Serial.println("Initilized");
    Serial.println("Values:");
    Serial.print("\t Uid: "); Serial.println(system_setup.uid);
    Serial.print("\t PWD: "); Serial.println(system_setup.pwd);
    Serial.print("\t SSID: " ); Serial.println(system_setup.ssid);

    connect_to_ap();
  } else {
    initialize_ap();
  }
}

void connect_to_ap() {
  
//  Define o dispositivo como uma estação e ponto de acesso ativado por software simultâneamente
//  WiFi.mode(WIFI_AP_STA);
WiFi.mode(WIFI_MODE_APSTA);

  WiFi.begin("CENTRAL", "32261474");
  //WiFi.begin(system_setup.ssid, system_setup.pwd);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());

  Serial.println();
  WiFi.printDiag(Serial);
}

void initialize_ap()
{
  if (WIFI_AP) {
    // Conecta à rede Wi-Fi com SSID e senha
    Serial.println("Setting AP (Access Point)…");
    // Remove o parâmetro de senha, se for desejável o AP (Ponto de Acesso) seja aberto
    WiFi.softAP(ssid, "");

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
  } else {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  }

  server.begin();
  wait_for_setup();
}

void load_config() {

}

String urldecode(String str)
{
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {

      encodedString += c;
    }

    yield();
  }

  return encodedString;
}

String urlencode(String str)
{
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
      //encodedString+=code2;
    }
    yield();
  }
  return encodedString;

}

unsigned char h2int(char c)
{
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

String fetch_query_value(String header, String locator) {
  int cursor = 0;
  cursor = header.indexOf(locator + "=", cursor);

  if (cursor > 0) {
    int begin = header.indexOf("=", cursor) + 1;
    int end = header.indexOf("&", begin);
    end = (end > 0) ? end : header.indexOf(" HTTP/", begin);
    if (begin != -1) {
      String out = {header.substring(begin, end)};
      out = urldecode(out);
      Serial.print(locator);
      Serial.print("=>'");
      Serial.print(out);
      Serial.println("'");


      return out;
    }
  } else {
    return "";
  }
}

void restart_system() {
  Serial.println("Restarting in 5 seconds");
  delay(5000);
  ESP.restart();
}

void wait_for_setup()
{
  while (true)
  {

    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             // if you get a client,
      Serial.println("new client");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
          Setup s;
          if (currentLine.endsWith(" HTTP/")) {
            if (currentLine.startsWith("GET /setup?") > 0) {
              fetch_query_value(currentLine, "ssid").toCharArray(system_setup.ssid, 100);
              fetch_query_value(currentLine, "uid").toCharArray(system_setup.uid, 100);
              fetch_query_value(currentLine, "pwd").toCharArray(system_setup.pwd, 100);
              store_setup_variables();

              currentLine = "";
              restart_system();
            }
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("client disonnected");

    }
  }
}
