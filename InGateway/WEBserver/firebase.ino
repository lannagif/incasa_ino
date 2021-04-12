#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#define FIREBASE_HOST "incasa-iot.firebaseio.com"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "incasa-iot"

#define FETCH_TIME 1000 // Tempo de coleta de documentos.
/** 3. Define the API key

   The API key can be obtained since you created the project and set up
   the Authentication in Firebase conssole.

   You may need to enable the Identity provider at https://console.cloud.google.com/customer-identity/providers
   Select your project, click at ENABLE IDENTITY PLATFORM button.
   The API key also available by click at the link APPLICATION SETUP DETAILS.

*/
#define API_KEY "Chave da API"

/** 3. Define the Service Account credentials (required for token generation)

   This information can be taken from the service account JSON file.

   To download service account file, from the Firebase console, goto project settings,
   select "Service accounts" tab and click at "Generate new private key" button
*/
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-k4fpj@incasa-iot.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "------BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDaR3tF/S8L53Ug\n6o53a/pNK+0JrjjfsBBMmKjI/qcP6U+366oZy0cj7WOp9/HFZb3sFFygcEwPqsPF\nqWQE9XjYdilxmuVLLZf79RqlsH3Nv2yqaw5WzxupRnmPEodk/G4unlCWKcamls3Y\nw+og+8e9almg3cUswinoGzttT/xiJqu1t3ab2zGdS6ME+6K3OW1bf4bq2kFtuD3N\n9zUXZTyoG2rZ/+ptdq2qARVS1Hj+IxeNQnpovsGNF18bUF0gjvuIqOeYk0E0de7t\nlXqKSt8nn6oEHFSQG+elZxH2vPPtQMuNqwdSDwgJl8/hlHmDllZT6Htew6C9YgNy\nkQRXVITFAgMBAAECggEAAJCFw5kTiZLYLTUeGDX5yEBQVSGTuRRh/mi4qwhpkzN1\nxtJ/tqBddzzScKqY//sy4gztbmbL85BsADNfdFnULa8kjUwqwqaECZ1JK9uIf8M6\nezhaqaPQfQAXKWURX5P9jvZO3AyQnHawYpqjHOG50wYW5EEelO6NoDGN1ksKKpDQ\n0D9ODLm2zUr5z1Wm/5LYdrpLJA/Vw0Hu+7bk4mybF7qteJ+8UC6+k8iDFc3PbgeC\nJ9j2jjvCphdgewm8SkeWFCKwkGyZ3reQoydePRyIVmamBf7CtlKCZ3m6odWnM6BK\nuG2d531xOyfW1825fbrxg7W/VpmxBxhb3Tnhd7qCQQKBgQD7dJnMEpa4RF1/VgkK\nAYCLJG8LQqyR/ZxghutAjMdLG9q7Y/y/RY07tNQFSq0szVM9PwEkDUep8RU9i11G\nTy/11N9lWRk5bXLDL2PrTg41ksa4CiWZe8OhqMFYmqpudqCgS0nbyMWv0XOzXBhi\naSN7wWUgl7xKORxyaDTXX28RYQKBgQDeOWKxSW96F5EZZcECF1+30LqX0P5gAY8K\nXSG0oIgErRJRXYjtWy0Eatiu5NGceuk/twscqwkEMwCDG1QCZ9XL+ZynFC7TMNVT\nb6YEUPjGYMMo4uFF53SgoRJOB9GFy/f/4fbx+4w4WsW33n3pG017MLOctNwZq7Yl\nrdxCs/eZ5QKBgBv+XyDY8O6SrdX/PgJfKHBVHgei5JNcKIyWeJo3EfjKoQ87kme4\n1oBPIUt1hNH4CcxHt1jMIa5JqJOxcDDjn/M+P6CIysYZ6SzRmc4YYnyUWosilg0A\nOkdFAkFa9p5VloxB1XciYt+v8bGGDzDlg43Def6m5c9djE0wTLGtbjABAoGBAIXR\nlr3d0/R4xpa9sKl6u9gsKIENiHD131ZFWOu1k9MFCbfXlIIiWoyDbvJ6tkL9vQHy\nTw23bRmLQO6ygTdlodhP+uad/8iLaprw689Dl1uWn086uJ9oztF/zU8R5Dkm57O+\nNf/oE7tH5jJgGABMwa7CWBawzq3iujAcbzv3tc2FAoGBANCLQCKyvzyY0WE3BeE8\nfrsATHy0alcaJEb8z9d/rSO+YNRLzypUaxOifurWZ+diXzEXyFHR8WVZk5fX1lHR\nhuD9Rv2l07zT8azR0alsxALBzKB4AaON8OeklpoghV3o4tUSGlBv8gjwoDpClgop\n7C8TLEVwIEnpFQomSyprw0/E\n-----END PRIVATE KEY-----\n";
//
///* This is Google root CA certificate */
///*
const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n"
                                  "A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n"
                                  "Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n"
                                  "MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n"
                                  "A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n"
                                  "hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n"
                                  "v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n"
                                  "eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n"
                                  "tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n"
                                  "C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n"
                                  "zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n"
                                  "mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n"
                                  "V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n"
                                  "bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n"
                                  "3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n"
                                  "J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n"
                                  "291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n"
                                  "ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n"
                                  "AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n"
                                  "TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n"
                                  "-----END CERTIFICATE-----\n";


