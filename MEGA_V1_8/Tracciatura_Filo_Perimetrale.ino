//  Stampa una visualizzazione del tracciamento del filo nel monitor seriale
//  per vedere come viene seguito il filo. Regolazione del valore P nelle impostazioni
//  può migliorare la capacità di tracciamento del filo del tosaerba.

void PrintWirePosition() {
  int PrintMAG_Now = MAG_Now / Scale;
  int PrintInMax  = InMax  / Scale;
  int PrintInMid  = InMid  / Scale;
  int PrintInMin  = InMin  / Scale;
  int PrintOutMin = OutMin / Scale;
  int PrintOutMid = OutMid / Scale;
  int PrintOutMax = OutMax / Scale;

  for (int i = PrintInMax; i <= PrintOutMax; i++) {
    if (i == PrintInMax)  Serial.print(F("|"));
    if (i == PrintInMid)  Serial.print(F("|"));
    if (i == PrintInMin)  Serial.print(F("|"));
    if (i == PrintOutMin) Serial.print(F("|"));
    if (i == PrintOutMid) Serial.print(F("|"));
    if (i == PrintOutMax) Serial.print(F("|"));
    if (i == PrintMAG_Now) Serial.print(F("X"));     // posizione attuale (media L/R)
    if (i == 0) Serial.print(F("0"));
    else Serial.print(F("."));
  }
  Serial.print(F("|MAG_Now:"));  Serial.print(MAG_Now); Serial.print(F("|"));
}



