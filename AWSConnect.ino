void connectAWS() {
  //Begin WiFi in station mode
  WiFi.mode(WIFI_STA); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  //Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Configure wifi_client with the correct certificates and keys
  wifi_client.setCACert(AWS_CERT_CA);
  wifi_client.setCertificate(AWS_CERT_CRT);
  wifi_client.setPrivateKey(AWS_CERT_PRIVATE);

  //Connect to AWS IOT Broker. 8883 is the port used for MQTT
  mqtt_client.begin(AWS_IOT_ENDPOINT, 8883, wifi_client);

  //Set action to be taken on incoming messages
  mqtt_client.onMessage(incomingMessageHandler);

  Serial.print("Connecting to AWS IOT");

  //Wait for connection to AWS IoT
  while (!mqtt_client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if(!mqtt_client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  //Subscribe to a topic
  mqtt_client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishStatus() {
  //still not used
  //Create a JSON document of size 200 bytes, and populate it
  //See https://arduinojson.org/
  StaticJsonDocument<200> doc;
  doc["elapsed_time"] = millis() - t1;
  doc["value"] = random(1000);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to mqtt_client

  mqtt_client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println("Sent a message");
}

void incomingMessageHandler(String &topic, String &payload) {
  Serial.println("Message received!");
  Serial.println("Topic: " + topic);
  Serial.println("Payload: " + payload);
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();
  String cmd = obj["command"];
  processCommand(cmd);
}
