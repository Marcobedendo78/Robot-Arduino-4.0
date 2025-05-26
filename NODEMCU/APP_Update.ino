

void Update_Blynk_App_With_Status() {

         if (Mower_Docked_Filter == 2)   {
           //Pulsanti dell'app
           Blynk.virtualWrite(V10,LOW);       // Pulsante Esci dalla Base
           Blynk.virtualWrite(V0, LOW);       // Pulsante di avvio rapido
           Blynk.virtualWrite(V2, LOW);       // Pulsante Pausa/Stop
           Blynk.virtualWrite(V1, LOW);       // Pulsante Vai dalla Base
           }
        
        if (Mower_Running_Filter == 2)   {
           //Pulsanti dell'app
           Blynk.virtualWrite(V10,LOW);       // Pulsante Esci dalla Base
           Blynk.virtualWrite(V0, LOW);       // Pulsante di avvio rapido
           Blynk.virtualWrite(V2, LOW);       // Pulsante Pausa/Stop
           Blynk.virtualWrite(V1, LOW);       // Pulsante Vai dalla Base
           }

        if (Manuel_Mode == 1)   {
           //Pulsanti dell'app
           Blynk.virtualWrite(V10,LOW);       // Pulsante Esci dalla Base
           Blynk.virtualWrite(V0, LOW);       // Pulsante di avvio rapido
           Blynk.virtualWrite(V2, LOW);       // Pulsante Pausa/Stop
           Blynk.virtualWrite(V1, LOW);       // Pulsante Vai dalla Base
           
           Blynk.virtualWrite(V6, 0);
           Blynk.virtualWrite(V7, 0);
           Blynk.virtualWrite(V9, 0);     
           Blynk.virtualWrite(V8, 0);
           }

        if (Mower_Parked == 1)   {
           //Pulsanti dell'app
           Blynk.virtualWrite(V10,LOW);       // Pulsante Esci dalla Base
           Blynk.virtualWrite(V0, LOW);       // Pulsante di avvio rapido
           Blynk.virtualWrite(V2, LOW);       // Pulsante Pausa/Stop
           Blynk.virtualWrite(V1, LOW);       // Pulsante Vai dalla Base
           }
      
      if (Charge_Detected == 4)   Blynk.virtualWrite(V12, 1023);               // LED di carica ON
      if (Charge_Detected == 0)   Blynk.virtualWrite(V12, 0);                  // LED di carica OFF
      if (Tracking_Wire == 1)     Blynk.virtualWrite(V9, 1023);                // LED di Tracciatura ON
      if (Tracking_Wire == 0)     Blynk.virtualWrite(V9, 0);                   // LED di Tracciatura OFF
      if (Mower_Parked == 1)      Blynk.virtualWrite(V6, 1023);                // LED di Parcheggio ON
      if (Mower_Parked == 0)      Blynk.virtualWrite(V6, 0);                   // LED di Parcheggio OFF
      if (Mower_Running_Filter == 2)     Blynk.virtualWrite(V7, 1023);         // LED di Taglio ON
      if (Mower_Running_Filter != 2)     Blynk.virtualWrite(V7, 0);            // LED di Taglio OFF
      if (Mower_Docked_Filter == 2)      Blynk.virtualWrite(V8, 1023);         // LED di Parcheggio ON
      if (Mower_Docked_Filter != 2)      Blynk.virtualWrite(V8, 0);            // LED di Parcheggio OFF
    }



BLYNK_WRITE(V0)                                                                // Funzione di taglio ed avvio rapido
{
  Clear_APP(); 
  
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V1 a una variabile
  if (pinValue == 1) StartMower();       
  //Clear_APP();      
  lcd.clear();
  lcd.print(0, 0, "Starting . . ");                                            // Stampa sul display LCD dell'APP
  delay(100);
  Blynk.virtualWrite(V0, HIGH);                                                // Pulsante Avvia taglio su ON
}


BLYNK_WRITE(V1)                                                                // Vai alla Base di Ricarica
{
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V1 a una variabile
  if (pinValue == 1) Going_Home();
  //Clear_APP();
  lcd.clear();
  lcd.print(0, 0, "Docking . . ");                                             // Stampa sul display LCD dell'APP
  delay(100);
  Blynk.virtualWrite(V1, HIGH);                                                // Pulsante di Parcheggio ON

}



