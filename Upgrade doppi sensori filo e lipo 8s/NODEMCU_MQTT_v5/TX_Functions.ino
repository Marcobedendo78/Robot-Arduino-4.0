// Funzione per trasmettere i dati al MEGA con aggiunta del codice


void Transmit_MQTT_Data_to_Mega() {
    i = transmit_MQTT_code;   // trasmissione il codice
      NodeMCU.print(i);
      NodeMCU.println("\p");  // Aggiunge il carattere di fine
      delay(30);
      Serial.print("TX_Code_to_MEGA: ");
      Serial.print(i);
      Serial.print("");
      delay(20);
      }
