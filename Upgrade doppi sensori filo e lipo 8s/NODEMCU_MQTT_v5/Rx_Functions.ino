// Funzione per ricevere i dati dal MEGA

void Receive_All_From_MEGA() {
  String NodeMCU_RX_Value = "";  // Variabile per ricevere i dati dal MEGA

  while (NodeMCU.available() > 0) {
    char recieved = NodeMCU.read();

    if (recieved != '\g' && recieved != '\c' && recieved != '\d' && recieved != '\z' && recieved != '\y' && recieved != '\o' && recieved != '\m') {
      NodeMCU_RX_Value += (char)recieved;

    } else if (recieved == '\g') {
      val_VoltNow = NodeMCU_RX_Value.toFloat();

      if (val_VoltNow >= 12.0f && val_VoltNow <= 33.6f) {
        BatteryVoltage = val_VoltNow;
        Serial.print("Battery Voltage OK: ");
        Serial.println(BatteryVoltage);

        // Pubblica SOLO numeri con punto decimale
        char buf[16];
        dtostrf(BatteryVoltage, 0, 2, buf);
        client.publish("home/robot/mower/status/battery", buf, true);

      } else {
        Serial.print("Battery Voltage fuori scala: ");
        Serial.println(val_VoltNow);

        // NON pubblicare testo sul topic numerico!
        client.publish("home/robot/mower/status/battery_status", "invalid", true);
      }
      NodeMCU_RX_Value = "";

    } else if (recieved == '\c') {
      Loop_Cycle_Mowing = NodeMCU_RX_Value.toInt();
      NodeMCU_RX_Value = "";

    } else if (recieved == '\d') {
      Mower_Docked = NodeMCU_RX_Value.toInt();
      NodeMCU_RX_Value = "";

    } else if (recieved == '\z') {
      Mower_Running = NodeMCU_RX_Value.toInt();
      NodeMCU_RX_Value = "";

    } else if (recieved == '\y') {
      Mower_Parked = NodeMCU_RX_Value.toInt();
      NodeMCU_RX_Value = "";

    } else if (recieved == '\o') {
      Charge_Detected = NodeMCU_RX_Value.toInt();
      NodeMCU_RX_Value = "";

    } else if (recieved == '\m') {
      Tracking_Wire = NodeMCU_RX_Value.toInt();
      NodeMCU_RX_Value = "";
    }
  }
}


void Update_Home_Assistant_With_Status() {
  // usa un documento statico con capienza adeguata
  StaticJsonDocument<512> discovery;
  String mqtt_data;
  mqtt_data.reserve(512);

  // ---------- BatteryVoltage (sensor) ----------
  String stateTopicBattery = String(mqtt_topic_out) + "/battery";
  discovery.clear();
  discovery["name"] = "Battery";
  discovery["device_class"] = "voltage";
  discovery["unit_of_measurement"] = "V";
  discovery["state_class"] = "measurement";
  discovery["value_template"] = "{{ value | float }}";
  discovery["suggested_display_precision"] = 2;
  discovery["state_topic"] = stateTopicBattery;
  discovery["unique_id"] = "robotmowerbattery";
  discovery["object_id"] = "robotmowerbattery";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/battery/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Loop_Cycle_Mowing (sensor numerico) ----------
  String stateTopicLoop = String(mqtt_topic_out) + "/loop";
  discovery.clear();
  discovery["name"] = "loop";
  discovery["state_topic"] = stateTopicLoop;
  discovery["unique_id"] = "mowerloop";
  discovery["object_id"] = "mowerloop";
  discovery["state_class"] = "measurement";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  // opzionale: assicurati sia int
  discovery["value_template"] = "{{ value | int }}";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/loop/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Mower_Parked (sensor testuale) ----------
  String stateTopicParked = String(mqtt_topic_out) + "/parked";
  discovery.clear();
  discovery["name"] = "parked";
  discovery["state_topic"] = stateTopicParked;
  discovery["unique_id"] = "mowerparked";
  discovery["object_id"] = "mowerparked";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/parked/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Mower_Docked (sensor testuale) ----------
  String stateTopicDocked = String(mqtt_topic_out) + "/docked";
  discovery.clear();
  discovery["name"] = "docked";
  discovery["state_topic"] = stateTopicDocked;
  discovery["unique_id"] = "mowerdocked";
  discovery["object_id"] = "mowerdocked";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/docked/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Mower_Running (sensor testuale) ----------
  String stateTopicRunning = String(mqtt_topic_out) + "/running";
  discovery.clear();
  discovery["name"] = "running";
  discovery["state_topic"] = stateTopicRunning;
  discovery["unique_id"] = "mowerrunning";
  discovery["object_id"] = "mowerrunning";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/running/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Charge_Detected (sensor testuale) ----------
  String stateTopicCharge = String(mqtt_topic_out) + "/charge";
  discovery.clear();
  discovery["name"] = "charge";
  discovery["state_topic"] = stateTopicCharge;
  discovery["unique_id"] = "mowercharge";
  discovery["object_id"] = "mowercharge";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/charge/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Tracking_Wire (sensor testuale) ----------
  String stateTopicTracking = String(mqtt_topic_out) + "/tracking";
  discovery.clear();
  discovery["name"] = "tracking";
  discovery["state_topic"] = stateTopicTracking;
  discovery["unique_id"] = "mowertracking";
  discovery["object_id"] = "mowertracking";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  mqtt_data = "";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/tracking/config").c_str(),
                      mqtt_data.length(), /*retain=*/true);
  client.print(mqtt_data.c_str());
  client.endPublish();

  // ---------- Stati/valori (publish) ----------
  // Loop (numerico)
  client.publish(stateTopicLoop.c_str(), String(Loop_Cycle_Mowing).c_str(), true);

  // Parked
  if (Mower_Parked == 1) client.publish(stateTopicParked.c_str(), "PARCHEGGIATO", true);
  else                   client.publish(stateTopicParked.c_str(), "OFF", true);

  // Docked
  if (Mower_Docked == 1) client.publish(stateTopicDocked.c_str(), "IN BASE", true);
  else                   client.publish(stateTopicDocked.c_str(), "OFF", true);

  // Running
  if (Mower_Running == 1) client.publish(stateTopicRunning.c_str(), "FALCIATURA", true);
  else                    client.publish(stateTopicRunning.c_str(), "OFF", true);

  // Charge
  if (Charge_Detected == 4) client.publish(stateTopicCharge.c_str(), "IN CARICA", true);
  else                      client.publish(stateTopicCharge.c_str(), "OFF", true);

  // Tracking
  if (Tracking_Wire == 1) client.publish(stateTopicTracking.c_str(), "TRACCIATURA FILO", true);
  else                    client.publish(stateTopicTracking.c_str(), "OFF", true);
}
