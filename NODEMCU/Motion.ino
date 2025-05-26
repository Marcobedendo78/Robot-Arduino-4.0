// Invia i vari codici WIFI al MEGA


void Pause_Mower() {
  Serial.println("");
  Serial.println("Pause/Stop");
  transmit_blynk_code = 1;
  Transmit_Blynk_Data_to_Mega();
  }

void Going_Home() {
  Serial.println("");
  Serial.println("Go to Dock");
  transmit_blynk_code = 2;
  Transmit_Blynk_Data_to_Mega();
  }

void StartMower() {
  Serial.println("");
  Serial.println("Quick Start");
  Mower_Parked = 0;
  Update_Blynk_App_With_Status();
  transmit_blynk_code = 3;                    // Codice utilizzato per far sapere a MEGA cosa fare
  Transmit_Blynk_Data_to_Mega();              // Trasmette quel codice al MEGA
  }

void Exit_Dock() {
  Serial.println("");
  Serial.println("Exit Dock");
  Mower_Docked = 0;                           // Forza l'APP a mostrare il dock come falso.
                                              // Altrimenti l'APP dovrà attendere il prossimo ciclo di aggiornamento
  Update_Blynk_App_With_Status();
  transmit_blynk_code = 4;
  Transmit_Blynk_Data_to_Mega();
  }


void Set_To_Manuel_Mode() {
    Serial.println("");
    Serial.println("Manuel Mode Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 5;
    Transmit_Blynk_Data_to_Mega();
    Manuel_Mode = 1;
}

void Set_To_Automatic_Mode_Random() {
    Serial.println("");
    Serial.println("Automatic Mode RANDOM Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 6;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 1;
}

void Set_To_Automatic_Mode_Spiral() {
    Serial.println("");
    Serial.println("Automatic Mode SPIRAL Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 11;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 2;
}

void Set_To_Automatic_Mode_Parallel() {
    Serial.println("");
    Serial.println("Automatic Mode PARALLEL Selected");
    Update_Blynk_App_With_Status();
    transmit_blynk_code = 12;
    Transmit_Blynk_Data_to_Mega();
    Automatic_Mode = 2;
}
