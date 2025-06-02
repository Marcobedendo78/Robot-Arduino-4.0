//  Stampa una visualizzazione del tracciamento del filo nel monitor seriale
//  per vedere come viene seguito il filo. Regolazione del valore P nelle impostazioni
//  può migliorare la capacità di tracciamento del filo del tosaerba.


void PrintWirePosition() {
  int PrintMAG_Now = MAG_Now / Scale;
  int PrintInMax = InMax / Scale;
  int PrintInMid = InMid / Scale;
  int PrintInMin = InMin / Scale;
  int PrintOutMin = OutMin / Scale;
  int PrintOutMid = OutMid / Scale;
  int PrintOutMax = OutMax / Scale;


  for (int i = PrintInMax; i <= PrintOutMax; i++) {
    if (i == PrintInMax) Serial.print(F("|"));
    if (i == PrintInMid) Serial.print(F("|"));
    if (i == PrintInMin) Serial.print(F("|"));
    if (i == PrintOutMin) Serial.print(F("|"));
    if (i == PrintOutMid) Serial.print(F("|"));
    if (i == PrintOutMax) Serial.print(F("|"));
    if (i == PrintMAG_Now) Serial.print(F("X"));     //maybe change to MAG_Last...
    if (i == 0) Serial.print(F("0"));

    else Serial.print(F("."));

  }
  Serial.print(F("|MAG_Now:"));  Serial.print(MAG_Now); Serial.print(F("|"));
}



