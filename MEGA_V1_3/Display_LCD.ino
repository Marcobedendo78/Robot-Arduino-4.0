// Informazioni da stampare sullo schermo LCD del tagliaerba
void Setup_Run_LCD_Intro () {
  Serial.println("Setup LCD");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("ROBOT WORRIOR OK!");
  lcd.setCursor(0,1);
  lcd.print(Version);  
  if (WIFI_Enabled == 1) {
    lcd.setCursor(7,1);
    lcd.print("WIFI ON");
    }
  delay(1000);
  lcd.clear();
  Serial.println("LCD Setup OK");
  }


void Print_Mower_Error() {
  lcd.clear();
  lcd.setCursor(6,0);                                        // lcd.setCursor(0,0);
  lcd.print("OFF");                                          // lcd.print(F("Tagliaerba Errore"));
 /* if (Wire_Off > 5) {
    lcd.setCursor(0,1);
    lcd.print("FILO OFF");
    }
  if (Wire_Refind_Tries > 4) {
    lcd.setCursor(0,1);
    lcd.print(F("Filo non Rilevato"));
  }*/
  
  }
  


void Print_LCD_Volt_Info() {
    lcd.setCursor(10, 0);
    lcd.print("V:");
    lcd.setCursor(12, 0);    
    lcd.print(Volts);  
    }

void Print_Charging_LCD()  {
  lcd.setCursor(5,0);
  if (Charge_Detected_MEGA == 1)  lcd.print("CARICA");
  if ((Rain_Detected == 0) && (Charge_Detected_MEGA != 1) ) lcd.print("     ");
  }

void Print_Raining_LCD()  {
  lcd.setCursor(4,0);
  if (Rain_Detected == 1) lcd.print("Pioggia");
  // Vedere la clausola relativa alla pioggia e all'addebito se non viene visualizzata correttamente.
    if ((Rain_Detected == 0) && (Charging == 0) ) lcd.print("    ");
  }

void Print_Recharge_LCD() {
    lcd.setCursor(0,1);
    lcd.print("Ricarica Batt");
    }

void Print_LCD_Wire()  {
       lcd.setCursor(0, 1);
       lcd.print("FILO RILEVATO");     
    }

void Print_LCD_Ruota()  {
       lcd.setCursor(0, 1);
       lcd.print("RUOTE BLOCCATE");     
    }


void Print_Cycles_Max_LCD() {
    lcd.setCursor(0,1);
    lcd.print("SGANCIA AIUOLA");
    }


void Print_LCD_Mowing() {
     if (Alarm_Timed_Mow_ON == 0) {
       lcd.setCursor(0, 1);
       lcd.print("Taglio.     ");
       }
     if (Alarm_Timed_Mow_ON == 1) {
       lcd.setCursor(0,1);
       lcd.print("Timer:");
       lcd.print(Alarm_Timed_Mow_Hour);
       lcd.print(":");
       if (Alarm_Timed_Mow_Minute < 10) lcd.print("0");
       lcd.print(Alarm_Timed_Mow_Minute);
       Mow_Time_Set = 1;
       }   
       
     }

void Print_LCD_Spiral() {
     if (Alarm_Timed_Mow_ON == 0) {
       lcd.setCursor(0, 1);
       lcd.print("Spirale.. ");
       }
     if (Alarm_Timed_Mow_ON == 1) {
       lcd.setCursor(0,1);
       lcd.print("Timer:");
       lcd.print(Alarm_Timed_Mow_Hour);
       lcd.print(":");
       if (Alarm_Timed_Mow_Minute < 10) lcd.print("0");
       lcd.print(Alarm_Timed_Mow_Minute);
       Mow_Time_Set = 1;
       }   
       
     }

void Print_LCD_Parallel() {
     if (Alarm_Timed_Mow_ON == 0) {
       lcd.setCursor(0, 1);
       lcd.print("Parallelo");
       }
     if (Alarm_Timed_Mow_ON == 1) {
       lcd.setCursor(0,1);
       lcd.print("Timer:");
       lcd.print(Alarm_Timed_Mow_Hour);
       lcd.print(":");
       if (Alarm_Timed_Mow_Minute < 10) lcd.print("0");
       lcd.print(Alarm_Timed_Mow_Minute);
       Mow_Time_Set = 1;
       }   
       
     }
     
void Print_LCD_Compass_Mowing() {
     if (Compass_Heading_Locked == 1) {
         lcd.setCursor(0, 1);
         if (PWM_Right > PWM_Left)  lcd.print("<TAG       ");      
         if (PWM_Left > PWM_Right)  lcd.print(" TAG>      "); 
         if (PWM_Left == PWM_Right) lcd.print("|TAG|      ");
         }
     if (Compass_Heading_Locked == 0) {
         lcd.setCursor(0, 1);
         lcd.print("           ");      
         }
     }

void Print_LCD_Parallel_Mowing() {
     if (Compass_Heading_Locked == 1) {
         lcd.setCursor(0, 1);
         if (PWM_Right > PWM_Left)  lcd.print("<P TAG     ");      
         if (PWM_Left > PWM_Right)  lcd.print(" P TAG>    "); 
         if (PWM_Left == PWM_Right) lcd.print("|P TAG|    ");
         }
     if (Compass_Heading_Locked == 0) {
         lcd.setCursor(0, 1);
         lcd.print(" Parallelo ");      
         }
     }

void Print_LCD_Info_Parked() {
    lcd.setCursor(0,1);
    lcd.print("  Parcheggiato  ");
    }

void Print_LCD_Info_Manuel() {
    lcd.setCursor(0,1);
    lcd.print("     Manuale    ");
    }

void Print_LCD_Info_Docked() {
    lcd.setCursor(0,1);
    lcd.print("Base");  
}

void Print_LCD_NO_Wire() {
    if ( (Mower_Docked == 1) || (Mower_Parked == 1) ) {
      lcd.setCursor(7,1);
      lcd.print(":FILO OFF");  
      Wire_ON_Printed = 0;
      }
    if ( (Mower_Docked == 0) && (Mower_Parked == 0) ) {
      lcd.setCursor(0,1);
      lcd.print(":FILO OFF        ");      
      Wire_ON_Printed = 0;
      }
}

void Print_LCD_Wire_ON() {
    if ( (Mower_Docked == 1) || (Mower_Parked == 1)  && (Wire_ON_Printed = 0)) {
      lcd.setCursor(7,1);
      lcd.print(":               ");  
      Wire_ON_Printed = 1; 
      }
    if ( (Mower_Docked == 0) && (Mower_Parked == 0) && (Wire_ON_Printed = 0) ) {
      lcd.setCursor(0,1);
      lcd.print(":               ");   
      Wire_ON_Printed = 1;    
      }
}

void Print_Sonar_Hit()  {
    lcd.setCursor(0,1);
    lcd.print("OGGETTO SONAR ");  
    }

void Print_LCD_Compass_Home() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bussola Casa");
    if (Rain_Detected == 1) {
     lcd.setCursor(0,1);
     lcd.print("PIOGGIA");
    }
}



void Print_Time_On_LCD() {
    if ((Charge_Detected_MEGA == 0) && (Mower_Running == 0) && (Rain_Detected == 0)) {
      lcd.setCursor(0,0);            // Spaces to the right & down
      Time t = rtc.time();
      lcd.print(t.hr);
      lcd.print(":");
      if (t.min < 10) lcd.print ("0");
      lcd.print(t.min);
      }
   }