/** 4. Define the database secret (optional)

   This database secret needed only for this example to modify the database rules

   If you edit the database rules yourself, this is not required.
*/
//#define FIREBASE_AUTH "UhU93XiVLmOBwErzTDhHMx2yWfQlAH2XErTMZm2s"

/* 5. Define the Firebase Data object */
FirebaseData fbdo;

/* 6. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 7. Define the FirebaseConfig data for config data */
FirebaseConfig config;

/* The helper function to modify the database rules (optional) */
void prepareDatabaseRules(const char *path, const char *var, const char *readVal, const char *writeVal);

/* The function to print the operating results */
void printResult(FirebaseData &data);

/* The calback function to print the token generation status */
void tokenStatusCallback(TokenInfo info);

/* The helper function to get the token status string */
String getTokenStatus(struct token_info_t info);

/* The helper function to get the token type string */
String getTokenType(struct token_info_t info);

/* The helper function to get the token error string */
String getTokenError(struct token_info_t info);

String path = "";
unsigned long dataMillis = 0;
int count = 0;

int state = 0;

FirebaseJson json1;
FirebaseJsonData jsonObj;


void setup_firebase() { /* Assign the project host and api key (required) */
  config.host = FIREBASE_HOST;
  config.api_key = API_KEY;

  /* Assign the sevice account credentials and private key (required) */
  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = PRIVATE_KEY;

  /** Assign the unique user ID (uid) (required)
     This uid will be compare to the auth.uid variable in the database rules.

     If the assigned uid (user UID) was not existed, the new user will be created.

     If the uid is empty or not assigned, the library will create the OAuth2.0 access token
     instead.

     With OAuth2.0 access token, the device will be signed in as admin which has
     the full ggrant access and no database rules and custom claims are applied.
     This similar to sign in using the database secret but no admin rights.
  */
  auth.token.uid = system_setup.uid;

  /** Assign the custom claims (optional)
     This uid will be compare to the auth.token.premium_account variable
     (for this case) in the database rules.
  */
  //    auth.token.claims.add("premium_account", true);
  //    auth.token.claims.add("admin", true);

  //    Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  /* path for user data is now "/UsersData/Node1" */
  String base_path = "/users/";

  /** Now modify the database rules (if not yet modified)

     The user, Node1 in this case will be granted to read and write
     at the curtain location i.e. "/UsersData/Node1" and we will also check the
     custom claims in the rules which must be matched.

     To modify the database rules in this exanple, we need the full access rights then
     using the database secret in prepareDatabaseRules function to sign in.

     If you database rules has been modified, plesse comment this code out.

     The character $ is to make a wildcard variable (can be any name) represents any node key
     which located at some level in the rule structure and use as reference variable
     in .read, .write and .validate rules

     For this case $userId represents any xxxx node that places under UsersData node i.e.
     /UsersData/xxxxx which xxxx is user UID or "Node1" in this case.

     Please check your the database rules to see the changes after run the below code.
  */
  //    String var = "$userId";
  //    String val = "($userId === auth.uid && auth.token.premium_account === true && auth.token.admin === true)";
  //    prepareDatabaseRules(base_path.c_str(), var.c_str(), val.c_str(), val.c_str());

  /** Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;

  /** Assign the maximum retry of token generation */
  config.max_token_generation_retry = 5;

  /* Now we start to signin using custom token */

  /** Initialize the library with the Firebase authen and config.

     The device time will be set by sending request to the NTP server
     befor token generation and exchanging.

     The signed RSA256 jwt token will be created and used for id token exchanging.

     Theses process may take time to complete.
  */
  Firebase.begin(&config, &auth);

  /**
     The custom token which created internally in this library will use
     to exchange with the id token returns from the server.

     The id token is the token which used to sign in as a user.

     The id token was already saved to the config data (FirebaseConfig data variable) that
     passed to the Firebase.begin function.

     The id token (C++ string) can be accessed from config.signer.tokens.id_token.
  */

  path = base_path + auth.token.uid.c_str() + "/dispositivos";

}