// Funzione per seguire il filo per un periodo di tempo specifico impostato dalle iterazioni 'I'
// Il robot segue il filo fino all'esaurimento delle iterazioni.
void Track_Wire_From_Dock_to_Zone_X() {
  delay(100);
  ADCMan.run();
  lcd.setCursor(0, 0);
  lcd.print("Uscita Garage x");                                             // nel giardino in una buona posizione per iniziare la falciatura
  lcd.setCursor(2, 1);
  if (Exit_Zone == 1) lcd.print(" Zona1");
  if (Exit_Zone == 2) lcd.print(" Zona2");
  if (Exit_Zone == 3) lcd.print(" Zona3");
  delay(1000);                                                              // Stampa le informazioni sul display LCD

  if (Blade_Override == 1) {                                              
        Motor_Action_Spin_Blades();
        }
  
  Tracking_Turn_Right = 0;                                                  // azzera gli errori di tracciamento per LH e RH.
  Tracking_Turn_Left = 0;
  
  //utilizza le impostazioni PID nel setup
  Serial.print(F("P = "));
  Serial.print(P);
  Serial.print(F(" / D = "));
  Serial.print(D);
  Serial.println(F(""));
  Tracking_Wire = 1;
  Mower_Running = 0;
  MAG_Now = 0;                                                              // Reset Valori
  MAG_Start = 0;
  MAG_Error = 0;
  MAG_Goal = 0;
  int Dock_Cycles = 0;
  delay(500);
  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  delay(5);
  lcd.setCursor(10,1);
  lcd.print("0");
  for (I = 0; I < Track_Wire_Itterations; I++) {                            // Iterazioni 'I' prima che il tosaerba lasci il cavo.
      if (Mower_Parked == 0) {
      delay(5);
      ADCMan.run();
      MAG_Start = perimeter.getMagnitude(0);                                // Ottiene la potenza del segnale del sensore
      if (MAG_Start> 100) MAG_Start = 100;                                   // Prima non c'era 30/04/2024 (100)
      if (MAG_Start< -100) MAG_Start = -100;                                 // Prima non c'era 30/04/2024 (-100)
      MAG_Now = MAG_Start;
      delay(5);
      MAG_Error = (MAG_Goal - MAG_Start);                                   // Calcola l'errore al centro del filo che normalmente è di magnitudine zero
      PrintWirePosition();                                                  // Stampa la panoramica sul monitor seriale.

 
      // Segue il cavo dalla docking station in senso antiorario fino alla posizione iniziale

      if (CCW_Tracking_To_Start == 1) {
      if (MAG_Error > 0) {                                                   // Se MAG_Error > 0, gira a destra per CCW Tracking. PWM_left è impostato su max per girare a destra.
        // GIRARE A DESTRA
        PWM_Left = PWM_TrackSpeed_RH;                                        // imposta il PWM al massimo possibile per la ruota
        PWM_Right = PWM_TrackSpeed_RH - (MAG_Error * P);                     // Mag_Error * P è il valore ridotto dal valore massimo impostato PWM e inviato al PWM
        if (PWM_Right > PWM_TrackSpeed_RH)  PWM_Right = PWM_TrackSpeed_RH;   // PWM_Right limitato a Max PWM di 120 e PID 0.08.
        if (PWM_Right >= 0) {
          SetPins_ToGoForwards();
          lcd.setCursor(15, 1);
          lcd.print(" ");
          }


         //Svolta curva stretta DX
         if (Boost_Turn == 1) {
            if (PWM_Right < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnRight();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }
         

        if (PWM_Right < 0) {                                                // imposta il tosaerba in una svolta brusca a destra (le ruote girano in senso opposto) se l'errore sul cavo è sufficientemente grande.
          PWM_Right = (-1 * PWM_Right) + 20 ;                               // Prima era 220 23/04/2022
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) SetPins_ToTurnRight();                       
          delay(5);
          lcd.setCursor(15, 1);
          lcd.print(">");
          }
        
        Motor_Action_Dynamic_PWM_Steering();                                 // Effettua le modifiche PWM della ruota per sterzare sul filo
        Serial.print(F(" Turn Right "));
        Tracking_Turn_Left = 0;                                              // Failsafe se il tosaerba perde il filo. Se al tosaerba viene comandato di girare a sinistra o a destra
        Tracking_Turn_Right = Tracking_Turn_Right + 1;                       // troppe volte si presume che il tosaerba stia girando e non possa tornare sul filo.
        if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {                 // se questo viene rilevato, viene eseguita una funzione per trovare di nuovo il filo.
          Motor_Action_Stop_Motors();
          lcd.clear();
          lcd.print("Ruota DX");
          lcd.print("Tracking_Turn_Right");
          delay(1180);
          lcd.clear();
          Tracking_Restart_Blocked_Path();
        }

      }
      if (MAG_Error <= 0) {                                                 // Se MAG_Error < 0, girare a sinistra per CCW Tracking
        // GIRARE A SINISTRA
        PWM_Right = PWM_TrackSpeed_LH;                                      // PWM_Right impostato su max per ruotare il tosaerba a sinistra.
        PWM_Left = PWM_TrackSpeed_LH + (MAG_Error * P);                     // + poiché mag_error è negativo per regolare PWM
        if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;     // PWM_Left limitato a mex PWM di 120
        if (PWM_Left >= 0) {
          SetPins_ToGoForwards();   
          lcd.setCursor(0, 1);
          lcd.print(" "); 
          }



          //Svolta curva stretta SX
         if (Boost_Turn == 1) {
            if (PWM_Left < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnLeft();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }


        if (PWM_Left < 0) {                                                 // imposta il tosaerba su una svolta brusca a sinistra (le ruote girano in senso opposto) se l'errore sul cavo è abbastanza grande.
          PWM_Left = (-1 * PWM_Left) + 20;                                  // Prima era 220 23/04/2022
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
          SetPins_ToTurnLeft();
          delay(5);
          lcd.setCursor(0, 1);
          lcd.print("<");
          }
        
        Motor_Action_Dynamic_PWM_Steering();
        Serial.print(F(" Turn Left "));
        Tracking_Turn_Right = 0;
        Tracking_Turn_Left = Tracking_Turn_Left + 1;
        if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
          Motor_Action_Stop_Motors();
          lcd.clear();
          lcd.print("Ruota SX");
          lcd.print("Tracking_Turn_Left");
          delay(1180);
          lcd.clear();
          Tracking_Restart_Blocked_Path();
        }
      }
      Serial.print(F(" : MAG_Error="));
      Serial.println(MAG_Error);
      Dock_Cycles = Dock_Cycles + 1;
      Loop_Cycle_Mowing = I;
      if (Dock_Cycles > 10) {
        Tracking_Wire = Tracking_Wire + 1;                                   // Fa lampeggiare il LED di tracciamento del filo nell'app.
        if (Tracking_Wire > 1) Tracking_Wire = 0;
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Dock_Cycles = 0;
        
        }
      }
    }
  

  if (CW_Tracking_To_Start == 1) {
      // Aggiungi qui il codice per il monitoraggio CW nella zona di uscita.
      // Usa il codice sopra per il tracciamento in senso antiorario verso la docking station
      // come modello.
    if (MAG_Error > 0) {                                                     // Se MAG_Error > 0, girare a sinistra per CW Tracking                      
        //  GIRARE A SINISTRA
        PWM_Right = PWM_TrackSpeed_LH;                                       // Set the right wheel to max PWMto turn left
        PWM_Left = PWM_TrackSpeed_LH - (MAG_Error * P);
        if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;      // 
        if (PWM_Left >= 0) {
          SetPins_ToGoForwards();                                            // keep the mower moving forward
          lcd.setCursor(15, 0);
          lcd.print(" ");
          }

        //Svolta curva stretta SX
         if (Boost_Turn == 1) {
            if (PWM_Right < Min_Track_PWM) {
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
          lcd.setCursor(15, 0);
          lcd.print("*");
          SetPins_ToTurnLeft();
          delay(5);
          }
        
        Motor_Action_Dynamic_PWM_Steering();                                   // send the PWM values to the motor driver.
        Serial.print(F(" Turn Left "));
        Tracking_Turn_Right = 0;
        Tracking_Turn_Left = Tracking_Turn_Left + 1;
        if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
          Motor_Action_Stop_Motors();
          Tracking_Restart_Blocked_Path();
        }
      }
      if (MAG_Error <= 0) {                                                    // Turn the Mower to the right to get back on the wire.
        // GIRARE A DESTRA
        PWM_Left = PWM_TrackSpeed_RH;
        PWM_Right = PWM_TrackSpeed_RH + (MAG_Error * P);                       // + as mag_error is negative to adjust PWM
        if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
        if (PWM_Right >= 0) {
          SetPins_ToGoForwards();
          lcd.setCursor(15, 0);
          lcd.print(" ");
          }

         //Svolta curva stretta DX
         if (Boost_Turn == 1) {
            if (PWM_Right < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnRight();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }

        if (PWM_Right < 0) {
          PWM_Right = (-1 * PWM_Right) + 20 ;
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) SetPins_ToTurnRight();                                    
          lcd.setCursor(15, 0);
          lcd.print("*");
        }

        Motor_Action_Dynamic_PWM_Steering();
        Serial.print(F(" Turn Right "));
        Tracking_Turn_Left = 0;
        Tracking_Turn_Right = Tracking_Turn_Right + 1;
        if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {
          Motor_Action_Stop_Motors();
          Tracking_Restart_Blocked_Path();
          } 
    }
      Serial.print(F(" : MAG_Error="));
      Serial.println(MAG_Error);
      Dock_Cycles = Dock_Cycles + 1;
      Loop_Cycle_Mowing = I;
      if (Dock_Cycles > 10) {
        Tracking_Wire = Tracking_Wire + 1;                                              // Makes the wire tracking LED in the app blink.
        if (Tracking_Wire > 1) Tracking_Wire = 0;
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Dock_Cycles = 0;
      }
    }
    lcd.setCursor(10,1);
    lcd.print(I);
    Check_Bumper();
    if ((Bump_Frnt_LH == true) || (Bump_Frnt_RH == true)) {
      Compass_Turn_Mower_To_Home_Direction();                                             // Nuova aggiunta 15/06/2024
      Manouver_Find_Wire_Track(); 
    }
  } 
   
  lcd.clear();
  Tracking_Wire = 0;
  Loop_Cycle_Mowing = 0;

delay(5);


}


