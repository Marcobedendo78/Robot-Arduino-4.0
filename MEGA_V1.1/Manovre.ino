// Le manovre sono un insieme di funzioni di movimento o azioni motorie che vengono chiamate regolarmente
// e.g. Turn the mower around

// Sposta il tosaerba in avanti nel giardino e attiva le funzioni della bussola se abilitate
void Maneuver_Mow_The_Grass() {

  Motor_Action_Spin_Blades();
  SetPins_ToGoForwards();
  Loop_Cycle_Mowing = (Loop_Cycle_Mowing + 1);
  Serial.print(F("Loop:"));
  Serial.print(Loop_Cycle_Mowing);
  Serial.print(F("|"));
  lcd.setCursor(14, 1);

  delay(1);

  if (Loop_Cycle_Mowing < 3) {
    Serial.print(F("C-Lock:OFF"));
    Serial.print(F("|"));
    Print_LCD_Mowing();
    if (Compass_Activate == 1) Get_Compass_Reading();
    Motor_Action_Go_Full_Speed();
    Compass_Heading_Locked = 0;  // Disattiva il blocco della direzione della bussola per i nuovi cicli
  }

  // Al 3° Ciclo di taglio vengono scelte le opzioni come Tagliare in base alle impostazioni del modello di taglio
  // Compass assistance etc.

  if (Loop_Cycle_Mowing == 3) {

    // Taglio casuale normale
    if (Pattern_Mow == 0) {

      // Normale Taglio Casuale senza bussola Assistenza
      if (Compass_Heading_Hold_Enabled == 0) {
        Serial.print(F("C-Lock:OFF"));
        Serial.print(F("|"));
        Print_LCD_Compass_Mowing();
        Motor_Action_Go_Full_Speed();
        Compass_Heading_Locked = 0;
      }

      // Normale Taglio Casuale con bussola Assistenza accesa
      if ((Compass_Heading_Hold_Enabled == 1) && (Compass_Activate == 1)) {
        Get_Compass_Reading();                   // Gets the latest compass reading
        Heading_Lock = Compass_Heading_Degrees;  // saves this compass reading to the heading lock
        Compass_Heading_Locked = 1;              // Turns on the heading lock feature
        Motor_Action_Go_Full_Speed();
      } else {
        Motor_Action_Go_Full_Speed();
        Serial.println("Compass not activated in the settings");
      }
    }

    if (Pattern_Mow == 1) {
      Motor_Action_Go_Full_Speed();
      Print_LCD_Parallel();
      Serial.print("Parallel:ON");
      Serial.print(F("|"));

      if ((Compass_Heading_Hold_Enabled == 1) && (Compass_Activate == 1)) {  // use the heading hold funtion for Parallel Mowing
        Get_Compass_Reading();                                               // Gets the latest compass reading
        Heading_Lock = Compass_Heading_Degrees;                              // saves this compass reading to the heading lock
        Compass_Heading_Locked = 1;                                          // Turns on the heading lock feature
        Motor_Action_Go_Full_Speed();
      } else {
        Motor_Action_Go_Full_Speed();  // if the compass is not activated
        Serial.println("Compass not activated in the settings");
      }
    }

    // Imposta le variabili in modo da attivare uno schema di taglio a spirale.
    if (Pattern_Mow == 2) {
      Compass_Heading_Locked = 0;  // Compass Lock is switched off
      Print_LCD_Spiral();
      Serial.print("Spiral:ON");
      Serial.print(F("|"));
      Motor_Action_Go_Full_Speed();
    }

  }  // end of statements for == 3



  // In base alle impostazioni sopra, il tagliaerba continuerà a tagliare con le seguenti azioni
  if (Loop_Cycle_Mowing > 3) {

    if (Pattern_Mow == 0) {
      if (Compass_Heading_Locked == 0) {
        lcd.setCursor(0, 1);
        Serial.print(F("C-Lock:OFF"));
        Serial.print(F("|"));
        lcd.print("Falciatura     ");
        Motor_Action_Go_Full_Speed();
        Compass_Steering_Status = 0;  // Nuova aggiunta
      }
      if ((Compass_Heading_Hold_Enabled == 1) && (Compass_Activate == 1)) {  // if the Mower is tracking using the compass steer here
        if ((Loop_Cycle_Mowing % 2) == 0) {
          Get_Compass_Reading();
          Calculate_Compass_Wheel_Compensation();
          Motor_Action_Dynamic_PWM_Steering();  // Removes the full speed function if the mower is trying to hold to the compass heading.
          Print_LCD_Compass_Mowing();
          Serial.print(F("C-Lock:ON_"));
          Serial.print("|");
        }
      }
    }


    if (Pattern_Mow == 1) {
      Pattern_Mow_Parallel();
    }

    if (Pattern_Mow == 2) {
      Pattern_Mow_Spirals();  // For pattern mow = 2 i.e. circular motion.
    }
  }

  // La logica di Max Cycles decide di quanto deve correre il tagliaerba prima di girarsi
  // Questo è utile se il tosaerba si aggancia a qualcosa e il cavo e i sensori del sonar non reagiscono.
  // La logica ora dipende dal tipo di taglio selezionato. Per il taglio a spirale i cicli massimi sono aumentati in base alle esigenze del taglio a spirale
  // più cicli per completare uno schema a spirale di dimensioni adeguate, seguito da una gamba di collegamento in linea retta alla spirale successiva

  int Max_Cycles_Active;                                                                 // define a veriable to hold the max cycles
  if (Pattern_Mow != 2) Max_Cycles_Active = Max_Cycles_Straight;                         // if normal straight line mowing is slected
  if ((Pattern_Mow == 2) && (Spiral_Mow == 3)) Max_Cycles_Active = Max_Cycles_Straight;  // if spiral is selected but its a straight line leg
  if ((Pattern_Mow == 2) && (Spiral_Mow < 3)) Max_Cycles_Active = Max_Cycles_Spirals;    // if spiral is selected and its a LH or RH spiral

  if (Loop_Cycle_Mowing > Max_Cycles_Active) {  // 150 the max length (Max_Cycles) for my garden. Adjust accordingly in the setup
    Serial.println("");
    Serial.println("Loop Cycle at Max");
    Serial.println("");
    Motor_Action_Stop_Spin_Blades();  // Stop the blades from spinning
    Maneuver_Turn_Around();           // Turn around the mower
    Loop_Cycle_Mowing = 0;            // Restes the loop cycle to start again.
    lcd.clear();
  }
}