void firebase_loop() {
  if (millis() - dataMillis > FETCH_TIME)
  {
    dataMillis = millis();


    /* Get the token status */
    TokenInfo info = Firebase.authTokenInfo();
    if (info.status == token_status_ready)
    {

      Serial.println("------------------------------------");
      Serial.println("Get a document...");

      if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), ""))
      {
        Serial.println("PASSED");
        Serial.println("------------------------------------");
//        Serial.println(fbdo.payload());
//        Serial.println("------------------------------------");

        memcpy(send_message.mac, slave.peer_addr, 6);
        prntmac(send_message.mac);

        deserializeJson(doc, fbdo.payload());
        state = doc["documents"][0]["fields"]["estado"]["integerValue"].as<unsigned int>();
        send_message.state = state;

        send_data((uint8_t *) &send_message);

//        json1 = fbdo.jsonObject();
//        String jsonStr;
//        json1.toString(jsonStr, true);
//        Serial.println(jsonStr);
//
//
//        json1.get(jsonObj, "/documents[0]");
//        Serial.println("documents[0]/updateTime: " + jsonObj.stringValue);
//        Serial.println();


        
//        iterate_json(doc);


        json1 = fbdo.jsonObject();
        json1.get(jsonObj, "documents[0]");
        Serial.println("------------------------------------");
        Serial.print("Estado: ");
        //        char buffer[200];
        //
        //  serializeJsonPretty(doc, buffer);
        //
        //  Serial.println(buffer);
        int state = doc["documents"][0]["fields"]["estado"]["integerValue"].as<unsigned int>();
        Serial.println(state);
        Serial.println("------------------------------------");
        Serial.println();



      }
      else
      {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
      }
    }
  }
}