// Traccia il cavo perimetrale utilizzando un metodo di tipo PID
// Questo codice traccia il cavo perimetrale e calcolando l'ampiezza del perimetro e calcolando la distanza dal centro del cavo.
// Questo valore di errore di (posizione - centro del filo) viene moltiplicato per il valore P nella configurazione per inviare una modifica PWM a sinistra o a destra
// ruota per riportare il sensore sul filo.


void Track_Perimeter_Wire_To_Dock()  {
  lcd.clear();
  lcd.print("Puntamento Per..");
  lcd.setCursor(0, 1);
  lcd.print("Base di ricarica");                                               // Stampa le informazioni sul display LCD
    Docked_Hits = 0;
  Check_if_Charging();                                                         // Verifica se viene rilevato un amperaggio sul cavo di ricarica
  Check_if_Docked();

  Serial.println(F(" Tracking the wire to the Garage: "));                     // Stampa i valori PID utilizzati.
  Serial.print(F("P = "));
  Serial.print(P);
  Serial.print(F(" / D = "));
  Serial.print(D);
  Serial.println(F(""));

  Tracking_Wire = 1;
  Loop_Cycle_Mowing = 0;
  Mower_Running = 0;
  
  MAG_Now = 0;                                                                  // Reset Valori
  MAG_Start = 0;
  MAG_Error = 0;
  MAG_Goal = 0;
  int Dock_Cycles = 0;
  delay(5);


  
  if (CCW_Tracking_To_Charge == 1)  {                                            // Il tosaerba segue il filo in senso antiorario
    Serial.println(F("TRACKING COUNTER-CLOCKWISE"));
    while ((Mower_Docked == 0) && (Mower_Parked == 0)) {
      lcd.clear();
      lcd.print("Puntamento CCW X");
      lcd.setCursor(0, 1);
      lcd.print("Base di Ricarica");                                             // Stampa le informazioni sul display LCD
      lcd.clear();
      lcd.setCursor(13, 1);
      lcd.print(Loop_Cycle_Mowing);                                              // Stampa il numero ci cicli sul display LCD
      delay(5);
      ADCMan.run();
      MAG_Start = perimeter.getMagnitude(0);                                     // Ottiene la potenza del segnale del sensore
      if (MAG_Start> 90) MAG_Start = 90;                                         // Prima non c'era 30/04/2024 (100)
      if (MAG_Start< -90) MAG_Start = -90;                                       // Prima non c'era 30/04/2024 (-100)
      MAG_Now = MAG_Start;
      delay(5);
      MAG_Error = (MAG_Goal - MAG_Start);                                        // Calcola l'errore al centro del filo che normalmente è di magnitudine zero (ricorda - - è + )
      PrintWirePosition();                                                       // Stampa la panoramica sul monitor seriale.
      Loop_Cycle_Mowing = Loop_Cycle_Mowing + 1;

      

      if (MAG_Error > 0) {                                                        // Ruotare il tosaerba a destra di MAG Error > 0 con una direzione di marcia antioraria.
        // GIRA A DX
        PWM_Left = PWM_TrackSpeed_RH;                                             // Imposta PWM_Sinistra al massimo
        PWM_Right = PWM_TrackSpeed_RH - (MAG_Error * P);                          // Mag_Error * P è il valore ridotto dal valore massimo impostato PWM e inviato al PWM                 
        if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;         // Limita il PWM_Right a 120
        if (PWM_Right >= 0) {
          SetPins_ToGoForwards();
          lcd.setCursor(15, 0);
          lcd.print(" ");
          }


          //Svolta curva stretta DX
         if (Boost_Turn == 1) {
            if (PWM_Right < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnRight();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }
       
        
        if (PWM_Right < 0) {                                                      // Impostare le ruote in modo che ruotino attorno all'asse se è necessaria una svolta brusca.
          PWM_Right = (-1 * PWM_Right) + 20;                                      // cambiare il valore negativo in positivo per l'ingresso PWM al ponte motore. // Prima era 20 05/05/2024
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;       // limitare il PWM massimo a 120
          if (PWM_Right >= 0) SetPins_ToTurnRight();                              // impostare i perni del ponte motore per girare a sinistra
          lcd.setCursor(15, 0);
          lcd.print("*");
          }
       
        Motor_Action_Dynamic_PWM_Steering();                                      // Effettua le modifiche PWM della ruota per sterzare sul filo
        Serial.print(F(" Turn Right "));
        Tracking_Turn_Left = 0;                                                   // Failsafe se il tosaerba perde il filo. Se al tosaerba viene comandato di girare a sinistra o a destra
        Tracking_Turn_Right = Tracking_Turn_Right + 1;                            // troppe volte si presume che il tosaerba stia girando e non possa tornare sul filo.
        if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {                      // se questo viene rilevato, viene eseguita una funzione per trovare di nuovo il filo.
          Tracking_Restart_Blocked_Path();
        }

      }
      if (MAG_Error <= 0) {                                                       // Girare il tosaerba a sinistra se MAG_Error < 0 con una direzione di marcia antiorariaon
        //GIRA A SX
        PWM_Right = PWM_TrackSpeed_LH;                                            // Imposta PWM_Right al massimo
        PWM_Left = PWM_TrackSpeed_LH + (MAG_Error * P);                           // + poiché mag_error è negativo per regolare PWM
        if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;           // cap PWM_Left al massimo
        if (PWM_Left >= 0) {
          SetPins_ToGoForwards();
          lcd.setCursor(15, 0);
          lcd.print(" ");
          }



          //Svolta curva stretta SX
         if (Boost_Turn == 1) {
            if (PWM_Left < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnLeft();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }
        

        if (PWM_Left < 0) {                                                        // se il PWM calcolato è inferiore a zero, impostare il rasaerba per una svolta brusca
          PWM_Left = (-1 * PWM_Left) + 20;                                         // cambiare il valore negativo in positivo per l'ingresso PWM al ponte motore. // Prima era 20 05/05/2024
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;          // se di nuovo il PWM è superiore a 120, limitalo a 120
          if (PWM_Left >= 0) SetPins_ToTurnLeft();                                 // impostare i perni in modo brusco girare a sinistra  
          delay(5);
          lcd.setCursor(15, 0);
          lcd.print("*");
          }
          
        Motor_Action_Dynamic_PWM_Steering();
        Serial.print(F(" Turn Left "));
        Tracking_Turn_Right = 0;
        Tracking_Turn_Left = Tracking_Turn_Left + 1;
        if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
          Tracking_Restart_Blocked_Path();
          if (Mower_Parked == 1) Mower_Docked = 1;
        }
      }
      Serial.print(F(" : MAG_Error="));
      Serial.print(MAG_Error);
      Read_Serial1_Nano();
      Check_if_Charging();
      Check_if_Docked();
      Dock_Cycles = Dock_Cycles + 1;
      if (Dock_Cycles > 10) {
        Tracking_Wire = Tracking_Wire + 1;                                          // Fa lampeggiare il LED di tracciamento del filo nell'app.
        if (Tracking_Wire > 1) Tracking_Wire = 0;
        Mower_Running = 0;
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Dock_Cycles = 0;
        }
        if (Loop_Cycle_Mowing == Cycles_Max) Manouver_Cycles_Max();                 // Manovra per sganciari da un'aiuola e ripetere la ricerca del filo peimetrale
       }
  
  Loop_Cycle_Mowing = 0;  
  }


      
  if (CW_Tracking_To_Charge == 1)  {                                                // Il tosaerba segue il filo in senso antiorario
    Serial.println(F("TRACKING ---  CLOCKWISE"));                                   // Con le stesse funzioni di cui sopra
    while ((Mower_Docked == 0) && (Mower_Parked == 0)) {
      lcd.clear();
      lcd.print("Puntamento CW X");
      lcd.setCursor(0, 1);
      lcd.print("Base di Ricarica");                                                // Stampa le informazioni sul display LCD
      lcd.clear();
      lcd.setCursor(13, 1);
      lcd.print(Loop_Cycle_Mowing);                                                 // Stampa il numero ci cicli sul display LCD
      delay(5);
      ADCMan.run();
      MAG_Start = perimeter.getMagnitude(0);
      if (MAG_Start> 80) MAG_Start = 80;                                            // Prima non c'era 30/04/2024 (100)
      if (MAG_Start< -80) MAG_Start = -80;                                          // Prima non c'era 30/04/2024 (-100)
      MAG_Now = MAG_Start;
      delay(5);
      MAG_Error = (MAG_Goal - MAG_Start);
      PrintWirePosition();
      Loop_Cycle_Mowing = Loop_Cycle_Mowing + 1;
     

      // Ruota il tagliaerba a sinistra per tornare sul cavo. Movimento in senso orario attorno al filo
      // Spegni la ruota sinistra e la ruota destra a piena potenza per girare a sinistra
      if (MAG_Error > 0) {                                                           // se MAG_Error > 0 allora Gira a sinistra in CW tracking                      
        // GIRA A SX
        PWM_Right = PWM_TrackSpeed_LH;                                               // Impostare la ruota destra al massimo PWM per girare a sinistra
        PWM_Left = PWM_TrackSpeed_LH - (MAG_Error * P);
        if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;                      
        if (PWM_Left >= 0) {
          SetPins_ToGoForwards();                                                    // far avanzare il tosaerba
          lcd.setCursor(15, 0); 
          lcd.print(" ");
          }


          //Svolta curva stretta SX
         if (Boost_Turn == 1) {
            if (PWM_Left < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnLeft();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }

        
        if (PWM_Left < 0) {
          PWM_Left = (-1 * PWM_Left) + 20;                                             // Prima era 20 05/05/2024
          if (PWM_Left > PWM_TrackSpeed_LH) PWM_Left = PWM_TrackSpeed_LH;
          if (PWM_Left >= 0) SetPins_ToTurnLeft();                                     // impostare i perni in modo brusco girare a sinistra  
          delay(5);
          lcd.setCursor(15, 0);
          lcd.print("*");
          }
        
        Motor_Action_Dynamic_PWM_Steering();                                           // inviare i valori PWM al driver del motore.
        Serial.print(F(" Turn Left "));
        Tracking_Turn_Right = 0;
        Tracking_Turn_Left = Tracking_Turn_Left + 1;
        if (Tracking_Turn_Left > Max_Tracking_Turn_Left) {
          Tracking_Restart_Blocked_Path();
        }
      }
      if (MAG_Error <= 0) {                                                            // Girare il tosaerba a destra per tornare sul cavo.
        //GIRA A DX
        PWM_Left = PWM_TrackSpeed_RH;
        PWM_Right = PWM_TrackSpeed_RH + (MAG_Error * P);                               // + poiché mag_error è negativo per regolare PWM
        if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
        if (PWM_Right >= 0) {
          SetPins_ToGoForwards();
          lcd.setCursor(15, 0);
          lcd.print(" ");
          }



          //Svolta curva stretta DX
         if (Boost_Turn == 1) {
            if (PWM_Right < Min_Track_PWM) {
              Serial.print(F("XX"));
              SetPins_ToTurnRight();
              Motor_Action_Go_Track_Speed();
              delay(Hard_Track_Turn_Delay);
              SetPins_ToGoForwards();
            }
         }


        if (PWM_Right < 0) {
          PWM_Right = (-1 * PWM_Right) + 20;                                           // Prima era 220 05/05/2024
          if (PWM_Right > PWM_TrackSpeed_RH) PWM_Right = PWM_TrackSpeed_RH;
          if (PWM_Right >= 0) SetPins_ToTurnRight();                                   // Prima era 0 30/04/2024
          lcd.setCursor(15, 0);
          lcd.print("*");
        }
        
        Motor_Action_Dynamic_PWM_Steering();
        Serial.print(F(" Turn Right "));
        Tracking_Turn_Left = 0;
        Tracking_Turn_Right = Tracking_Turn_Right + 1;
        if (Tracking_Turn_Right > Max_Tracking_Turn_Right) {
          Tracking_Restart_Blocked_Path();
          }

      }
      Serial.print(F(" : MAG_Error="));
      Serial.print(MAG_Error);
      Read_Serial1_Nano();
      Check_if_Charging();
      Check_if_Docked();
      Dock_Cycles = Dock_Cycles + 1;
      if (Dock_Cycles > 10) {
        Tracking_Wire = Tracking_Wire + 1;                                             // Fa lampeggiare il LED di tracciamento del filo nell'app.
        if (Tracking_Wire > 1) Tracking_Wire = 0;
        Mower_Running = 0;
        if (WIFI_Enabled == 1) Get_WIFI_Commands();
        Dock_Cycles = 0;
        }
        if (Loop_Cycle_Mowing == Cycles_Max) Manouver_Cycles_Max();                    // Manovra per sganciari da un'aiuola e ripetere la ricerca del filo peimetrale
      }
    
  Loop_Cycle_Mowing = 0;  
  }
 //Tracking_Wire = 0;
 //Loop_Cycle_Mowing = 0;
 if (Mower_Docked == 1) Tracking_Wire = 0;
 Loop_Cycle_Mowing = 0;
}

 // Avvia un algoritmo per ritrovare il filo dopo averlo perso nel tracciamento
void Tracking_Restart_Blocked_Path() {
  Motor_Action_Stop_Motors();
  Serial.println(F("Possible Blocked Path - Trying to Avoid"));

  // Cicli di reset e delay con controllo comando
  for (int i = 0; i < 2; i++) {
    if (Abort_Wire_Find || Mower_Parked == 1) return;
    Mower_Running = 1;
    Tracking_Wire = 1;
    if (WIFI_Enabled == 1) Get_WIFI_Commands();
    delay(500);

    if (Abort_Wire_Find || Mower_Parked == 1) return;
    Mower_Running = 0;
    Tracking_Wire = 0;
    if (WIFI_Enabled == 1) Get_WIFI_Commands();
    delay(500);
  }

  if (Abort_Wire_Find || Mower_Parked == 1) return;

  lcd.clear();
  lcd.print("Perso il Filo.");
  lcd.setCursor(0, 1);
  lcd.print("Recupero.....");
  if (WIFI_Enabled == 1) Get_WIFI_Commands();

  // ❌ Se è stato parcheggiato nel frattempo, abbandona la funzione
  if (Mower_Parked == 1 || Abort_Wire_Find) return;

  Motor_Action_Stop_Motors();
  delay(1000);

  Tracking_Turn_Left = 0;
  Tracking_Turn_Right = 0;
  Mower_Running = 0;
  Tracking_Wire = 0;

  if (WIFI_Enabled == 1) Get_WIFI_Commands();
  if (Abort_Wire_Find || Mower_Parked == 1) return;

  if (Compass_Activate == 1) Compass_Turn_Mower_To_Home_Direction();
  if (Abort_Wire_Find || Mower_Parked == 1) return;

  Manouver_Find_Wire_Track();
}

