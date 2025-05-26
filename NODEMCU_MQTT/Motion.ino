// Funzione per ricevere comandi da Home Assistant

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Received message: ");
  Serial.println(message);
  Serial.print("From topic: ");
  Serial.println(topic);

    if (String(topic) == mqtt_topic_in) {
    if (message == "start") {
      Serial.println("Starting Mower");
      transmit_MQTT_code = 13;  // Comando per avviare il Robot
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "pause") {
      Serial.println("Pausing Mower");
      transmit_MQTT_code = 11;  // Comando per mettere in pausa il Robot
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "dock") {
      Serial.println("Going Home");
      transmit_MQTT_code = 12;   // Comando per andare il Robot alla base
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "Exit Dock") {
      Serial.println("Exit Dock");
      transmit_MQTT_code = 14;   // Comando per far uscire il Robot dalla base
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "manuale") {
      Serial.println("Switching to manual mode");
      transmit_MQTT_code = 15;  // Comando per modalità manuale
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "automatico") {
      Serial.println("Switching to automatic mode");
      transmit_MQTT_code = 16;  // Comando per modalità automatica
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "avanti") {
      Serial.println("Moving forward");
      transmit_MQTT_code = 17;  // Comando per muovere avanti
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "indietro") {
      Serial.println("Moving backward");
      transmit_MQTT_code = 18;  // Comando per muovere indietro
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "destra") {
      Serial.println("Turning right");
      transmit_MQTT_code = 20;  // Comando per girare a destra
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "sinistra") {
      Serial.println("Turning left");
      transmit_MQTT_code = 19;  // Comando per girare a sinistra
      Transmit_MQTT_Data_to_Mega();
    } else if (message == "fermo") {
      Serial.println("Stopping motors");
      transmit_MQTT_code = 11;  // Comando per fermare il robot (pausa)
      Transmit_MQTT_Data_to_Mega();
      
    }
  }
}
