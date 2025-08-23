// BUTTONS SETTINGS MENU


// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Settings(byte LCD_Menu_Settings) {
  if (LCD_Menu_Settings == 1)  lcd.print(F("Timer"));
  if (LCD_Menu_Settings == 2)  lcd.print(F("Sensori"));
  if (LCD_Menu_Settings == 3)  lcd.print(F("Movimento"));  
  if (LCD_Menu_Settings == 4)  lcd.print(F("Tracciatura"));
  if (LCD_Menu_Settings == 5)  lcd.print(F("Navigazione"));
  if (LCD_Menu_Settings == 6)  lcd.print(F("- -"));
  if (LCD_Menu_Settings == 7)  lcd.print(F("BETA"));   // Leave Blank
  if (LCD_Menu_Settings == 8)  lcd.print(F("- -"));   // Leave Blank
  if (LCD_Menu_Settings == 9)  lcd.print(F("- -"));   // Leave Blank
  if (LCD_Menu_Settings == 10) lcd.print(F("CLEAR EEPROM"));   // Leave Blank

  Max_Options_Settings = 10; 
  }







void Print_Membrane_Switch_Input_Settings() {

  //Menu Options if the Mower is Settings.
     Read_Membrane_Keys();
     Menu_Complete = 1;
     Menu_Mode_Selection = 0;
     Menu_View = 0;

    
      Serial.println();
      Serial.println(F("Settings Menu Activated"));
      Menu_Complete_Settings = false;                                // Menu complete will return to the normal loop
      lcd.clear();
      delay(5);
 

      while (Menu_Complete_Settings == false) {                      // holds the program in a loop until a selection has been made in the membrane button menu
        if (Menu_View == 0) {
          lcd.setCursor(1,0);
          //Print_LCD_Menu_Settings(1);
          lcd.print("-- Settaggi --");
          lcd.setCursor(1,1);
          Print_LCD_Menu_Settings(1);
          }
        // Gets the values again from the keys
        Read_Membrane_Keys();
        delay(100);
             
        if(!Start_Key_X){
          Menu_Complete_Settings = true;
          Serial.println(F("Start key is pressed"));
          lcd.clear();
          Activate_Menu_Option_Settings();
          }
        if(!Plus_Key_X) {
          Serial.println(F("+ key is pressed"));
          Menu_View = Menu_View - 1;
          Run_Menu_Order_Settings();
          }
        if(!Minus_Key_X) {
          Menu_View = Menu_View + 1;
          Run_Menu_Order_Settings();
        }
        if(!Stop_Key_X){
          Serial.println(F("Stop key is pressed"));
          Menu_Complete_Settings = true;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Uscita Settaggi");
          delay(1000);
          lcd.clear();          
          Menu_Mode_Selection = -1;
          }
      }
    }


   
 
 void Run_Menu_Order_Settings() {
     if (Menu_View > Max_Options_Settings) Menu_View = Menu_View -1;
     if (Menu_View < 0) Menu_View = Menu_View + 1;     
     Serial.print(F("- key is pressed "));
     lcd.clear();
     lcd.setCursor(1,0);
     Print_LCD_Menu_Settings(Menu_View);
     lcd.setCursor(1,1);
     Print_LCD_Menu_Settings( Menu_View + 1);
     lcd.setCursor(0,0);
     lcd.print(">");
     Menu_Mode_Selection = Menu_View;
     Serial.print(F("Menu View : "));
     Serial.print(Menu_View);
     Serial.print(F("| Menu Selection"));
     Serial.println(Menu_Mode_Selection);
     delay(100);
     }


void Activate_Menu_Option_Settings() {

      

      if (Menu_Mode_Selection == 1) {
        lcd.clear();
        lcd.print("Setup Alarmi");
        Serial.println(F("Alarms Setup Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        lcd.clear();
        Print_Membrane_Switch_Input_Alarms();
        Menu_Mode_Selection = -1;      // skips the re-writing of the menu
        }


      if (Menu_Mode_Selection == 2) {
        lcd.clear();
        lcd.print("Setup Sensori");
        Serial.println(F("Sensor Setup Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        lcd.clear();
        Print_Membrane_Switch_Input_Sensors();
        }


      if (Menu_Mode_Selection == 3) {
        lcd.clear();
        lcd.print("Setup Movimento");
        Serial.println(F("Motion Setup Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        lcd.clear();
        Print_Membrane_Switch_Input_Motion();
        }

  
      if (Menu_Mode_Selection == 4) {
        lcd.clear();
        lcd.print("Setup Tracciatur");
        Serial.println(F("Tracking Setup Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        lcd.clear();
        Print_Membrane_Switch_Input_Tracking();
        //Menu_Mode_Selection = -1;      // skips the re-writing of the menu
        }

   
      if (Menu_Mode_Selection == 5) {
        lcd.clear();
        lcd.print("Setup Navigaz");
        Serial.println(F("NAVISetup Selected"));
        Menu_Mode_Selection = 0;
        delay(1000);
        lcd.clear();
        Print_Membrane_Switch_Input_NAVI();
        //Menu_Mode_Selection = -1;      // skips the re-writing of the menu
        }
          
 
      if (Menu_Mode_Selection == 7) {
        lcd.clear();
        lcd.print("BETA Commands");
        Serial.println(F("BETA Section"));
        Menu_Mode_Selection = 0;
        delay(1000);
        lcd.clear();
        Print_Membrane_Switch_Input_BETA();
        //Menu_Mode_Selection = -1;      // skips the re-writing of the menu
        }     



// Menu Option to CLEAR EEPROM
//-----------------------------
     
      if (Menu_Mode_Selection == 10) {
       // EEPORM Clear MENU
       lcd.clear();
       lcd.print("Cancel EEPROM?");
       lcd.setCursor(0,1);
       lcd.print("Si/No");
       Serial.println(F("Clear EEPROM Yes/No   - Press Up for Yes and Down for No.  Start to Confirm"));
       Menu_Mode_Selection = 0;
       Menu_Complete = false;
       int Answer = 0;
       while (Menu_Complete == false) {
          //Enter Code Here to Cycle until stop key is pressed.
          Read_Membrane_Keys();
          delay(100);
           if(!Start_Key_X){
             Serial.println(F("Stop key is pressed"));
             Menu_Complete = true;
             lcd.clear();
             lcd.setCursor(0,0);
             if (Answer == 1) {
              Clear_EERPOM();
              lcd.clear();
              lcd.print("EEPROM Cancellat");
              lcd.setCursor(0,1);
              lcd.print("OFF Tagliaerba");
              }
             if (Answer == 0) {
              lcd.clear();
              lcd.print("Cancellato");
              lcd.setCursor(0,1);
              }
             delay(2000);
             lcd.clear();          
             Menu_Mode_Selection = 0;
             }
            if (!Plus_Key_X) {
               Answer = 1;
               lcd.setCursor(0,1);
               lcd.print("Si    ");
               Serial.println(F("Clear EEPROM = YES....  Press Start to Confirm"));
               delay(100);
               }
             if (!Minus_Key_X) {
               Answer = 0;
               lcd.setCursor(0,1);
               lcd.print("No     ");               
               Serial.println(F("Clear EEPROM = No....Press Start to Confirm"));
               delay(100);
               }
       }
     }

  Print_Membrane_Switch_Input_Settings();
  }
