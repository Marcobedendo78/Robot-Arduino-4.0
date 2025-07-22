// Dopo aver tracciato il cavo il codice per portare il tagliaerba in una posizione ragionevole lontano dal cavo
// Questa parte del codice dovrebbe essere modificata per adattarsi alle esigenze del tuo giardino
void Special_Move_Into_Garden_Zone_X() {
    Serial.println(F("start mower code here"));
    Motor_Action_Stop_Motors();                                     // Stop the wheel motors
    GYRO_Heading_Locked = 0;                                        // Disattiva correzione direzionale
    if (CCW_Tracking_To_Start == 1) SetPins_ToTurnLeft();           // Gira a SX
    if (CW_Tracking_To_Start == 1) SetPins_ToTurnRight();           // Gira a DX
    Motor_Action_Max_Slow_Speed;                                       
    delay(900);                                                     // Tempo di rotazione (Regolare in modo che ruoti di 90° rispetto il cavo)
    Motor_Action_Stop_Motors();                                     // Ferma i motori
    delay(500);
    SetPins_ToGoForwards();                                         // Pronto per lavorare
    Motor_Action_Max_Slow_Speed;
    delay(200);
    // Dopo la rotazione
    Get_GYRO_Reading();                                             // Per registrare l'orientamento finale
    Gyro_Heading = 0;                                               // Reset accumulato
    targetHeading = 0;                                              // Nuova direzione da mantenere
    lastTime = millis();                                            // ⚠ Importante per evitare errori di tempo al prossimo ciclo
    Compass_Heading_Locked = 0;
    delay(1000);
    }

void Special_Exit_From_Docking_Station() {
  if (Wire_Detected == 1){                                          // Aggiunto per controllare se il filo è acceso prima di uscire dalla base
    lcd.clear();
    lcd.print(F("Uscita Garage"));
    delay(1500);
    SetPins_ToGoBackwards();                                        // Prepara i pin dei motori per andare all'indietro
    Motor_Action_GoFullSpeed_Out_Garage();                          // Gira le ruote
    Serial.print(F("Left Wheel PWM:"));
    Serial.print(PWM_Slow_Speed_LH);
    Serial.print("|");
    Serial.print(F("Right Wheel PWM:"));
    Serial.println(PWM_Slow_Speed_RH);   
    delay(2000);                                                   // Tempo di uscita dalla base
    Motor_Action_Stop_Motors();                                    // Stop
    delay(1000);                                                   // Tempo di sosta
   
    if (CCW_Tracking_To_Start == 1) {                              // Uscita dal Garage in senso Antiorario
        Motor_Action_Stop_Motors();                                // Stop
        SetPins_ToTurnLeft();                                      // Prepara i motori a girare a Sinistra
        Motor_Action_Max_Slow_Speed();                             // Gira le ruote
        delay(800);                                                // Tempo di rotazione 
        Motor_Action_Stop_Motors();                                // Stop
        }

    if (CW_Tracking_To_Start == 1) {                               // Uscita dal Garage in senso Orario
        Motor_Action_Stop_Motors();                                // Stop
        SetPins_ToTurnRight();                                     // Prepara i motori a girare a Destra
        Motor_Action_Max_Slow_Speed();                             // Gira le ruote
        delay(800);                                                // Tempo di rotazione
        Motor_Action_Stop_Motors();                                // Stop
        }
    
    lcd.clear();                                                    // Pulisci LCD display
    lcd.print("Garage Libero");                                     // Scrivi sul LCD display      
    lcd.clear();

   if (Wire_Detected == 0){                                         // Se il filo è spento considera il Tagliaerba nella base e non lo fa ucire
    Mower_Docked = 1;
    }
    Motor_Action_Stop_Motors();                                     // Stop
    delay(1000);                                                    // Tempo di sosta
  }
}

