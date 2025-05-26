void Maneuver_Find_Wire_Track_Exit() {


  Serial.println(F("Find Wire Track Function Activated"));
  lcd.clear();
  lcd.print("Ricerca Filo...  ");
  //Motor_Action_Stop_Spin_Blades();                                                      //Tolto il 27/06/2023 se perde il filo nel tracciamento quando riparte non spegne le lame
  delay(5);
  Abort_Wire_Find = 0;
  No_Wire_Found_Fwd = 0;
  No_Wire_Found_Bck = 0;
  Mower_Running = 0;
  TestforBoundaryWire();  // Check to see that the wire is on.
  Loop_Cycle_Mowing = 0;
  int cycle = 0;


  for (int i = 0; i <= 1; i++) {
    if (Mower_Parked == 0) {
      if (WIFI_Enabled == 1) Get_WIFI_Commands();
      Serial.print(F("Position Try = "));
      Serial.println(i);
      ADCMan.run();
      UpdateWireSensor();
      delay(20);
      ADCMan.run();
      UpdateWireSensor();
      delay(20);
      Serial.println(F("before if loops start"));
      PrintBoundaryWireStatus();

      // Per prima cosa torna indietro se il tosaerba è fuori dal cavo
      if ((inside == false) && (Abort_Wire_Find == 0)) {  // If the mower is outside the wire then run the following code.
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
        Motor_Action_Stop_Motors();  // Stop all wheel motion
        delay(500);
        SetPins_ToGoBackwards();  // Set the mower to back up
        delay(100);
        lcd.clear();
        lcd.print("Prova Indietro...  ");
        lcd.setCursor(0, 1);
        lcd.print("Ricerca filo  ");
        delay(100);
        cycle = 0;

        // Far funzionare il tosaerba all'indietro finché non viene rilevato il cavo e il tosaerba si trova all'interno del cavo
        while ((inside != true) && (Abort_Wire_Find == 0) && (No_Wire_Found_Bck == 0)) {  // While the mower is still outside the fence run this code
          Loop_Cycle_Mowing = 222;                                                        // Display this number "222" in the APP under loop cycles.
          cycle = cycle + 1;                                                              // adds one to the cycle count
          Motor_Action_Go_Slow_Speed();                                                   // Go full speed (in this case backwards)
          UpdateWireSensor();                                                             // Read the wire sensor and see of the mower is now  or outside the wire
          ADCMan.run();
          PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings.
          Serial.println("");
          if ((WIFI_Enabled == 1) && (Manual_Mode == 0)) Get_WIFI_Commands();
          if (Mower_Parked == 1) {
            Serial.println("Abort Wire Find");
            Abort_Wire_Find = 1;
          }
          if (cycle > Max_Cycle_Wire_Find_Back) {  // Traccia in avanti per Max_Cycle_Wire_Find_Back cycles
            No_Wire_Found_Bck = 1;                 // if mower is still tracking after Max_Cycle_Wire_Find_Back cycles then cancel the find.
            Serial.println("Max Backward Cycles reached");
          }
        }

        Motor_Action_Stop_Motors();
        delay(1000);
      }

      // Codice per andare avanti finché il rasaerba non si trova all'esterno/SUL cavo
      if ((inside == true) && (Abort_Wire_Find == 0) && (No_Wire_Found_Fwd == 0)) {  // Se il tosaerba è posizionato sul cavo, esegui il seguente codice.
        ADCMan.run();
        UpdateWireSensor();
        Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
        PrintBoundaryWireStatus();
        Motor_Action_Stop_Motors();
        delay(500);
        SetPins_ToGoForwards();  // Set the motors to move the mower forwards
        delay(100);
        lcd.clear();
        lcd.print("Prova Avanti...  ");
        lcd.setCursor(0, 1);
        lcd.print("Ricerca Filo  ");
        delay(100);
        cycle = 0;  // resets the cycles

        // Muove il tosaerba in avanti finché non viene rilevato il cavo e il tosaerba si trova fuori dal cavo
        while ((inside != false) && (No_Wire_Found_Fwd == 0) && (Mower_Parked == 0)) {  // Spostate il tosaerba in avanti finché non si trova fuori/SUL recinto di filo metallico o finché non sono trascorsi 500 cicli
          Loop_Cycle_Mowing = 111;                                                      // Displays 111 in the APP
          cycle = cycle + 1;
          lcd.setCursor(0, 1);
          lcd.print("Traccia -> Filo ");
          Motor_Action_Go_Slow_Speed();  // Go full speed (in this case forwards)
          Check_Bumper();                // Nuova aggiunta
          for (int i = 0; i < 3; i++) {  // Nuova aggiunta 15/06/2024
            delay(2);
            ADCMan.run();
            UpdateWireSensor();
            delay(2);
            ADCMan.run();
            UpdateWireSensor();
            delay(2);
            PrintBoundaryWireStatus();
          }
          //UpdateWireSensor();                                                                   // Read the wire sensor and see of the mower is now  or outside the wire
          //ADCMan.run();
          //PrintBoundaryWireStatus();                                                            // Prints of the status of the wire sensor readings.
          Serial.println("");
          if ((WIFI_Enabled == 1) && (Manual_Mode == 0)) Get_WIFI_Commands();
          if (Mower_Parked == 1) {
            Serial.println("Abort Wire Find");
            Abort_Wire_Find = 1;
          }
          if (Bumper == true) {  // Nuova aggiunta
            Motor_Action_Stop_Motors();
            SetPins_ToGoBackwards();
            Motor_Action_Go_Slow_Speed();
            delay(Mower_Reverse_Delay);
            Motor_Action_Stop_Motors();
            //Compass_Turn_Mower_To_Home_Direction();                                             // Nuova aggiunta 15/06/2024
            Bumper = false;
            delay(2000);
            cycle = Max_Cycle_Wire_Find;
          }



          if (cycle > Max_Cycle_Wire_Find) {  // Track forwards for Max_Cycle_Wire_Find_Fwd cycles
            No_Wire_Found_Fwd = 1;            // if mower is still tracking after Max_Cycle_Wire_Find_Fwd cycles then cancel the find.
            Serial.println("Max Forward Cycles reached");
          }
        }
        Motor_Action_Stop_Motors();
        delay(1000);
      }
      Motor_Action_Stop_Motors();
      delay(100);
    }
  }

  // Posizionare il tosaerba più lontano sul cavo in modo che abbia spazio per ruotare di 90° sul cavo.
  if ((Abort_Wire_Find == 0) && (No_Wire_Found_Fwd == 0) && (Mower_Parked == 0)) {
    SetPins_ToGoForwards();
    delay(500);
    Motor_Action_Go_Slow_Speed();
    delay(200);
    Motor_Action_Stop_Motors();
    delay(1000);
  }

  // Imposta l'attivazione della rotazione del tosaerba dopo l'uscita dalla base


  // Imposta i pin per tracciare uscire dalla base.
  if ((Mower_Track_To_Exit == 1) && (Mower_Parked == 0)) {
    lcd.setCursor(0, 1);
    lcd.print("Traccia-> Uscita");
    delay(500);
    if (CW_Tracking_To_Start == 1) {
      SetPins_ToTurnRight();  // Track perimeter wire in a Clockwise Direction to the charging station
      Serial.println(F("CW Tracking to Exit"));
    }
    if (CCW_Tracking_To_Start == 1) {
      SetPins_ToTurnLeft();
      Serial.println(F("CCW Tracking to Exit"));
    }
  }


  delay(20);
  // I pin sono ora impostati per girare secondo la logica di cui sopra in che direzione girare sul filo

  // Update the mowers position to the wire.
  ADCMan.run();
  UpdateWireSensor();
  ADCMan.run();
  PrintBoundaryWireStatus();  // Prints of the status of the wire sensor readings
  delay(1000);





  //int Max_Spin_Attempts_Exit = 15;                                                                // Prima era 15 26/05/2024
  int Spin_Attempts_Exit = 0;
  int WIFI_Check_Up;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Gira sul Filo  ");
  // Fa girare il tosaerba sul cavo nella direzione di tracciamento
  while ((inside == false) && (Abort_Wire_Find == 0) && (No_Wire_Found_Fwd == 0) && (Mower_Parked == 0) && (Spin_Attempts_Exit < Max_Spin_Attempts_Exit)) {
    while ((inside != true) && (Spin_Attempts_Exit < Max_Spin_Attempts_Exit)) {  // Fate questo giro finché il tosaerba non torna indietro rispetto al recinto di filo perimetrale                                                                  // Fate questo giro finché il tosaerba non torna indietro rispetto al recinto di filo perimetrale
      Serial.print(F("Spin Attempts_Exit"));
      Serial.print(Spin_Attempts_Exit);
      lcd.setCursor(0, 1);
      lcd.print(Spin_Attempts_Exit);
      Motor_Action_Go_Slow_Speed();  // Andare a tutta velocità (nel caso girando come previsto dalla logica precedente)
      UpdateWireSensor();            // Leggere il sensore del cavo e vedere se il tosaerba si trova ora o fuori dal cavo
      ADCMan.run();
      PrintBoundaryWireStatus();                    // Stampa dello stato delle letture del sensore filo.
      Spin_Attempts_Exit = Spin_Attempts_Exit + 1;  // controlla che il tosaerba non si blocchi cercando di girare sul cavo e rimanga intrappolato in questo anello
      WIFI_Check_Up = WIFI_Check_Up + 1;
      if (WIFI_Check_Up = 20) {
        if ((WIFI_Enabled == 1) && (Manual_Mode == 0)) Get_WIFI_Commands();
        WIFI_Check_Up = 0;
      }
    }
    Motor_Action_Stop_Motors();  // Fermare il tosaerba sul cavo rivolto nella direzione corretta.
    delay(2000);
  }


  Motor_Action_Stop_Motors();
  if ((Abort_Wire_Find == 0) && (Spin_Attempts_Exit < Max_Spin_Attempts_Exit)) {
    Serial.println(F("Track Wire Function Complete - ON WIRE??"));
    lcd.clear();
    lcd.print("Filo Trovato....");
    delay(2000);  // 2 second pause to check result
    SetPins_ToGoForwards();
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Traccia -> Filo ");
    lcd.clear();
  }
  /*  
  // se è stata ricevuta un'interruzione o il tosaerba non gira nella direzione giusta sul cavo, ripeti il ciclo di ricerca del cavo.
  if ((Abort_Wire_Find == 1) || (Spin_Attempts_Exit >= Max_Spin_Attempts_Exit))  {
    Serial.println("Wire Find Aborted");
    lcd.clear();
    lcd.print("Trova Filo ABORT");
    delay(1000);
    Abort_Wire_Find = 0;
    SetPins_ToGoForwards();
    Maneuver_Park_The_Mower();
    }
*/
  if (No_Wire_Found_Fwd == 1) {
    Serial.println("Re-starting wire find");
    SetPins_ToGoForwards();
  }
}