// Turn Around definisce come deve reagire il tagliaerba quando viene attivato un cavo o un sensore sonar.
// Ora con il taglio a spirale si decide anche la logica di quale forma a spirale sarà la prossima

void Maneuver_Turn_Around() {
  Motor_Action_Stop_Motors();
  if (Outside_Wire == 1) Serial.println(F("Il tagliaerba è fuori dal cavo"));
  if (Wheel_Blocked == 4) Serial.println(F("Le ruote del tagliaerba sono inceppate"));
  delay(1000);  //Prima 500

  // Eseguire il backup del tosaerba
  SetPins_ToGoBackwards();
  Motor_Action_Go_Accel();
  delay(Mower_Reverse_Delay);
  Motor_Action_Stop_Motors();
  delay(1000);  //Prima 200

  if (Perimeter_Wire_Enabled == 1) {
    Check_Wire_In_Out();

    if (Outside_Wire == 1) {
      for (int i = 0; i < 5; i++) {
        delay(10);
        ADCMan.run();
        UpdateWireSensor();
        delay(10);
        ADCMan.run();
        UpdateWireSensor();
        delay(10);
        PrintBoundaryWireStatus();
      }


      UpdateWireSensor();
      Check_Wire_In_Out();
      if (Outside_Wire == 1) {
        SetPins_ToGoBackwards();
        Motor_Action_Go_Accel();
        delay(600);
        Motor_Action_Stop_Motors();
        delay(100);  //Prima era 100
        UpdateWireSensor();
        Check_Wire_In_Out();
      }
    }
  }


  // Decidere casualmente se il tagliaerba deve girare a sinistra oa destra a seconda che il ciclo del ciclo sia pari o dispari
  if ((Loop_Cycle_Mowing % 2) == 0) {
    SetPins_ToTurnRight();
  } else SetPins_ToTurnLeft();


  // Ruota casualmente il tagliaerba su una nuova rotta a seconda del ritardo Min o Delay Max dalle impostazioni
  Motor_Action_Turn_Speed();
  delay(random(Mower_Turn_Delay_Min, Mower_Turn_Delay_Max));
  //Serial.println("");
  //Serial.println("Mower Turned Around");
  //Serial.println("");

  // Se la modalità Spirale è attivata
  // Fa avanzare il tipo di movimento in modalità Pattern Spiral da: 1 Spirale destra | 2 Spirale sinistra | 3 Linea retta.
  if (Pattern_Mow == 2) {
    Spiral_Mow = (Spiral_Mow + 1);
    if (Spiral_Mow > 3) Spiral_Mow = 1;
  }
  //Spirale (casuale (1,3)); Attivalo per creare un vero modello casuale.

  Motor_Action_Stop_Motors();
  delay(500);

  // Se il cavo perimetrale è abilitato
  // Verificare che il tagliaerba si sia girato e sia tornato all'interno del cavo perimetrale.


  Bumper = 0;             // Reset Paraurti
  Loop_Cycle_Mowing = 1;  // Rest Cicli Loop
  Sonar_Hit = 0;          // Reset Sonar
  distance1 = 999;
  distance2 = 999;
  distance3 = 999;
  Sonar_Hit_1_Total = 0;
  Sonar_Hit_2_Total = 0;
  Sonar_Hit_3_Total = 0;
  Compass_Heading_Locked = 0;  // Reset Compass Heading Lock
  lcd.clear();
}

