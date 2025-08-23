
void Pattern_Mow_Spirals() {
      //analogWrite (ENAPin, PWM_MaxSpeed_RH - (100 - Loop_Cycle_Mowing)); // 255-200 = di nuovo da 55 a 255. (255 è la velocità massima). La velocità è impostata nelle impostazioni                        
      
      int PWM_1 = 20;         // diametro 0.4m    
      int PWM_2 = 30;         //                    
      int PWM_3 = 40;
      int PWM_4 = 50;
      int PWM_5 = 60;        //
      int PWM_6 = 70;
      int PWM_7 = 80;        // (diametro 3-4m)

      int End_Linking                     = 10;      // la sezione in linea retta tra le spirali 15 è di circa 3-4 m
      int End_Spiral_1   = End_Linking    + 10;      // imposta il numero di cicli all'interno della Spirale 1
      int End_Spiral_2   = End_Spiral_1   + 15;
      int End_Spiral_3   = End_Spiral_2   + 25;
      int End_Spiral_4   = End_Spiral_3   + 30;      // imposta il numero di cicli all'interno della Spirale 4
      int End_Spiral_5   = End_Spiral_4   + 35;
      int End_Spiral_6   = End_Spiral_5   + 40;
      int End_Spiral_7   = End_Spiral_6   + 50;     // imposta il numero di cicli all'interno della Spirale 7

      // Il numero totale di cicli in tutte le sezioni non deve superare Max_Cycles_Spirals
      // altrimenti il ​​tosaerba si fermerà prima che il percorso a spirale sia completato


      // Spiral_Mow = 1 è uno Sprial della mano destra
      // Spiral_Mow = 2 è una spirale della mano sinistra
      // Sprial: Mow = 3 no sprial e un normale movimento in linea retta


      Get_Compass_Reading();  // mantiene attiva la bussola
      
      if (Spiral_Mow == 1) analogWrite(ENBPin, PWM_MaxSpeed_LH);  
      if (Spiral_Mow == 2) analogWrite(ENAPin, PWM_MaxSpeed_RH);
      if (Spiral_Mow == 3) {
        analogWrite(ENAPin, PWM_MaxSpeed_RH);
        analogWrite(ENBPin, PWM_MaxSpeed_LH);
      }

      if (Spiral_Mow < 3) {
        
      
      Serial.print(F("Wheel:CIRCLE|"));
      lcd.setCursor(9,1);
      if (Spiral_Mow == 1) lcd.print("DX");
      if (Spiral_Mow == 2) lcd.print("SX");
      if (Spiral_Mow == 3) lcd.print("|");

      if (Loop_Cycle_Mowing < End_Linking) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_MaxSpeed_RH);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_MaxSpeed_LH);
        lcd.print("|");
        }

      //Spirale interna
      if ((Loop_Cycle_Mowing >= End_Linking) && (Loop_Cycle_Mowing < End_Spiral_1)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_1);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_1);
        lcd.print("1");
        }

      // Anello spirale 2
      if ((Loop_Cycle_Mowing >= End_Spiral_1) && (Loop_Cycle_Mowing < End_Spiral_2)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_2);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_2);
        lcd.print("2");        
        }

      // Anello spirale 3
      if ((Loop_Cycle_Mowing >= End_Spiral_2) && (Loop_Cycle_Mowing < End_Spiral_3)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_3);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_3);
        lcd.print("3");         
        }

      // Anello spirale 4
      if ((Loop_Cycle_Mowing >= End_Spiral_3) && (Loop_Cycle_Mowing < End_Spiral_4)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_4);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_4);
        lcd.print("4");         
        }
      
      // Anello spirale 5
      if ((Loop_Cycle_Mowing >= End_Spiral_4) && (Loop_Cycle_Mowing < End_Spiral_5)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_5);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_5);
        lcd.print("5");         
        }

      // Anello spirale 6
      if ((Loop_Cycle_Mowing >= End_Spiral_5) && (Loop_Cycle_Mowing < End_Spiral_6)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_6);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_6);
        lcd.print("6");         
        }

      // Anello spirale 7
      if ((Loop_Cycle_Mowing >= End_Spiral_6) && (Loop_Cycle_Mowing < End_Spiral_7)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_7);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_7);
        lcd.print("7");         
        }
        
      // Il tagliaerba si sposta in una nuova posizione
      if ((Loop_Cycle_Mowing >= End_Spiral_7) && (Loop_Cycle_Mowing < Max_Cycles_Spirals)) {
        if (Spiral_Mow == 1) analogWrite(ENAPin, PWM_MaxSpeed_RH);
        if (Spiral_Mow == 2) analogWrite(ENBPin, PWM_MaxSpeed_LH);
        lcd.print("X");     
        }
      }

}



