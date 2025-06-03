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
  Serial2.print(Volts);           // Send the voltage value
  Serial2.println("\g");          // Send end-of-message character for Volts
  delay(5);

  Serial2.print(Loop_Cycle_Mowing); // Send the loop cycle status
  Serial2.println("\c");           // Send end-of-message character for Loop_Cycle_Mowing
  delay(5);

  Serial2.print(Mower_Docked);    // Send the docked status
  Serial2.println("\d");          // Send end-of-message character for Mower_Docked
  delay(5);

  Serial2.print(Mower_Running);   // Send whether the mower is running
  Serial2.println("\z");          // Send end-of-message character for Mower_Running
  delay(5);

  Serial2.print(Mower_Parked);    // Send whether the mower is parked
  Serial2.println("\y");          // Send end-of-message character for Mower_Parked
  delay(5);

  Serial2.println(Charging);      // Send charging status
  Serial2.println("\o");          // Send end-of-message character for Charging
  delay(5);

  Serial2.println(Tracking_Wire); // Send tracking wire status
  Serial2.println("\m");          // Send end-of-message character for Tracking_Wire
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

  // 🅿️ Parcheggia
  Manouver_Park_The_Mower();

  // 🔄 Cancella qualsiasi stato in corso (ricerca filo, tracciamento, rotazioni ecc.)
  Abort_Wire_Find = 1;
  Tracking_Wire = 0;
  Mower_Track_To_Exit = 0;
  Mower_Track_To_Charge = 0;
  Loop_Cycle_Mowing = 0;
  Manuel_Mode = 0;

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