void Maneuver_Turn_Around_Sonar() {
  Motor_Action_Stop_Motors();
  delay(500);
  SetPins_ToGoBackwards();
  Motor_Action_Go_Accel();
  delay(Mower_Reverse_Delay);
  Motor_Action_Stop_Motors();
  delay(500);

  if (distance2 < maxdistancesonar) {  // Se il Sonar 2 SX legge gira a DX
    lcd.setCursor(0, 8);
    lcd.print("Gira a DX -->   ");
    SetPins_ToTurnRight();
    Motor_Action_Turn_Speed();
    delay(Mower_Turn_Delay_Min);

    Motor_Action_Stop_Motors();
    Check_Sonar_Sensors();
  }



  if (distance3 < maxdistancesonar) {  // Se il Sonar 3 DX legge gira a SX
    lcd.setCursor(0, 8);
    lcd.print("<-- Gira a SX   ");
    SetPins_ToTurnLeft();
    Motor_Action_Turn_Speed();
    delay(Mower_Turn_Delay_Min);
    Motor_Action_Stop_Motors();
    Check_Sonar_Sensors();
  }


  if (distance2 < maxdistancesonar && distance3 < maxdistancesonar) {  // Se il Sonar 2 SX legge gira a DX
    lcd.setCursor(0, 8);
    lcd.print("Gira a DX -->   ");
    SetPins_ToTurnRight();
    Motor_Action_Turn_Speed();
    delay(Mower_Turn_Delay_Min);

    Motor_Action_Stop_Motors();
    Check_Sonar_Sensors();
  }


  Motor_Action_Stop_Motors();
  delay(500);
  Compass_Heading_Locked = 0;
  Sonar_Hit = 0;
  Loop_Cycle_Mowing = 0;

  Check_Sonar_Sensors();
}

void Maneuver_Manual_Mode() {
  Mower_Docked = 0;
  Mower_Parked = 0;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Exit = 0;
  Mower_Track_To_Charge = 0;
  Mower_Error = 0;
  Manual_Mode = 1;
  Loop_Cycle_Mowing = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
}


void Maneuver_Start_Mower() {
  Mower_Docked = 0;
  Mower_Parked = 0;
  Mower_Running = 1;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 0;
  Rain_Hit_Detected = 0;
  Mower_Error = 0;
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  Wire_Refind_Tries = 0;
  Turn_On_Relay();
  Y_Tilt = 0;
}

void Maneuver_Mower_Exit_Dock() {

  Mower_Docked = 0;
  Mower_Parked = 0;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Rain_Hit_Detected = 0;
  Mower_Error = 0;
  Manual_Mode = 0;
  Tracking_Wire = 0;
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
}

void Maneuver_Dock_The_Mower() {
  Mower_Docked = 1;
  Mower_Parked = 0;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Exit = 0;
  Mower_Track_To_Charge = 0;
  Mower_Error = 0;
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
  Turn_Off_Relay();
  Print_LCD_Info_Docked();
  Charge_Detected_MEGA = 0;

  //Setup Alarms
  Alarm_Timed_Mow_ON = 0;  // Turns off the 1 hr Alarm
}