void Pattern_Mow_Parallel() {
  
  int Turn_90_Delay_LH        = 500 ;      // regola questo numero in modo che il tagliaerba giri di 90 ° a sinistra
  int Turn_90_Delay_RH        = 500 ;      // regola questo numero in modo che il tagliaerba giri di 90 ° a destra
  int Move_to_next_line_delay = 500 ;      // distanza tra le linee
  int Line_Length_Cycles      = 25 ;        // lunghezza del filo falciato
  int Parallel_Compass_Assist = 1 ;         // la bussola cerca di mantenere dritta la linea falciata
  int Turning_Compass_Assist  = 0 ;         // la bussola aiuta a ruotare il tosaerba di 90 ° ad ogni angolo - ANCORA NON FUNZIONA !!!
       


// se la lunghezza della linea non è stata raggiunta, continua ad andare dritto.
  if (Loop_Cycle_Mowing < Line_Length_Cycles)  {
    SetPins_ToGoForwards();

    // Utilizzare la bussola assistita per mantenere il tosaerba in linea retta, si spera parallela all'ultima
    if (Parallel_Compass_Assist == 1) {  
        
     if (Loop_Cycle_Mowing < 3) {
        if ((Compass_Heading_Hold_Enabled == 1 ) && (Compass_Activate == 1 )) {          // usa la funzione di blocco dell'intestazione per il taglio parallelo
           Get_Compass_Reading ();                                                      // Ottiene l'ultima lettura della bussola
           Heading_Lock = Compass_Heading_Degrees;                                     // salva questa lettura della bussola nel blocco della direzione
           Compass_Heading_Locked = 1 ;                                                 // Attiva la funzione di blocco della direzione
           Compass_Last = Heading_Lock;
           Motor_Action_Go_Full_Speed();
           }
        }
    
     if (Loop_Cycle_Mowing >= 3) {
        if ((Compass_Heading_Hold_Enabled == 1) && (Compass_Activate == 1)) {            // se il tagliaerba sta tracciando utilizzando la bussola sterza qui
          if ( (Loop_Cycle_Mowing % 2) == 0 ) {
          Get_Compass_Reading(); 
          Calculate_Compass_Wheel_Compensation();
          Motor_Action_Dynamic_PWM_Steering();              // Rimuove la funzione di velocità massima se il tagliaerba sta cercando di mantenere la direzione della bussola.
          Print_LCD_Parallel_Mowing();
          Serial.print(F("C-Lock:ON_"));
          Serial.print("|");
          }
        }
     }
    }
     
    // Nessun utilizzo della bussola assistita
    if (Parallel_Compass_Assist == 0) {
        Motor_Action_Go_Full_Speed();
        Serial.println("Compass not activated in the settings");
       }
    }

// se la lunghezza della linea è stata raggiunta, girarsi.
  if (Loop_Cycle_Mowing > Line_Length_Cycles) {
    Motor_Action_Stop_Motors();
    delay(300);
    
    // Gira di 90°
    if (Leg == 1) {
      SetPins_ToTurnLeft();
      Motor_Action_Go_Full_Speed();
      delay(Turn_90_Delay_LH);
      Get_Compass_Reading(); 
    }
    if (Leg == 2) {
      SetPins_ToTurnRight();
      Motor_Action_Go_Full_Speed();
      delay(Turn_90_Delay_RH); 
      Get_Compass_Reading(); 
      }

    Motor_Action_Stop_Motors();
    delay(300);
    
    // Vai alla riga successiva
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    delay(Move_to_next_line_delay);
    Get_Compass_Reading(); 
    Motor_Action_Stop_Motors();
    delay(300);
    
    // Ruota la finale di 90 ° per affrontare la direzione parallela successiva utilizzando solo i tempi indicati sulle ruote sinistra e destra
    // questo darà una svolta innaczrate ma viene eseguito stabkle
    if (Turning_Compass_Assist  == 0) {
      if (Leg == 1) {
        SetPins_ToTurnLeft();
        Motor_Action_Go_Full_Speed();
        delay(Turn_90_Delay_LH);
        Get_Compass_Reading(); 
        Motor_Action_Stop_Motors();
      }
      if (Leg == 2) {
        SetPins_ToTurnRight();
        Motor_Action_Go_Full_Speed();
        delay(Turn_90_Delay_RH); 
        Get_Compass_Reading(); 
        Motor_Action_Stop_Motors();
        }
      }

    //Regola la direzione finale del tosaerba utilizzando la funzione di rotazione della bussola
    if (Turning_Compass_Assist  == 1) {
      
    
      // Imposta i gradi della bussola di destinazione a 180 ° dall'ultima riga
      Compass_Target = Heading_Lock + 180;
      if (Compass_Target > 360) (Compass_Target = Compass_Target - 360);

      
      if (Leg == 1) {
        SetPins_ToTurnLeft();
        Motor_Action_Go_Full_Speed();
        delay(Turn_90_Delay_LH);
        Motor_Action_Stop_Motors();
        Get_Compass_Reading(); 
        delay(500);
        lcd.clear();
        lcd.print("Turn Assist");
        lcd.setCursor(0,1);
        lcd.print("Target");
        lcd.print(Compass_Target);
        delay(1000);
        lcd.clear();
        Turn_To_Compass_Heading();
        lcd.clear();
      }
      if (Leg == 2) {
        SetPins_ToTurnRight();
        Motor_Action_Go_Full_Speed();
        delay(Turn_90_Delay_RH); 
        Motor_Action_Stop_Motors();
        Get_Compass_Reading(); 
        delay(500);
        lcd.clear();
        lcd.print("Turn Assist");
        lcd.setCursor(0,1);
        lcd.print("Target");
        lcd.print(Compass_Target);
        delay(1000);
        lcd.clear();
        Turn_To_Compass_Heading();
        lcd.clear();
        }
      }
    
    Motor_Action_Stop_Motors();
    delay(300);
    SetPins_ToGoForwards();


    Loop_Cycle_Mowing   = 0;          // Reimposta il contatore del loop
    lcd.setCursor(13, 1);
    lcd.print("   ");
    Leg = Leg + 1;                    // Fa avanzare la gamba in modo che il tosaerba giri nella direzione opposta.
    if (Leg > 2) Leg = 1;             // Mantiene la variabile di gamba su 1 o 2
     
    }

  
}

    
