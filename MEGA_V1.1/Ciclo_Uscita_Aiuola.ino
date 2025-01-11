// La logica di Cycles Max decide di quanto deve seguire il filo il tagliaerba se gira attorno a un'aiuola
// Questo è utile se il tosaerba rileva il filo perimetrale di un'aiuola e non riesce più a seguire il filo del perimetro per tornare alla base di ricarica.
   

void Manouver_Cycles_Max() {                                                                      
     Serial.println("");
     Serial.println("Loop Cycle at Max");
     Serial.println("");
     Motor_Action_Stop_Spin_Blades();
     Motor_Action_Stop_Motors();
     lcd.clear();
     Print_Cycles_Max_LCD(); 
     delay(2000);

    if (CCW_Tracking_To_Start == 1) {                              // Uscita dal Garage in senso Antiorario
        SetPins_ToTurnLeft();                                      // Prepara i motori a girare a Sinistra
        Motor_Action_Turn_Speed();                                 // Gira le ruote
        delay(700);                                                // Tempo di rotazione 
        Motor_Action_Stop_Motors;                                  // Stop
        delay(2000);
        lcd.clear();
        Manouver_Go_To_Charging_Station_2();
        }

    if (CW_Tracking_To_Start == 1) {                               // Uscita dal Garage in senso Orario
        SetPins_ToTurnRight();                                     // Prepara i motori a girare a Destra
        Motor_Action_Turn_Speed();                                 // Gira le ruote
        delay(700);                                                // Tempo di rotazione
        Motor_Action_Stop_Motors;                                  // Stop
        delay(2000);
        lcd.clear();
        Manouver_Go_To_Charging_Station_2();
        }
 }