BLYNK_WRITE(V2)                                                                // Pausa alle Funzioni del Robot
{
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V2 a una variabile
  if (pinValue == 1)  Pause_Mower();
  //Clear_APP();
  lcd.clear();
  lcd.print(0, 0, "Stopping . . ");                                            // Stampa sul display LCD dell'APP
  delay(100);
  Blynk.virtualWrite(V2, HIGH);                                                // Pulsante di Pausa ON
   }





BLYNK_WRITE(V10)                                                               // Funzione di uscita dalla base
{
  int pinValue = param.asInt();                                                //assegnando il valore in ingresso dal pin V10 a una variabile
  if (pinValue == 1) Exit_Dock();
  //Clear_APP();
  lcd.clear();
  lcd.print(0, 0, "Exiting . . ");                                             // Stampa sul display LCD dell'APP
  delay(100);
  Blynk.virtualWrite(V10, HIGH);                                               // Pulsante Base ON
  

}


BLYNK_WRITE(V13)                                                               // Manuale - Movimento avanti
{
  if (Manuel_Mode = 1) {
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V10 a una variabile
  if (pinValue == 1)
  transmit_blynk_code = 7;
  Transmit_Blynk_Data_to_Mega();
  Blynk.virtualWrite(V13, LOW);                                                // Pulsante Avvia taglio su ON
  }
  

}

BLYNK_WRITE(V14)                                                               // Manuale - Movimento indietro
{
  if (Manuel_Mode = 1) {
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V10 a una variabile
  if (pinValue == 1)
  transmit_blynk_code = 8;
  Transmit_Blynk_Data_to_Mega();
  }
}

BLYNK_WRITE(V15)                                                               // Manuale - Gira a SX
{
  if (Manuel_Mode = 1) {
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V10 a una variabile
  if (pinValue == 1)
  transmit_blynk_code = 9;
  Transmit_Blynk_Data_to_Mega();
  }
}

BLYNK_WRITE(V16)                                                               // Manuale - Gira a DX
{
  if (Manuel_Mode = 1) {
  int pinValue = param.asInt();                                                // assegnando il valore in ingresso dal pin V10 a una variabile
  if (pinValue == 1)
  transmit_blynk_code = 10;
  Transmit_Blynk_Data_to_Mega();


  }
}

BLYNK_WRITE(V4) {
  switch (param.asInt())
  {
    case 1: { // Item 1
        Serial.println("Automatic Mode");
        Automatic_Mode = 1;
        Manuel_Mode = 0;
        Set_To_Automatic_Mode_Random();
        Update_Blynk_App_With_Status();
        break;
      }
    case 2: { // Item 2
        Serial.println("Automatic Spiral");
        Automatic_Mode = 2;
        Manuel_Mode = 0;
        Set_To_Automatic_Mode_Spiral();
        Update_Blynk_App_With_Status();
        break;
      }
    
    case 3: { // Item 3
        Serial.println("Automatic Parallel");
        Manuel_Mode = 0;
        Automatic_Mode = 1;
        Set_To_Automatic_Mode_Parallel();
        Mower_Parked = 0;
        Mower_Docked = 0;
        Mower_Running = 0;
        Update_Blynk_App_With_Status();
        break;
      }

    
    case 4: { // Item 4
        Serial.println("Manuel Mode");
        Manuel_Mode = 1;
        Automatic_Mode = 0;
        Set_To_Manuel_Mode();
        Mower_Parked = 0;
        Mower_Docked = 0;
        Mower_Running = 0;
        Update_Blynk_App_With_Status();
        break;
      }

    }
}

void Clear_APP() {
  Blynk.virtualWrite(V0, LOW);                   // Pulsante Partenza a Tagliare OFF
  Blynk.virtualWrite(V1, LOW);                   // Pulsante Vai alla Base OFF
  Blynk.virtualWrite(V2, LOW);                   // Pulsante STOP OFF
  Blynk.virtualWrite(V3, 0);                     // Reset Voltaggio
  Blynk.virtualWrite(V5, 0);                     // Loops
  Blynk.virtualWrite(V7, 0);                     // LED Taglio
  Blynk.virtualWrite(V8, 0);                     // LED Base OFF
  Blynk.virtualWrite(V9, 0);                     // LED Tracciatura OFF
  Blynk.virtualWrite(V10, LOW);                  // Dock Button OFF
  Blynk.virtualWrite(V11, 0);                    // LED Bussola OFF  
  Blynk.virtualWrite(V12, 0);                    // LED Ricarica OFF  
}
