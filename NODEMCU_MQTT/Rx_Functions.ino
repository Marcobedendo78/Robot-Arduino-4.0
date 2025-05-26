// Funzione per ricevere i dati dal MEGA

void Receive_All_From_MEGA() {
  String NodeMCU_RX_Value = "";  // Variabile per ricevere i dati dal MEGA
  
  while (NodeMCU.available() > 0) {
    char recieved = NodeMCU.read();
    if (recieved != '\g' && recieved != '\c' && recieved != '\d' && recieved != '\z' && recieved != '\y' && recieved != '\o' && recieved != '\m') {
      NodeMCU_RX_Value += (char)recieved;
    } else if (recieved == '\g') {
      val_VoltNow = NodeMCU_RX_Value.toFloat();
      BatteryVoltage = val_VoltNow;
      Serial.print("Battery Voltage: ");
      Serial.println(BatteryVoltage);
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
  client.publish((String(mqtt_topic_out) + "/battery").c_str(), String(BatteryVoltage).c_str());
  client.publish((String(mqtt_topic_out) + "/loop").c_str(), String(Loop_Cycle_Mowing).c_str());
  client.publish((String(mqtt_topic_out) + "/parked").c_str(), String(Mower_Parked).c_str());
  client.publish((String(mqtt_topic_out) + "/docked").c_str(), String(Mower_Docked).c_str());
  client.publish((String(mqtt_topic_out) + "/running").c_str(), String(Mower_Running).c_str());
  client.publish((String(mqtt_topic_out) + "/charge").c_str(), String(Charge_Detected).c_str());
  client.publish((String(mqtt_topic_out) + "/tracking").c_str(), String(Tracking_Wire).c_str());
}
