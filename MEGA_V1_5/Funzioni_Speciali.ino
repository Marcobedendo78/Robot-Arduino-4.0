// Dopo aver tracciato il cavo il codice per portare il tagliaerba in una posizione ragionevole lontano dal cavo
// Questa parte del codice dovrebbe essere modificata per adattarsi alle esigenze del tuo giardino
void Special_Move_Into_Garden_Zone_X() {
    Serial.println(F("start mower code here"));
    Motor_Action_Stop_Motors();                                     // Stop the wheel motors
    if (CCW_Tracking_To_Start == 1) SetPins_ToTurnLeft();           // Gira a SX
    if (CW_Tracking_To_Start == 1) SetPins_ToTurnRight();           // Gira a DX
    Motor_Action_Go_Slow_Speed();                                       
    delay(900);                                                     // Tempo di rotazione (Regolare in modo che ruoti di 90° rispetto il cavo)
    Motor_Action_Stop_Motors();                                     // Ferma i motori
    delay(500);
    SetPins_ToGoForwards();                                         // Pronto per lavorare
    Motor_Action_Go_Slow_Speed();
    delay(200);
    // Dopo la rotazione
    Get_GYRO_Reading();                                             // Per registrare l'orientamento finale
    Gyro_Heading = 0;                                               // Reset accumulato
    targetHeading = 0;                                              // Nuova direzione da mantenere
    lastTime = millis();                                            // ⚠ Importante per evitare errori di tempo al prossimo ciclo
    Compass_Heading_Locked = 0;
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
    delay(1900);                                                   // Tempo di uscita dalla base
    Motor_Action_Stop_Motors();                                    // Stop
    delay(1000);                                                   // Tempo di sosta
   
    if (CCW_Tracking_To_Start == 1) {                              // Uscita dal Garage in senso Antiorario
        Motor_Action_Stop_Motors();                                // Stop
        SetPins_ToTurnLeft();                                      // Prepara i motori a girare a Sinistra
        Motor_Action_Max_Slow_Speed();                             // Gira le ruote
        delay(500);                                                // Tempo di rotazione 
        Motor_Action_Stop_Motors();                                // Stop
        }

    if (CW_Tracking_To_Start == 1) {                               // Uscita dal Garage in senso Orario
        Motor_Action_Stop_Motors();                                // Stop
        SetPins_ToTurnRight();                                     // Prepara i motori a girare a Destra
        Motor_Action_Max_Slow_Speed();                             // Gira le ruote
        delay(500);                                                // Tempo di rotazione
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
void Specials_Find_Wire_Track() {
  Serial.println("");
  Serial.println(F("Lost Mower - find wire Track"));
  lcd.clear();
  lcd.print("Ricerca filo...");
  delay(5);

  Abort_Wire_Find = 0;
  Wire_Find_Attempt = 0;
  Outside_Wire_Count = 0;

  for (int i = 0; i <= 1; i++) {
    Serial.print(F("Position Try = "));
    Serial.println(i);

    ADCMan.run();
    UpdateWireSensor();  // aggiorna inside
    delay(40);

    // 🔙 Se è fuori dal filo, prova ad andare indietro
    if (!inside) {
      Serial.println("Reversing to find the wire");
      Motor_Action_Stop_Motors();
      Loop_Cycle_Mowing = 0;
      delay(1000);

      SetPins_ToGoBackwards();
      lcd.clear();
      lcd.print("Prova indietro...");
      lcd.setCursor(0, 1);
      lcd.print("Ricerca Filo");
      delay(100);

      while (!inside && Wire_Find_Attempt < 100 && !Abort_Wire_Find) {
        Motor_Action_Go_Slow_Speed();
        ADCMan.run();
        UpdateWireSensor();  // inside aggiornato qui
        PrintBoundaryWireStatus();

        if ((WIFI_Enabled == 1) && (Manuel_Mode == 0)) Get_WIFI_Commands();

        Wire_Find_Attempt++;
        Serial.print(F("No Wire Count Backwards: "));
        Serial.println(Wire_Find_Attempt);

        if (Wheel_Blocked == 4) {
          Motor_Action_Stop_Motors();
          SetPins_ToGoBackwards();
          Motor_Action_Go_Accel();
          delay(Mower_Reverse_Delay);
          Motor_Action_Stop_Motors();
          Bumper = false;
          delay(1000);
        }
      }

      Motor_Action_Stop_Motors();
      delay(1000);
    }

    // 🔛 Se è dentro al filo, prova ad avanzare per allinearsi
    Wire_Find_Attempt = 0;
    ADCMan.run();
    UpdateWireSensor();

    if (inside) {
      Serial.println(F("Moving Forwards to find the wire"));
      Motor_Action_Stop_Motors();
      delay(1000);

      SetPins_ToGoForwards();
      lcd.clear();
      lcd.print("Prova avanti...");
      lcd.setCursor(0, 1);
      lcd.print(F("Ricerca Filo"));
      delay(100);

      while (inside && Wire_Find_Attempt < 100 && !Abort_Wire_Find) {
        Motor_Action_Go_Slow_Speed();
        Check_Bumper();
        ADCMan.run();
        UpdateWireSensor();  // inside aggiornato qui
        Check_Wire_In_Out();
        PrintBoundaryWireStatus();

        if ((WIFI_Enabled == 1) && (Manuel_Mode == 0)) Get_WIFI_Commands();

        Wire_Find_Attempt++;
        Serial.print(F("No Wire Count Forwards: "));
        Serial.println(Wire_Find_Attempt);

        if (Wheel_Blocked == 4) {
          Motor_Action_Stop_Motors();
          SetPins_ToGoForwards();
          Motor_Action_Go_Accel();
          delay(Mower_Reverse_Delay);
          Motor_Action_Stop_Motors();
          Bumper = false;
          delay(2000);
        }
      }

      Motor_Action_Stop_Motors();
      delay(1000);
    }
    // ❌ Se il robot è stato troppo tempo fuori dal perimetro, ha fallito la ricerca o è stato interrotto → parcheggia
   if (Outside_Wire_Count >= Outside_Wire_Count_Max || Wire_Find_Attempt >= 100 || Abort_Wire_Find) {
     Serial.println(F("Wire not found or operation aborted → Parking robot"));
     Motor_Action_Stop_Motors();
     Manouver_Park_The_Mower();
     return;
   }

    Motor_Action_Stop_Motors();
    Loop_Cycle_Mowing = 0;
    delay(1000);
  }
}