// Funzione per seguire il filo per un periodo di tempo specifico impostato dalle iterazioni 'I'
// Usa due sensori: centratura = (magL - magR), media per visualizzazione
void Track_Wire_From_Dock_to_Zone_X() {
  delay(100);
  ADCMan.run();
  lcd.setCursor(0, 0);
  lcd.print("Uscita Garage x");
  lcd.setCursor(2, 1);
  if (Exit_Zone == 1) lcd.print(" Zona1");
  if (Exit_Zone == 2) lcd.print(" Zona2");
  if (Exit_Zone == 3) lcd.print(" Zona3");
  delay(1000);

  if (Blade_Override == 1) Motor_Action_Spin_Blades();
  
  Tracking_Turn_Right = 0;
  Tracking_Turn_Left  = 0;
  
  Serial.print(F("P = ")); Serial.print(P);
  Serial.print(F(" / D = ")); Serial.print(D);
  Serial.println(F(""));

  Tracking_Wire = 1;
  Mower_Running = 0;

  MAG_Now   = 0;
  MAG_Start = 0;
  MAG_Error = 0;
  MAG_Goal  = 0;

  int Dock_Cycles = 0;

  delay(500);
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(5);
  lcd.setCursor(10,1); lcd.print("0");

  for (I = 0; I < Track_Wire_Itterations; I++) {
    if (Mower_Parked == 0) {
      delay(5);
      ADCMan.run();

      // === DUE SENSORI: lettura e centratura ===
      int magL = perimeter.getMagnitude(0);   // SX
      int magR = perimeter.getMagnitude(1);   // DX
      if (magL > 100)  magL = 100;
      if (magL < -100) magL = -100;
      if (magR > 100)  magR = 100;
      if (magR < -100) magR = -100;

      MAG_Now   = (magL + magR) / 2;          // media solo per display
      MAG_Error = (magL - magR);              // differenza per centratura: >0 filo più verso SX → sterza DX

      // Stato dentro/fuori (istantaneo) per logiche Boost
      UpdateWireSensor();        // aggiorna inside  (SX)
      UpdateWireSensorRight();   // aggiorna insideR (DX)
      bool bothOut = (!inside && !insideR);
      bool bothIn  = ( inside &&  insideR);

      delay(5);
      PrintWirePosition();

      // =======================
      // CCW (antiorario) verso zona di uscita
      // Esterno considerato a DESTRA → bothIn => curva verso DESTRA (out), bothOut => curva verso SINISTRA (in)
      // =======================
      if (CCW_Tracking_To_Start == 1) {
        if (MAG_Error > 0) {   // filo più forte a SX → GIRA A DESTRA
          PWM_Left  = PWM_TrackSpeed_RH;
          PWM_Right = PWM_TrackSpeed_RH - (MAG_Error * P);
          if (PWM_Right >  PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) { SetPins_ToGoForwards(); lcd.setCursor(15, 1); lcd.print(" "); }

          // Boost DX: PWM basso OPPURE entrambi DENTRO (bothIn ⇒ spingi verso l'esterno=DX)
          if (Boost_Turn == 1) {
            if ((PWM_Right < Min_Track_PWM) || bothIn) {
              Serial.print(F("XX"));
              SetPins_ToTurnRight();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
          }

          // rotazione stretta (ruote opposte) se PID richiede PWM negativo
          if (PWM_Right < 0) {
            PWM_Right = (-1 * PWM_Right) + 20;
            if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
            if (PWM_Right >= 0) SetPins_ToTurnRight();
            delay(5);
            lcd.setCursor(15, 1); lcd.print(">");
          }
          
          Motor_Action_Dynamic_PWM_Steering();
          Serial.print(F(" Turn Right "));
          Tracking_Turn_Left = 0;
          Tracking_Turn_Right++;
          if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {
            Motor_Action_Stop_Motors();
            lcd.clear(); lcd.print("Ruota DX");
            lcd.setCursor(0,1); lcd.print("Tracking_Turn_Right");
            delay(1180); lcd.clear();
            Tracking_Restart_Blocked_Path();
          }

        } else { // MAG_Error <= 0 → filo più verso DX → GIRA A SINISTRA
          PWM_Right = PWM_TrackSpeed_LH;
          PWM_Left  = PWM_TrackSpeed_LH + (MAG_Error * P);
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
          if (PWM_Left >= 0) { SetPins_ToGoForwards(); lcd.setCursor(0, 1); lcd.print(" "); }

          // Boost SX: PWM basso OPPURE entrambi FUORI (bothOut ⇒ spingi verso l'interno=SX)
          if (Boost_Turn == 1) {
            if ((PWM_Left < Min_Track_PWM) || bothOut) {
              Serial.print(F("XX"));
              SetPins_ToTurnLeft();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
          }

          if (PWM_Left < 0) {
            PWM_Left = (-1 * PWM_Left) + 20;
            if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
            SetPins_ToTurnLeft();
            delay(5);
            lcd.setCursor(0, 1); lcd.print("<");
          }
          
          Motor_Action_Dynamic_PWM_Steering();
          Serial.print(F(" Turn Left "));
          Tracking_Turn_Right = 0;
          Tracking_Turn_Left++;
          if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
            Motor_Action_Stop_Motors();
            lcd.clear(); lcd.print("Ruota SX");
            lcd.setCursor(0,1); lcd.print("Tracking_Turn_Left");
            delay(1180); lcd.clear();
            Tracking_Restart_Blocked_Path();
          }
        }

        Serial.print(F(" : MAG_Error=")); Serial.println(MAG_Error);
        Dock_Cycles++;
        Loop_Cycle_Mowing = I;
        if (Dock_Cycles > 10) {
          Tracking_Wire = Tracking_Wire + 1;
          if (Tracking_Wire > 1) Tracking_Wire = 0;
          if (WIFI_Enabled == 1) Get_WIFI_Commands();
          Dock_Cycles = 0;
        }
        lcd.setCursor(10,1); lcd.print(I);
        Check_Bumper();
        if ((Bump_Frnt_LH == true) || (Bump_Frnt_RH == true)) {
          Compass_Turn_Mower_To_Home_Direction();
          Manouver_Find_Wire_Track();
        }
      } // fine CCW

      // =======================
      // CW (orario) verso zona di uscita
      // Esterno considerato a SINISTRA → bothIn => curva verso SINISTRA, bothOut => curva verso DESTRA
      // =======================
      if (CW_Tracking_To_Start == 1) {
        if (MAG_Error > 0) { // filo più verso SX → GIRA A DESTRA
          PWM_Left  = PWM_TrackSpeed_RH;
          PWM_Right = PWM_TrackSpeed_RH - (MAG_Error * P);
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) { SetPins_ToGoForwards(); lcd.setCursor(15, 0); lcd.print(" "); }

          // Boost DX: PWM basso OPPURE entrambi FUORI (CW inward = DX)
          if (Boost_Turn == 1) {
            if ((PWM_Right < Min_Track_PWM) || bothOut) {
              Serial.print(F("XX"));
              SetPins_ToTurnRight();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
          }

          if (PWM_Right < 0) {
            PWM_Right = (-1 * PWM_Right) + 20;
            if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
            if (PWM_Right >= 0) SetPins_ToTurnRight();
            lcd.setCursor(15, 0); lcd.print("*");
          }
          
          Motor_Action_Dynamic_PWM_Steering();
          Serial.print(F(" Turn Right "));
          Tracking_Turn_Left = 0;
          Tracking_Turn_Right++;
          if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {
            Motor_Action_Stop_Motors();
            Tracking_Restart_Blocked_Path();
          }

        } else { // MAG_Error <= 0 → GIRA A SINISTRA
          PWM_Right = PWM_TrackSpeed_LH;
          PWM_Left  = PWM_TrackSpeed_LH + (MAG_Error * P);
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
          if (PWM_Left >= 0) { SetPins_ToGoForwards(); lcd.setCursor(15, 0); lcd.print(" "); }

          // Boost SX: PWM basso OPPURE entrambi DENTRO (CW outward = SX)
          if (Boost_Turn == 1) {
            if ((PWM_Left < Min_Track_PWM) || bothIn) {
              Serial.print(F("XX"));
              SetPins_ToTurnLeft();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
          }

          if (PWM_Left < 0) {
            PWM_Left = (-1 * PWM_Left) + 20;
            if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
            if (PWM_Left >= 0) SetPins_ToTurnLeft();
            delay(5);
            lcd.setCursor(15, 0); lcd.print("*");
          }
          
          Motor_Action_Dynamic_PWM_Steering();
          Serial.print(F(" Turn Left "));
          Tracking_Turn_Right = 0;
          Tracking_Turn_Left++;
          if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
            Motor_Action_Stop_Motors();
            Tracking_Restart_Blocked_Path();
          } 
        }

        Serial.print(F(" : MAG_Error=")); Serial.println(MAG_Error);
        Dock_Cycles++;
        Loop_Cycle_Mowing = I;
        if (Dock_Cycles > 10) {
          Tracking_Wire = Tracking_Wire + 1;
          if (Tracking_Wire > 1) Tracking_Wire = 0;
          if (WIFI_Enabled == 1) Get_WIFI_Commands();
          Dock_Cycles = 0;
        }
        lcd.setCursor(10,1); lcd.print(I);
        Check_Bumper();
        if ((Bump_Frnt_LH == true) || (Bump_Frnt_RH == true)) {
          Compass_Turn_Mower_To_Home_Direction();
          Manouver_Find_Wire_Track();
        }
      } // fine CW

    } // Mower_Parked == 0
  }   // for I

  lcd.clear();
  Tracking_Wire = 0;
  Loop_Cycle_Mowing = 0;
}



// Traccia il cavo perimetrale fino alla base (ritorno).
// Due sensori + Boost simmetrico come sopra.
void Track_Perimeter_Wire_To_Dock() {
  lcd.clear();
  lcd.print("Ritorno al Dock");
  lcd.setCursor(0, 1);
  lcd.print("Seguendo il filo");

  Docked_Hits = 0;
  Check_if_Charging();
  Check_if_Docked();
  if (WIFI_Enabled == 1) Get_WIFI_Commands();

  Serial.println(F("Tracking the wire back to the Garage: "));
  Serial.print(F("P_Return = ")); Serial.print(P_Return);
  Serial.print(F(" / D = "));     Serial.print(D);
  Serial.println(F(""));

  Tracking_Wire = 1;
  Loop_Cycle_Mowing = 0;
  Mower_Running = 0;

  MAG_Now   = 0;
  MAG_Start = 0;
  MAG_Error = 0;
  MAG_Goal  = 0;
  int Dock_Cycles = 0;
  delay(5);

  // =======================
  // CCW verso base (esterno a DESTRA)
  // =======================
  if (CCW_Tracking_To_Charge == 1) {
    Serial.println(F("TRACKING COUNTER-CLOCKWISE"));
    while ((Mower_Docked == 0) && (Mower_Parked == 0)) {
      ADCMan.run();

      int magL = perimeter.getMagnitude(0);
      int magR = perimeter.getMagnitude(1);
      if (magL > 100)  magL = 100;
      if (magL < -100) magL = -100;
      if (magR > 100)  magR = 100;
      if (magR < -100) magR = -100;

      MAG_Now   = (magL + magR) / 2;
      MAG_Error = (magR - magL);   // inversione per ritorno CCW
      delay(5);

      UpdateWireSensor();
      UpdateWireSensorRight();
      bool bothOut = (!inside && !insideR);
      bool bothIn  = ( inside &&  insideR);

      // === stessa logica dell’uscita, speculare ===
      if (MAG_Error > 0) {   // filo più verso DX → gira DX
        PWM_Left  = PWM_TrackSpeed_RH;
        PWM_Right = PWM_TrackSpeed_RH - (MAG_Error * P_Return);
        if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
        if (PWM_Right >= 0) { SetPins_ToGoForwards(); }

        if (Boost_Turn == 1) {
          if ((PWM_Right < Min_Track_PWM) || bothIn) {
            SetPins_ToTurnRight();
            Motor_Action_Go_Track_Speed();
            delay(Hard_Track_Turn_Delay);
            SetPins_ToGoForwards();
          }
        }

        if (PWM_Right < 0) {
          PWM_Right = (-1 * PWM_Right) + 20;
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) SetPins_ToTurnRight();
        }

        Motor_Action_Dynamic_PWM_Steering();
        Tracking_Turn_Left = 0;
        Tracking_Turn_Right++;
        if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {
          Tracking_Restart_Blocked_Path();
        }

      } else { // filo più verso SX → gira SX
        PWM_Right = PWM_TrackSpeed_LH;
        PWM_Left  = PWM_TrackSpeed_LH + (MAG_Error * P_Return);
        if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
        if (PWM_Left >= 0) { SetPins_ToGoForwards(); }

        if (Boost_Turn == 1) {
          if ((PWM_Left < Min_Track_PWM) || bothOut) {
            SetPins_ToTurnLeft();
            Motor_Action_Go_Track_Speed();
            delay(Hard_Track_Turn_Delay);
            SetPins_ToGoForwards();
          }
        }

        if (PWM_Left < 0) {
          PWM_Left = (-1 * PWM_Left) + 20;
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
          if (PWM_Left >= 0) SetPins_ToTurnLeft();
        }

        Motor_Action_Dynamic_PWM_Steering();
        Tracking_Turn_Right = 0;
        Tracking_Turn_Left++;
        if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
          Tracking_Restart_Blocked_Path();
          if (Mower_Parked == 1) Mower_Docked = 1;
        }
      }

      Serial.print(F(" : MAG_Error=")); Serial.println(MAG_Error);

      Read_Serial1_Nano();
      Check_if_Charging();
      Check_if_Docked();
      Dock_Cycles++;
      if (Dock_Cycles > 10) {
        Tracking_Wire = Tracking_Wire + 1;
        if (Tracking_Wire > 1) Tracking_Wire = 0;
        Mower_Running = 0;
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Dock_Cycles = 0;
      }
      if (Loop_Cycle_Mowing == Cycles_Max) Manouver_Cycles_Max();
      Check_Wheel_Amps();
      if (Wheel_Blocked == 4) {
        Compass_Turn_Mower_To_Home_Direction();
        Manouver_Find_Wire_Track();
      }
    }
    Loop_Cycle_Mowing = 0;
  }

  // =======================
  // CW verso base (esterno a SINISTRA)
  // =======================
  if (CW_Tracking_To_Charge == 1) {
    Serial.println(F("TRACKING CLOCKWISE"));
    while ((Mower_Docked == 0) && (Mower_Parked == 0)) {
      ADCMan.run();

      int magL = perimeter.getMagnitude(0);
      int magR = perimeter.getMagnitude(1);
      if (magL > 100)  magL = 100;
      if (magL < -100) magL = -100;
      if (magR > 100)  magR = 100;
      if (magR < -100) magR = -100;

      MAG_Now   = (magL + magR) / 2;
      MAG_Error = (magL - magR);   // inversione per ritorno CW
      delay(5);

      UpdateWireSensor();
      UpdateWireSensorRight();
      bool bothOut = (!inside && !insideR);
      bool bothIn  = ( inside &&  insideR);

      if (MAG_Error > 0) {   // filo più verso SX → gira DX
        PWM_Left  = PWM_TrackSpeed_RH;
        PWM_Right = PWM_TrackSpeed_RH - (MAG_Error * P_Return);
        if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
        if (PWM_Right >= 0) { SetPins_ToGoForwards(); }

        if (Boost_Turn == 1) {
          if ((PWM_Right < Min_Track_PWM) || bothOut) {
            SetPins_ToTurnRight();
            Motor_Action_Go_Track_Speed();
            delay(Hard_Track_Turn_Delay);
            SetPins_ToGoForwards();
          }
        }

        if (PWM_Right < 0) {
          PWM_Right = (-1 * PWM_Right) + 20;
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) SetPins_ToTurnRight();
        }

        Motor_Action_Dynamic_PWM_Steering();
        Tracking_Turn_Left = 0;
        Tracking_Turn_Right++;
        if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {
          Tracking_Restart_Blocked_Path();
        }

      } else { // filo più verso DX → gira SX
        PWM_Right = PWM_TrackSpeed_LH;
        PWM_Left  = PWM_TrackSpeed_LH + (MAG_Error * P_Return);
        if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
        if (PWM_Left >= 0) { SetPins_ToGoForwards(); }

        if (Boost_Turn == 1) {
          if ((PWM_Left < Min_Track_PWM) || bothIn) {
            SetPins_ToTurnLeft();
            Motor_Action_Go_Track_Speed();
            delay(Hard_Track_Turn_Delay);
            SetPins_ToGoForwards();
          }
        }

        if (PWM_Left < 0) {
          PWM_Left = (-1 * PWM_Left) + 20;
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
          if (PWM_Left >= 0) SetPins_ToTurnLeft();
        }

        Motor_Action_Dynamic_PWM_Steering();
        Tracking_Turn_Right = 0;
        Tracking_Turn_Left++;
        if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
          Tracking_Restart_Blocked_Path();
        }
      }

      Serial.print(F(" : MAG_Error=")); Serial.println(MAG_Error);

      Read_Serial1_Nano();
      Check_if_Charging();
      Check_if_Docked();
      Dock_Cycles++;
      if (Dock_Cycles > 10) {
        Tracking_Wire = Tracking_Wire + 1;
        if (Tracking_Wire > 1) Tracking_Wire = 0;
        Mower_Running = 0;
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Dock_Cycles = 0;
      }
      if (Loop_Cycle_Mowing == Cycles_Max) Manouver_Cycles_Max();
      Check_Wheel_Amps();
      if (Wheel_Blocked == 4) {
        Compass_Turn_Mower_To_Home_Direction();
        Manouver_Find_Wire_Track();
      }
    }
    Loop_Cycle_Mowing = 0;
  }

  if (Mower_Docked == 1) Tracking_Wire = 0;
  Loop_Cycle_Mowing = 0;
}