// Il tagliaerba è una posizione parcheggiata e necessita di ricarica manuale
void Maneuver_Park_The_Mower_Low_Batt() {

  Mower_Docked = 0;
  Mower_Parked = 0;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 1;
  Mower_Track_To_Charge = 0;
  Mower_Error = 0;
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
}


// Il tagliaerba è in una pozione parcheggiata o in pausa pronto per il riavvio
void Maneuver_Park_The_Mower() {

  if (Mower_Parked == 0) lcd.clear();
  Mower_Docked = 0;
  Mower_Parked = 1;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 0;
  Tracking_Wire = 0;
  Mower_Track_To_Exit = 0;
  Mower_Error = 0;
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
  Turn_Off_Relay();

  Alarm_Timed_Mow_ON = 0;  // Turns off the 1 hr Alarm
  //if (Alarm_1_Repeat == 0) Alarm_1_ON = 0;
  //if (Alarm_2_Repeat == 0) Alarm_2_ON = 0;
  //if (Alarm_3_Repeat == 0) Alarm_3_ON = 0;
}

void Maneuver_Hibernate_Mower() {

  Mower_Docked = 0;
  Mower_Parked = 1;  // Prima era 0   03/08/2022
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 0;
  Tracking_Wire = 0;
  Mower_Track_To_Exit = 0;
  Mower_Error = 0;  // Prima era 1    03/08/2022
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  Motor_Action_Stop_Motors();
  Motor_Action_Stop_Spin_Blades();
  Turn_Off_Relay();
}

// Il rasaerba viene inviato alla stazione di ricarica dopo il rilevamento di bassi voltaggi o l'inserimento di una chiave a membrana.
void Maneuver_Go_To_Charging_Station() {
  Mower_Docked = 0;
  Mower_Parked = 0;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 1;
  Tracking_Wire = 1;
  Mower_Track_To_Exit = 0;
  Mower_Error = 0;
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  No_Wire_Found_Fwd = 0;
  No_Wire_Found_Bck = 0;
  Manage_Alarms();  // Switches on or off the Alarms depending on the setup
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(5);
  Motor_Action_Stop_Spin_Blades();
  Motor_Action_Stop_Motors();
  delay(2000);
  Turn_On_Relay();
  delay(500);
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if ((Compass_Activate == 1) && (Mower_Parked == 0)) Compass_Turn_Mower_To_Home_Direction();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if (Mower_Parked == 0) Maneuver_Find_Wire_Track();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if ((Mower_Parked == 0) && (No_Wire_Found_Fwd == 0)) Track_Perimeter_Wire_To_Dock();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if (No_Wire_Found_Fwd == 1) Maneuver_Go_To_Charging_Station();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
}

// Il rasaerba viene inviato alla stazione di ricarica dopo il rilevamento di bassi voltaggi o l'inserimento di una chiave a membrana se gira attorno ad un'aiuola.
void Maneuver_Go_To_Charging_Station_2() {
  Mower_Docked = 0;
  Mower_Parked = 0;
  Mower_Running = 0;
  Mower_Parked_Low_Batt = 0;
  Mower_Track_To_Charge = 1;
  Tracking_Wire = 1;
  Mower_Track_To_Exit = 0;
  Mower_Error = 0;
  Loop_Cycle_Mowing = 0;
  Manual_Mode = 0;
  No_Wire_Found_Fwd = 0;
  No_Wire_Found_Bck = 0;
  Manage_Alarms();  // Switches on or off the Alarms depending on the setup
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(5);
  Motor_Action_Stop_Spin_Blades();
  Motor_Action_Stop_Motors();
  delay(2000);
  Turn_On_Relay();
  delay(500);
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if (Mower_Parked == 0) Maneuver_Find_Wire_Track();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if ((Mower_Parked == 0) && (No_Wire_Found_Fwd == 0)) Track_Perimeter_Wire_To_Dock();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if (No_Wire_Found_Fwd == 1) Maneuver_Go_To_Charging_Station();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
}


