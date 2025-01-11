void Check_Wire_In_Out() {
  // Verificare che il tagliaerba sia all'interno (0) o all'esterno (1) del cavo perimetrale

  // Se il sensore filo perimetrale è spento
  if (Perimeter_Wire_Enabled == 0) {
        Outside_Wire = 0;    
      }

  // Se il sensore filo perimetrale è acceso      
  if (Perimeter_Wire_Enabled == 1) {
    for (int i = 0; i < 3; i++) {
             delay(10);                                                             
             ADCMan.run();
             UpdateWireSensor();
             delay(10);
             ADCMan.run();
             UpdateWireSensor();
             delay(10);
             PrintBoundaryWireStatus();
            }

    
   // UpdateWireSensor();                                               // Leggere il sensore del cavo e vedere se il tagliaerba è ora o fuori dal cavo 
   // ADCMan.run();
   // PrintBoundaryWireStatus(); 

  // Fuori dal filo
    if ( (perimeter.isInside(0)) == 0 )  {
      Outside_Wire = 1;                                               // Il tagliaerba è fuori dal cavo perimetrale
      if (Mower_Running == 1) Motor_Action_Stop_Motors();
      Print_LCD_Wire();
      Outside_Wire_Count = Outside_Wire_Count + 1;                    // Il numero di conteggi di fili esterni è aumentato.
      }
    

  // Dentro al filo
    if ( (perimeter.isInside(0)) == 1 )   {
      Outside_Wire = 0;                                               // Il tagliaerba è all'interno del cavo perimetrale
      Outside_Wire_Count = 0;                                         // Il numero di conteggi fili esterni viene reimpostato a 0
      Wire_Refind_Tries = 0;
      }

  }


  // Le opzioni vengono impostate in Mower Setup
  if (Outside_Wire_Count >= Outside_Wire_Count_Max) {
    if  (Action_On_Over_Wire_Count_Max == 1) Manouver_Hibernate_Mower();                  // Metti il ​​tosaerba a dormire e aspetta
    if  (Action_On_Over_Wire_Count_Max == 2) Manouver_Outside_Wire_ReFind_Function();     // ritrova il giardino usando il sonar 1 e il rilevamento del filo
    
    if  (Action_On_Over_Wire_Count_Max == 3) {                                            // prova a individuare il filo utilizzando la funzione di ricerca filo
      lcd.clear();
      lcd.print("Trova Filo");
      lcd.setCursor(0,1);
      lcd.print("Funzione Speciale");
      delay(2000);
      Outside_Wire_Count = 0;
      Specials_Find_Wire_Track();                  
      SetPins_ToGoBackwards();                                          // Imposta il tosaerba per tornare indietro
      Motor_Action_Go_Full_Speed(); 
      delay(300);                                                       //Prima era 700 15/06/22
      Motor_Action_Stop_Motors();
      SetPins_ToTurnRight();                                            // Aggiunta il 18/06/22
      Motor_Action_Turn_Speed();                                        // Aggiunta il 18/06/22
      delay(Mower_Turn_Delay_Max);                                      // Aggiunta il 18/06/22
      Motor_Action_Stop_Motors();                                       // Aggiunta il 18/06/22
      UpdateWireSensor();                                               // Leggere il sensore del cavo e vedere se il tagliaerba è ora o fuori dal cavo  
      PrintBoundaryWireStatus(); 
      delay(1000);
      UpdateWireSensor();                                               // Leggere il sensore del cavo e vedere se il tagliaerba è ora o fuori dal cavo
      ADCMan.run();
      PrintBoundaryWireStatus(); 
      Wire_Refind_Tries = Wire_Refind_Tries + 1;
      Serial.println("");
      Serial.print("|Wire Refind Atempts:");
      Serial.print(Wire_Refind_Tries);
      Serial.print("|");
      Serial.println(""); 

         
      }
   
      if (Wire_Refind_Tries > 2) {                          // Prima era (> 4) Aggiunta il 18/06/22
        lcd.print("Filo Non Trovato");
        Motor_Action_Stop_Motors();
        Motor_Action_Stop_Spin_Blades();                    // Ferma la rotazione delle lame
        Turn_Off_Relay(); 
        lcd.clear();
       // Print_Mower_Error();
       // Mower_Error = 1;
        Manouver_Park_The_Mower();                          // Aggiunto il 17/06/2022
        Serial.println("");
        Serial.println("Max refind tries exceeded - Parking the Mower");
        delay(2000);
      
       } 
     }  
  }


//Verificare che il cavo perimetrale sia acceso
//************************************************************************************
void TestforBoundaryWire()  {
  ADCMan.run();
  UpdateWireSensor();
  
  if (Perimeter_Wire_Enabled == 1) {                                               // Perimeter use is ON - Perimter_USE can be turned on or off in the setup.

    /*Controlla se il cavo perimetrale è spento.
       utilizza il campo magnetico calcolato. Tra -10 e 10 normalmente il campo è spento.     */

    MAG_Now = perimeter.getMagnitude(0);

    // Only used for testing purposes 
    if ( (MAG_Now > -7 ) && (MAG_Now < 7 )  ) {
      Wire_Detected = 0;
      Print_LCD_NO_Wire();
      Wire_Off = Wire_Off + 1;
      }

     /* Verifica che la recinzione perimetrale sia attivata.
      usa il campo magnetico. inferiore a -10 o superiore a 10 significa che il campo è attivo  */
    if (   (MAG_Now <= -7 ) || (MAG_Now >= 7 )  ) {
      Wire_Detected = 1;
      Mower_Running = 1;
      Print_LCD_Wire_ON();
      Wire_Off = 0;
      }

      // Se il tosaerba non è agganciato alla base e non è parcheggiato, e rileva per troppo tempo il filo spento blocca il robot
      if ( (Wire_Off > Count_Wire_Off) && (Mower_Docked == 0) && (Mower_Parked == 0) ) {
        Serial.println(F("Wire Test Failed - Hibernating Mower"));
        Manouver_Hibernate_Mower();      // Send the mower to sleep        
        }

    Serial.print(F("Wire"));
    Serial.print(F(":"));
    if (Wire_Detected == 0) Serial.print(F("OFF|"));
    if (Wire_Detected == 1) Serial.print(F("ON|"));  
  }

  // Se il filo è in modalità test
  if (Perimeter_Wire_Enabled == 0) {   
    Serial.print(F("Wire"));
    Serial.print(F(":"));                                            
    Serial.print(F("TEST|"));
    Wire_Detected = 1;

  }
}


void UpdateWireSensor()   {
    if (millis() >= nextTime)  {
    nextTime = millis() + 50;
    if (perimeter.isInside(0) != inside) {
      inside = perimeter.isInside(0);
      counter++; 
    }
  }

} 


void PrintBoundaryWireStatus() {

  Serial.print("IN/OUT:");
  Serial.print((perimeter.isInside(0)));
  Serial.print("|");
  Serial.print("Mag:");
  Serial.print((int)perimeter.getMagnitude(0));
  Serial.print("|");
  Serial.print("Smag:");
  Serial.print((int)perimeter.getSmoothMagnitude(0));
  Serial.print("|");
  Serial.print("Q:");
  Serial.print((perimeter.getFilterQuality(0)));
  Serial.print("|");
  }