void tokenStatusCallback(TokenInfo info)
{
  /** fb_esp_auth_token_status enum
     token_status_uninitialized,
     token_status_on_initialize,
     token_status_on_signing,
     token_status_on_request,
     token_status_on_refresh,
     token_status_ready,
     token_status_error
  */
  if (info.status == token_status_error)
  {
    Serial.printf("Token info: type = %s, status = %s\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
    Serial.printf("Token error: %s\n", getTokenError(info).c_str());
  }
  else
  {
    Serial.printf("Token info: type = %s, status = %s\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
  }
}

void printResult(FirebaseData &data)
{

  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData(), 5);
  else if (data.dataType() == "double")
    printf("%.9lf\n", data.doubleData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData() == 1 ? "true" : "false");
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
  {
    Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println();
    Serial.println("Iterate JSON data:");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      Serial.print(i);
      Serial.print(", ");
      Serial.print("Type: ");
      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        Serial.print(", Key: ");
        Serial.print(key);
      }
      Serial.print(", Value: ");
      Serial.println(value);
    }
    json.iteratorEnd();
  }
  else if (data.dataType() == "array")
  {
    Serial.println();
    //get array data from FirebaseData using FirebaseJsonArray object
    FirebaseJsonArray &arr = data.jsonArray();
    //Print all array values
    Serial.println("Pretty printed Array:");
    String arrStr;
    arr.toString(arrStr, true);
    Serial.println(arrStr);
    Serial.println();
    Serial.println("Iterate array values:");
    Serial.println();
    for (size_t i = 0; i < arr.size(); i++)
    {
      Serial.print(i);
      Serial.print(", Value: ");

      FirebaseJsonData &jsonData = data.jsonData();
      //Get the result data from FirebaseJsonArray object
      arr.get(jsonData, i);
      if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
        Serial.println(jsonData.boolValue ? "true" : "false");
      else if (jsonData.typeNum == FirebaseJson::JSON_INT)
        Serial.println(jsonData.intValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
        Serial.println(jsonData.floatValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
        printf("%.9lf\n", jsonData.doubleValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
               jsonData.typeNum == FirebaseJson::JSON_NULL ||
               jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
               jsonData.typeNum == FirebaseJson::JSON_ARRAY)
        Serial.println(jsonData.stringValue);
    }
  }
  else if (data.dataType() == "blob")
  {

    Serial.println();

    for (size_t i = 0; i < data.blobData().size(); i++)
    {
      if (i > 0 && i % 16 == 0)
        Serial.println();

      if (i < 16)
        Serial.print("0");

      Serial.print(data.blobData()[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  else if (data.dataType() == "file")
  {

    Serial.println();

    File file = data.fileStream();
    int i = 0;

    while (file.available())
    {
      if (i > 0 && i % 16 == 0)
        Serial.println();

      int v = file.read();

      if (v < 16)
        Serial.print("0");

      Serial.print(v, HEX);
      Serial.print(" ");
      i++;
    }
    Serial.println();
    file.close();
  }
  else
  {
    Serial.println(data.payload());
  }
}

/* The helper function to get the token type string */
String getTokenType(struct token_info_t info)
{
  switch (info.type)
  {
    case token_type_undefined:
      return "undefined";

    case token_type_legacy_token:
      return "legacy token";

    case token_type_id_token:
      return "id token";

    case token_type_custom_token:
      return "custom token";

    case token_type_oauth2_access_token:
      return "OAuth2.0 access token";

    default:
      break;
  }
  return "undefined";
}

/* The helper function to get the token status string */
String getTokenStatus(struct token_info_t info)
{
  switch (info.status)
  {
    case token_status_uninitialized:
      return "uninitialized";

    case token_status_on_signing:
      return "on signing";

    case token_status_on_request:
      return "on request";

    case token_status_on_refresh:
      return "on refreshing";

    case token_status_ready:
      return "ready";

    case token_status_error:
      return "error";

    default:
      break;
  }
  return "uninitialized";
}

/* The helper function to get the token error string */
String getTokenError(struct token_info_t info)
{
  String s = "code: ";
  s += String(info.error.code);
  s += ", message: ";
  s += info.error.message.c_str();
  return s;
}

void iterate_json(DynamicJsonDocument root) {

//  JsonArray documents = doc["documents"].as<JsonArray>();
//  for (JsonObject document : documents) {
//    Serial.print("State: ");Serial.println(&doc["documents"][0]["fields"]["estado"]["integerValue"]);
//    Serial.print("Mac: ");Serial.println(&doc["documents"][0]["fields"]["mac"]["stringValue"]);
//  }
    
  }