// Avvia un algoritmo per ritrovare il filo dopo averlo perso nel tracciamento
void Tracking_Restart_Blocked_Path() {
  Motor_Action_Stop_Motors();
  Serial.println(F(""));
  Serial.println(F("Possible Blocked Path - Trying to Avoid"));
  Serial.println(F(""));
  Mower_Running = 1;
  Tracking_Wire = 1;
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(1000);
  Mower_Running = 0;
  Tracking_Wire = 0;
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(1000);
  Mower_Running = 1;
  Tracking_Wire = 1;
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(1000);
  Mower_Running = 0;
  Tracking_Wire = 0;
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(1000);
  
  lcd.clear();
  lcd.print("Perso il Filo.");
  lcd.setCursor(0, 1);
  lcd.print("Recupero.....");
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if (Mower_Parked != 1) {
    if (WIFI_Enabled == 1) Get_WIFI_Commands();
    SetPins_ToGoBackwards();
    delay(180);
    Motor_Action_Go_Accel(600);
    Motor_Action_Stop_Motors();
    delay(1000);
    Tracking_Turn_Left  = 0;
    Tracking_Turn_Right = 0;
    delay(180);
    Mower_Running = 0;
    Tracking_Wire = 0;
    //if (Compass_Activate == 1) Compass_Turn_Mower_To_Home_Direction();
    Manouver_Find_Wire_Track();
  }
}