// Avvia un algoritmo per ritrovare il filo dopo averlo perso nel tracciamento
void Specials_Find_Wire_Track()  {

 Serial.println("");
  Serial.println(F("Lost Mower - find wire Track"));
  lcd.clear();
  lcd.print("Ricerca filo... ");
  //Motor_Action_Stop_Spin_Blades();                                                          // Disattivato per prova 27/09/2022
  delay(5);
  Abort_Wire_Find = 0;
  //No_Wire_Found = 0;
 // TestforBoundaryWire();                                                                    // Check to see that the wire is on.

  for (int i = 0; i <= 1; i++) {
    if (WIFI_Enabled == 1) Get_WIFI_Commands();
    Serial.print(F("Position Try = "));
    Serial.println(i);
    ADCMan.run();
    UpdateWireSensor();
    delay(40);
    ADCMan.run();
    UpdateWireSensor();
    delay(40);
    PrintBoundaryWireStatus();
    //No_Wire_Found = 0;
    Wire_Find_Attempt = 0;  

    // Per prima cosa andare indietro se il tagliaerba è fuori dal cavo
    if ( inside == false) {                                    // If the mower is outside the wire then run the following code.
      Serial.println("Reversing to find the wire");
      ADCMan.run();
      UpdateWireSensor();
      PrintBoundaryWireStatus();
      Motor_Action_Stop_Motors();                                                           // Stop all wheel motion
      Loop_Cycle_Mowing = 0;
      delay(1000);
      SetPins_ToGoBackwards();                                                              // Set the mower to back up
      delay(100);
      lcd.clear();
      lcd.print("Prova indietro...  ");
      lcd.setCursor(0,1);
      lcd.print("Ricerca Filo  ");
      delay(100);
      
      // Mentre il tagliaerba è ancora fuori dal cavo perimetrale, esegui questo codice a meno che non arrivi il segnale di interruzione dall'APP o finisca i tentativi.
      while (( inside != true) && (Wire_Find_Attempt < 100) ){
        Motor_Action_Max_Slow_Speed_2();                                                    // Velocità ridotta ne posizionamento sul filo
        UpdateWireSensor();                                                                 // Read the wire sensor and see of the mower is now  or outside the wire
        ADCMan.run();
        PrintBoundaryWireStatus();                                                          // Prints of the status of the wire sensor readings.
        Serial.println("");
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Wire_Find_Attempt = Wire_Find_Attempt + 1;                                                      // Counts how many loops have passed to find the wire.
        Serial.print(F("No Wire Count Backwards:"));
        Serial.print(Wire_Find_Attempt);
        Serial.print("|");
        }

          if (Wheel_Blocked == 4) {
            Motor_Action_Stop_Motors();
            SetPins_ToGoBackwards();
            Motor_Action_Go_Accel(Mower_Reverse_Delay);
            //delay(Mower_Reverse_Delay);
            Motor_Action_Stop_Motors();
            Bumper = false; 
            delay(100);
            }
            
      }
      
      Motor_Action_Stop_Motors();
      delay(5);
      }
    
    Wire_Find_Attempt = 0; 
    // Codice per andare avanti fino a quando il rasaerba si trova all'esterno / sul cavo
    if ( inside == true) {             // If the Mower is situated  the wire then run the following code.
        Serial.println(F("Moving Forwards to find the wire"));
        ADCMan.run();
        UpdateWireSensor();
        Serial.println(F("CODE POSITION - MOTOR FORWARDS LOOP:  If statements"));
        PrintBoundaryWireStatus();
        Motor_Action_Stop_Motors();
        delay(1000);  
        SetPins_ToGoForwards();                                                                  // Set the motors to move the mower forwards
        delay(100);
        lcd.clear();
        lcd.print("Prova avanti...  ");
        lcd.setCursor(0,1);
        lcd.print(F("Ricerca Filo    "));
        delay(100);                                                                              // resets the cycles
        while (( inside != false) && (Wire_Find_Attempt < 100)) {                                // Move the mower forward until mower is outisde/ON the wire fence or 500 cycles have passed
          Motor_Action_Max_Slow_Speed_2();                                                       // Velocità ridotta ne posizionamento sul filo
          Check_Bumper();                                                                        // Nuova aggiunta
          UpdateWireSensor();                                                                    // Read the wire sensor and see of the mower is now  or outside the wire
          ADCMan.run();
          PrintBoundaryWireStatus();                                                             // Prints of the status of the wire sensor readings.
          Serial.println("");
          if (WIFI_Enabled == 1) Get_WIFI_Commands();
          Wire_Find_Attempt = Wire_Find_Attempt + 1;                                             // Counts how many loops have passed to find the wire.
          Serial.print("No Wire Count Forwards:");
          Serial.print(Wire_Find_Attempt);
          Serial.print("|");
          }

          if (Wheel_Blocked == 4) {
            Motor_Action_Stop_Motors();
            SetPins_ToGoForwards();
            Motor_Action_Go_Accel(Mower_Reverse_Delay);
            delay(Mower_Reverse_Delay);
            Motor_Action_Stop_Motors();
            Bumper = false; 
            delay(100);
            }
         
      }
      Motor_Action_Stop_Motors();
      Loop_Cycle_Mowing = 0;
      delay(5);
 }