void Maneuver_Exit_To_Zone_X() {
  // Zone 1 or Zone 2 and the Wire itterations are set on the Membrane Buttons.
  // These values are then crried into the following functions.
  Turn_On_Relay();
  delay(1000);
  Maneuver_Mower_Exit_Dock();
  Special_Exit_From_Docking_Station();  // Move the Mower into position backing out of the docking station
  if (Perimeter_Wire_Enabled == 1) {
    Mower_Track_To_Exit = 1;
    TestforBoundaryWire();
    delay(50);
    if (Wire_Detected == 1) {
      if (WIFI_Enabled == 1) Get_WIFI_Commands();
      Maneuver_Find_Wire_Track_Exit();  // Located the boundary wire
      if (WIFI_Enabled == 1) Get_WIFI_Commands();
      if (Mower_Parked == 0) Track_Wire_From_Dock_to_Zone_X();
      if (WIFI_Enabled == 1) Get_WIFI_Commands();
      if (Mower_Parked == 0) Special_Move_Into_Garden_Zone_X();
      if (WIFI_Enabled == 1) Get_WIFI_Commands();
      if (Mower_Parked == 0) Maneuver_Start_Mower();
      if (WIFI_Enabled == 1) Get_WIFI_Commands();
      if (Mower_Parked == 1) Maneuver_Park_The_Mower();
    }
    if (Wire_Detected == 0) {
      TestforBoundaryWire();     // Test again for the boundary wire
      if (Wire_Detected == 0) {  // if its still saying the wire is off then park the mower.
        Serial.println("");
        Serial.println("Perimeter Wire not detected");
        Maneuver_Park_The_Mower();
      }
    }
  }
  if (Perimeter_Wire_Enabled == 0) {
    Serial.println("");
    Serial.println("Perimeter Wire not activated in settings");
    Maneuver_Park_The_Mower();
  }
}


// Funzione per ritrovare il cavo se il rasaerba perde il cavo durante il taglio
// 3 al di fuori del filo attiva questa funzione. La funzione sonar e filo viene quindi utilizzata per ritrovare il filo.
void Maneuver_Outside_Wire_ReFind_Function() {
  Motor_Action_Stop_Spin_Blades();
  lcd.clear();
  lcd.print("Cercando il Filo");
  lcd.setCursor(0, 1);
  lcd.print("Filo di Nuovo...");
  ADCMan.run();
  UpdateWireSensor();
  delay(40);  // Prima era 20
  PrintBoundaryWireStatus();
  ADCMan.run();
  UpdateWireSensor();
  delay(40);  // Prima era 20
  PrintBoundaryWireStatus();
  while (inside == false) {  // If the mower is outside the wire then run the following code.
    ADCMan.run();
    UpdateWireSensor();
    PrintBoundaryWireStatus();
    //Check_Wire_In_Out();
    delay(500);
    distance_blockage = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 4, 0);
    delay(500);
    Serial.print(F("Distance measured from sonar :"));
    Serial.println(distance_blockage);

    // if the sonar is measuring an opening as the distance is greater than 300cm then move forward in that direction.
    if (distance_blockage > 400) {
      while ((inside == false) && (distance_blockage > 400)) {
        SetPins_ToGoForwards();
        Motor_Action_Go_Full_Speed();
        delay(500);
        ADCMan.run();
        UpdateWireSensor();
        PrintBoundaryWireStatus();
        distance_blockage = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 4, 0);
        delay(10);
        Check_Wire_In_Out();
      }
      Motor_Action_Stop_Motors;
      ADCMan.run();
      UpdateWireSensor();
      PrintBoundaryWireStatus();
    }

    // if the sonar is measuring something less than 300cm then turn to the left and measure again
    // keep turning 'while function' until a path is open.
    while (distance_blockage < 300) {
      SetPins_ToTurnLeft();
      delay(200);
      Motor_Action_Turn_Speed();
      delay(100);
      distance_blockage = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 4, 0);
      delay(10);
    }
    Motor_Action_Stop_Motors;
    ADCMan.run();
    UpdateWireSensor();
    PrintBoundaryWireStatus();
  }

  Serial.println("Mower is now back inside the wire......?");
  lcd.clear();
  lcd.print("Falciatura Ora");
  lcd.setCursor(0, 1);
  lcd.print("Dentro il Filo?");
  Outside_Wire_Count = 0;
  //FindWireTrack();
}
