// Funzione per ricevere i dati dal MEGA

void Receive_All_From_MEGA() {
  String NodeMCU_RX_Value = "";  // Variabile per ricevere i dati dal MEGA

  while (NodeMCU.available() > 0) {
    char recieved = NodeMCU.read();
    if (recieved != '\g' && recieved != '\c' && recieved != '\d' && recieved != '\z' && recieved != '\y' && recieved != '\o' && recieved != '\m') {
      NodeMCU_RX_Value += (char)recieved;
    } else if (recieved == '\g') {
    val_VoltNow = NodeMCU_RX_Value.toFloat();
    if (val_VoltNow >= 12.0 && val_VoltNow <= 33.6) {
    BatteryVoltage = val_VoltNow;
    Serial.print("Battery Voltage OK: ");
    Serial.println(BatteryVoltage);
    client.publish("home/robot/mower/status/battery", String(BatteryVoltage, 2).c_str(), true);
    } else {
    Serial.print("Battery Voltage fuori scala: ");
    Serial.println(val_VoltNow);
    client.publish("home/robot/mower/status/battery", "INVALIDO", true);  // Debug
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
  JsonDocument discovery;
  String mqtt_data = "";

  // BatteryVoltage
  discovery["name"] = "Battery";
  discovery["device_class"] = "voltage";
  discovery["unit_of_measurement"] = "V";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/battery").c_str();
  discovery["unique_id"] = "robotmowerbattery";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/battery/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();

  // Loop_Cycle_Mowing
  discovery["name"] = "loop";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/loop").c_str();
  discovery["unique_id"] = "mowerloop";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/loop/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();
  ;

  // Mower_Parked
  discovery["name"] = "parked";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/parked").c_str();
  discovery["unique_id"] = "mowerparked";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/parked/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();

  // Mower_Docked
  discovery["name"] = "docked";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/docked").c_str();
  discovery["unique_id"] = "mowerdocked";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/docked/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();

  // Mower_Running
  discovery["name"] = "running";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/running").c_str();
  discovery["unique_id"] = "mowerrunning";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/running/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();

  // Charge_Detected
  discovery["name"] = "charge";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/charge").c_str();
  discovery["unique_id"] = "mowercharge";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/charge/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();

  // Tracking_Wire
  discovery["name"] = "tracking";
  discovery["state_topic"] = (String(mqtt_topic_out) + "/tracking").c_str();
  discovery["unique_id"] = "mowertracking";
  discovery["device"]["name"] = "Mower";
  discovery["device"]["manufacturer"] = "Marco Bedendo";
  discovery["device"]["model"] = "Mower Sensor";
  discovery["device"]["identifiers"] = "BedendoMower";
  serializeJson(discovery, mqtt_data);
  client.beginPublish((String(mqtt_topic_discovery) + "/sensor/mower/tracking/config").c_str(), mqtt_data.length(), false);
  client.print(mqtt_data.c_str());
  client.endPublish();
  discovery.clear();

  // Loop Cycle Mowing
  client.publish("home/robot/mower/status/loop", String(Loop_Cycle_Mowing).c_str(), true);

  // Parked
  if (Mower_Parked == 1) {
    client.publish("home/robot/mower/status/parked", "PARCHEGGIATO", true);
  } else {
    client.publish("home/robot/mower/status/parked", "OFF", true);
  }

  // Docked
  if (Mower_Docked == 1) {
    client.publish("home/robot/mower/status/docked", "IN BASE", true);
  } else {
    client.publish("home/robot/mower/status/docked", "OFF", true);
  }

  // Running
  if (Mower_Running == 1) {
    client.publish("home/robot/mower/status/running", "FALCIATURA", true);
  } else {
    client.publish("home/robot/mower/status/running", "OFF", true);
  }

  // Charge
  if (Charge_Detected == 4) {
    client.publish("home/robot/mower/status/charge", "IN CARICA", true);
  } else {
    client.publish("home/robot/mower/status/charge", "OFF", true);
  }

  // Tracking wire
  if (Tracking_Wire == 1) {
    client.publish("home/robot/mower/status/tracking", "TRACCIATURA FILO", true);
  } else {
    client.publish("home/robot/mower/status/tracking", "OFF", true);
  }
}
