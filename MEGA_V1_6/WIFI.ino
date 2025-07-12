// Funzione che riceve i comandi via seriale dal NodeMCU (ora usiamo MQTT tramite NodeMCU)
// Funzione che riceve i comandi via seriale dal NodeMCU (comunicazione via MQTT)
void Get_WIFI_Commands() {
  Receive_Data_From_NODEMCU();  // Continua a ricevere i comandi dal NodeMCU
  delay(5);
  Transmit_All_To_NODEMCU();    // Mantieni la trasmissione dei dati al NodeMCU
}

// Funzione che riceve i dati dal NodeMCU tramite seriale
void Receive_Data_From_NODEMCU() {
  while (Serial2.available() > 0) {
    val_WIFI = Serial2.parseInt();  // Legge l'intero valore inviato
    if (Serial2.read() == '\p') {    // Verifica se il comando è completo (carattere di terminazione '\p')
      delay(1000);
      Execute_MQTT_Command_To_Mower();  // Passa il comando alla funzione per eseguire l'azione
    }
  }
}

// La funzione per ricevere i comandi manuali dalla seriale
void Receive_WIFI_Manuel_Commands() {
  while (Serial2.available() > 0) {
    val_WIFI = Serial2.parseInt();  // Legge l'intero valore inviato
    if (Serial2.read() == '\p') {    // Controlla se il comando è completo (terminato da '\p')
      delay(5);
      Execute_Manuel_MQTT_Command_To_Mower(); // Esegui il comando manuale
    }
  }
}

void Transmit_All_To_NODEMCU() {
  delay(5);
  Serial2.print(Volts);                       // Invia il valore della tensione
  Serial2.println("\g");                      // Invia il carattere di fine messaggio per Volts
  delay(5);

  Serial2.print(Amps);                        // Invia il valore della corrente batteria totale (A)
  Serial2.println("\\a");                     // Invia il carattere di fine messaggio per corrente batteria totale
  delay(5);

  Serial2.print(WheelAmps);                   // Invia il valore della corrente (A)
  Serial2.println("\\w");                     // Invia il carattere di fine messaggio per corrente ruote
  delay(5);

  Serial2.print(Loop_Cycle_Mowing);           // Invia lo stato del ciclo del loop
  Serial2.println("\c");                      // Invia il carattere di fine messaggio per Loop_Cycle_Mowing
  delay(5);

  Serial2.print(Mower_Docked);                // Invia lo stato se in base
  Serial2.println("\d");                      // Invia il carattere di fine messaggio per Mower_Docked
  delay(5);

  Serial2.print(Mower_Running);               // Invia se il tosaerba è in funzione
  Serial2.println("\z");                      // Invia il carattere di fine messaggio per Mower_Running
  delay(5);

  Serial2.print(Mower_Parked);                // Invia se il tosaerba è parcheggiato
  Serial2.println("\y");                      // Invia il carattere di fine messaggio per Mower_Parked
  delay(5);

  Serial2.println(Charging);                  // Invia lo stato di carica
  Serial2.println("\o");                      // Invia il carattere di fine messaggio per la ricarica
  delay(5);

  Serial2.println(Tracking_Wire);             // Invia lo stato del tracciamento
  Serial2.println("\m");                      // Invia il carattere di fine messaggio per Tracking_Wire
  delay(5);
}

