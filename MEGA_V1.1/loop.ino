void mower_loop() {

  // Leggi le porte seriali per i dati
  Read_Nano();           // Leggi i dati seriali dal nano
  Print_Mower_Status();  // Aggiorna il monitor seriale con lo stato attuale del rasaerba.

  // Il tosaerba è agganciato in attesa di un comando per partire e falciare.
  if (Mower_Docked == 1) {
    Print_LCD_Volt_Info();  // Stampa la tensione sullo schermo LCD
    Check_if_Charging();
    Print_LCD_Info_Docked();  // Stampa le informazioni sullo schermo LCD
    Print_Time_On_LCD();
    Check_Membrane_Switch_Input_Docked();  // Controllare i pulsanti per eventuali input
    TestforBoundaryWire();                 // Il test è che il cavo perimetrale è sotto tensione
    Maneuver_Dock_The_Mower();
    Print_Time_On_Serial();
    Display_Next_Alarm();
    Activate_Alarms();
  }

  // Il tosaerba è parcheggiato pronto per essere avviato/riavviato.
  if (Mower_Parked == 1) {
    Print_LCD_Volt_Info();  // Stampa la tensione sullo schermo LCD
    Check_if_Charging();
    Check_if_Raining_From_Nano();          // Controlla se il sensore dell'acqua rileva pioggia
    Print_LCD_Info_Parked();               // Stampa le informazioni sullo schermo LCD
    Check_Membrane_Switch_Input_Parked();  // Controllare i pulsanti per eventuali input
    TestforBoundaryWire();
    Maneuver_Park_The_Mower();
  }

  // Il tosaerba è parcheggiato con batteria scarica e necessita di ricarica manuale
  if (Mower_Parked_Low_Batt == 1) {
    Print_LCD_Volt_Info();  // Stampa la tensione sullo schermo LCD
    Print_Recharge_LCD();   // Stampa Ricarica sullo schermo LCD
    Check_Membrane_Switch_Input_Parked();
  }
  // Il tagliaerba smarrito viene messo in modalità standby
  if (Mower_Error == 1) {
    Print_Mower_Error();  // Modalità di sicurezza in caso di smarrimento del tosaerba o in uno stato di errore
    Check_Membrane_Switch_Input_Parked();
    Maneuver_Park_The_Mower();
  }
  // Il tosaerba sta tagliando l'erba.
  if (Mower_Running == 1) {
    Print_LCD_Volt_Info();                             // Stampa la tensione sullo schermo LCD.
    Process_Volt_Information();                        // Agire in base alle letture di tensione.
    Check_if_Raining_From_Nano();                      // Testare il sensore pioggia per la pioggia. Se rilevato, invia il tagliaerba a casa.
    Check_Membrane_Keys_Running();                     // Verificare se è necessario arrestare il rasaerba tramite tastiera.
    Check_Timed_Mow();                                 // Controlla se è arrivato il momento di tornare a casa.
    TestforBoundaryWire();                             // Il test è che il cavo perimetrale è sotto tensione.
    if (Wheel_Amp_Sensor_ON == 1) Check_Wheel_Amps();  // Test per vedere se le ruote sono bloccate.
    if (Tip_Safety == 1) Compass_Check_Tilt_Angle();   // Prove per vedere se il tosaerba è capovolto.
    if (Wire_Detected == 1) {
      Check_Wire_In_Out();  // Verificare se il tosaerba è dentro o fuori dal recinto di filo.
    if (Outside_Wire == 0) {
        Check_Sonar_Sensors();  // Se il tosaerba è il cavo perimetrale, controlla se i sonar non presentano ostacoli e stampa sul display LCD.
        if (Sonar_Hit == 0) {
          Maneuver_Mow_The_Grass();  // Ingressi ai motori delle ruote/motori delle lame in base all'ambiente.
          Check_Bumper();            // Se il tagliaerba è il cavo perimetrale, controllare l'attivazione del paraurti.
        }
    }
    if (((Outside_Wire == 1) || (Bumper == 1)) && (Loop_Cycle_Mowing > 0)) Maneuver_Turn_Around();  // Se il paraurti è attivato o il tosaerba è fuori dal cavo perimetrale, girarsi.
    if ((Outside_Wire == 0) && (Sonar_Hit == 1)) Maneuver_Turn_Around_Sonar();                      // Se viene rilevato un colpo del sonar e il tosaerba è il cavo, aggirare l'ostacolo.
    }
  }
  
  // WIFI Comandi da e verso APP
  if (Manual_Mode == 1) {
    Receive_WIFI_Manual_Commands();
    Print_LCD_Info_Manual();
  }

  if ((WIFI_Enabled == 1) && (Manual_Mode == 0)) Get_WIFI_Commands();  // Dati TX e RX da NodeMCU

  Serial.println();
}