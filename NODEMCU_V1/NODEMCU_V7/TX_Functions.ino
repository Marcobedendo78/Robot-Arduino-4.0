// Tutte le funzioni delle Informazioni trasmesse


void Transmit_Blynk_Data_to_Mega() {
    i = transmit_blynk_code + 10;           // Trasmette il codice e aggiunge 10 al numero
      NodeMCU.print(i);
      NodeMCU.println("\p");
      delay(30);
      Serial.print("TX_Code_to_MEGA:");
      Serial.print(i);
      Serial.print("");
      delay(20);
      }