// Funzione per eseguire il comando ricevuto tramite MQTT (da NodeMCU che lo riceve da Home Assistant)
void Execute_MQTT_Command_To_Mower() {
  delay(30);

  // Esegui il comando "Exit Dock" per Zona 1 (comando 14)
  if (val_WIFI == 14) {
    if (Mower_Docked == 1) {   
      Serial.println("");
      Serial.print(F("WIFI Command: ")); 
      Serial.print(val_WIFI);
      Serial.print(F("Exit Dock| "));
      Serial.println(F("Zone 1"));
      lcd.clear();
      lcd.print(F("WIFI Start"));
      lcd.setCursor(0,1);
      lcd.print(F("Uscita base Z1"));
      delay(500);
      lcd.clear();
      Blade_Override = 1;
      Exit_Zone = 1;
      Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
      Manouver_Exit_To_Zone_X();
    }
  }

  // Esegui il comando "Quick Start" (comando 13)
  if (val_WIFI == 13) {
    Serial.println("");
    Serial.print(F("WIFI Command:")); 
    Serial.print(val_WIFI);
    Serial.println(F("|Quick Start"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("WIFI Start"));
    if (Mower_Docked == 0) { 
      Manouver_Start_Mower();
      lcd.clear();    
    } else {
      Serial.println(F("Mower Docked - Quick Start not possible"));
    }
  }

  // Esegui il comando "Go To Dock" (comando 12)
  if (val_WIFI == 12) {
    Serial.println("");
    Serial.print(F("WIFI Command: ")); 
    Serial.print(val_WIFI);
    Serial.println(F("|Go To Dock"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("WIFI Vai a Casa"));
    Menu_Mode_Selection = 0;
    Motor_Action_Stop_Spin_Blades();
    Motor_Action_Stop_Motors();
    delay(1000);
    lcd.clear();
    Manouver_Go_To_Charging_Station();
  }

 // Esegui il comando "Pause/Stop" (comando 11)
 if (val_WIFI == 11) {
  Serial.println("");
  Serial.print(F("WIFI Command: "));
  Serial.print(val_WIFI);
  Serial.println(F(" | Pause/Stop"));

  // 🛑 Ferma immediatamente motori e lama
  Motor_Action_Stop_Spin_Blades();
  Motor_Action_Stop_Motors();

  // 🔄 Cancella qualsiasi stato in corso (ricerca filo, tracciamento, rotazioni ecc.)
  Abort_Wire_Find = 1;
  Tracking_Wire = 0;
  Mower_Track_To_Exit = 0;
  Mower_Track_To_Charge = 0;
  Loop_Cycle_Mowing = 0;
  Manuel_Mode = 0;

  // 🅿️ Parcheggia
  Manouver_Park_The_Mower();

  // (Opzionale) stampa per debug
  Serial.println(F("Stati tracciamento e ricerca filo annullati."));

  val_WIFI = 0;  // Reset per eseguire il comando solo una volta
}


  // Modalità Manuale (comando 15)
  if (val_WIFI == 15) {    
    Serial.println("");
    Serial.print(F("WIFI Command: ")); 
    Serial.print(val_WIFI);
    Serial.println(F("|Manual Mode"));
    Manouver_Park_The_Mower(); 
    delay(1000);
    Manouver_Manuel_Mode(); 
    Turn_On_Relay(); 
    val_WIFI = 0;   // Reset per eseguire il comando solo una volta
  }

  // Modalità Automatica RANDOM (comando 16)
  if (val_WIFI == 16) {    
    Serial.println("");
    Serial.print(F("WIFI Command: ")); 
    Serial.print(val_WIFI);
    Serial.println(F("|Automatic Mode RANDOM"));
    lcd.clear();
    lcd.print("Taglio Random");
    lcd.setCursor(0,1);
    lcd.print("Modello");
    delay(200);
    lcd.clear();
    if (Mower_Running == 0) {
      Manouver_Park_The_Mower(); 
    }
    Manuel_Mode = 0;
    Manouver_Park_The_Mower();
    //Turn_On_Relay();
    Pattern_Mow = 0;
    val_WIFI = 0;  // Reset per eseguire il comando solo una volta
  }
}

// Funzione per eseguire i comandi manuali per il movimento (ad esempio per la ruota)
void Execute_Manuel_MQTT_Command_To_Mower() {
  if (val_WIFI == 17) {    
    Serial.print(F("WIFI:")); 
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Forward"));
    SetPins_ToGoForwards(); 
    Motor_Action_Go_Full_Speed();
    delay(300);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // Reset per eseguire il comando solo una volta
  }

  if (val_WIFI == 18) {    
    Serial.print(F("WIFI:")); 
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Reverse"));
    SetPins_ToGoBackwards(); 
    Motor_Action_Go_Full_Speed();
    delay(300);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // Reset per eseguire il comando solo una volta
  }

  if (val_WIFI == 19) {    
    Serial.print(F("WIFI:")); 
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Left"));
    SetPins_ToTurnLeft();
    Motor_Action_Go_Full_Speed();
    delay(200);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // Reset per eseguire il comando solo una volta
  }

  if (val_WIFI == 20) {    
    Serial.print(F("WIFI:")); 
    Serial.print(val_WIFI);
    Serial.print(F("|Wheel Right"));
    SetPins_ToTurnRight();
    Motor_Action_Go_Full_Speed();
    delay(200);
    Motor_Action_Stop_Motors();
    val_WIFI = 0;   // Reset per eseguire il comando solo una volta
  }
  
   // Esegui il comando "Pause/Stop" (comando 11)
  if (val_WIFI == 11) {
   Serial.println("");
   Serial.print(F("WIFI Command: "));
   Serial.print(val_WIFI);
   Serial.println(F(" | Pause/Stop"));

   // 🛑 Ferma immediatamente motori e lama
   Motor_Action_Stop_Spin_Blades();
   Motor_Action_Stop_Motors();

   // 🅿️ Parcheggia
   Manouver_Park_The_Mower();
   val_WIFI = 0;  // Reset per eseguire il comando solo una volta
 }
}
