// BUTTONS ALARMS MENU


// Test to displyed on the LCD screen when using the membrane key menus
void Print_LCD_Menu_Alarms(byte LCD_Menu_Alarms) {
  if (LCD_Menu_Alarms == 1) lcd.print(F("Set Alarm 1"));
  if (LCD_Menu_Alarms == 2) lcd.print(F("Set Alarm 2"));
  if (LCD_Menu_Alarms == 3) lcd.print(F("Set Alarm 3"));
  if (LCD_Menu_Alarms == 4) lcd.print(F("Set Alarm 4"));
  if (LCD_Menu_Alarms == 5) lcd.print(F("Set Alarm 5"));
  if (LCD_Menu_Alarms == 6) lcd.print(F("Set Alarm 6"));
  if (LCD_Menu_Alarms == 7) lcd.print(F("Set Orario"));
  Max_Options_Alarms = 7;
}


void Print_Membrane_Switch_Input_Alarms() {
  Read_Membrane_Keys();
  Menu_Complete_Alarms = 1;
  Menu_Mode_Selection = 0;
  Menu_View = 0;

  Serial.println();
  Serial.println(F("Alarms Menu Activated"));
  Menu_Complete_Alarms = false;  // Menu complete will return to the normal loop
  lcd.clear();
  delay(5);

  while (Menu_Complete_Alarms == false) {  // holds the program in a loop until a selection has been made in the membrane button menu
    if (Menu_View == 0) {
      lcd.setCursor(1, 0);
      Print_LCD_Menu_Alarms(1);
      lcd.setCursor(1, 1);
      Print_LCD_Menu_Alarms(2);
    }
    // Gets the values again from the keys
    Read_Membrane_Keys();
    delay(100);

    if (!Start_Key_X) {
      Menu_Complete_Alarms = true;
      Serial.println(F("Start key is pressed"));
      Activate_Menu_Option_Alarms();
      lcd.clear();
    }
    if (!Plus_Key_X) {
      Serial.println(F("+ key is pressed"));
      Menu_View = Menu_View - 1;
      Run_Menu_Order_Alarms();
    }
    if (!Minus_Key_X) {
      Menu_View = Menu_View + 1;
      Run_Menu_Order_Alarms();
    }
    if (!Stop_Key_X) {
      Serial.println(F("Stop key is pressed"));
      Menu_Complete_Alarms = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Exit Alarms");
      delay(1000);
      lcd.clear();
    }
  }
}




// Code to scroll the menu and print the menu options in the LCD
void Run_Menu_Order_Alarms() {
  if (Menu_View > Max_Options_Alarms) Menu_View = Menu_View - 1;
  if (Menu_View < 0) Menu_View = Menu_View + 1;
  Serial.print(F("- key is pressed "));
  lcd.clear();
  lcd.setCursor(1, 0);
  Print_LCD_Menu_Alarms(Menu_View);
  lcd.setCursor(1, 1);
  Print_LCD_Menu_Alarms(Menu_View + 1);
  lcd.setCursor(0, 0);
  lcd.print(">");
  Menu_Mode_Selection = Menu_View;
  Serial.print(F("Menu View : "));
  Serial.print(Menu_View);
  Serial.print(F("| Menu Selection"));
  Serial.println(Menu_Mode_Selection);
  delay(100);
}


// Defines the actions when that option is selected with the keypad.
void Activate_Menu_Option_Alarms() {


  if (Menu_Mode_Selection == 1) {
    // Alarm 1 Settings

    int Set = 1;
    Menu_Mode_Selection = 1;
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F(">Alarm 1: "));
    if (Alarm_1_ON == 1) lcd.print("ON");
    if (Alarm_1_ON == 0) lcd.print("OFF");
    lcd.setCursor(0, 1);
    lcd.print(" Time: ");
    lcd.print(Alarm_1_Hour);
    lcd.print(F(":"));
    if (Alarm_1_Minute < 10) lcd.print("0");
    lcd.print(Alarm_1_Minute);


    Menu_Complete_Alarms = false;
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(100);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {

        if (Set == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Alarm 1"));
          lcd.setCursor(0, 1);
          lcd.print(F("Settings Saved"));
          delay(2000);
          EEPROM.write(1, 1);
          EEPROM.write(2, Alarm_1_Hour);
          EEPROM.write(3, Alarm_1_Minute);
          EEPROM.write(4, Alarm_1_ON);
          EEPROM.write(87, Alarm_1_Action);
          Menu_Mode_Selection = 0;
          Menu_Complete_Alarms = true;
        }

        if (Set == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_1_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_1_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_1_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_1_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_1_Action == 5) lcd.print("Custom         ");
          Set = Set + 1;
        }

        if (Set == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          lcd.print("<");
          Set = Set + 1;
        }

        if (Set == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          Set = Set + 1;
        }
      }

      if (!Plus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_1_ON = 1;
          if (Alarm_1_ON > 1) Alarm_1_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_1_Hour = Alarm_1_Hour + 1;
          if (Alarm_1_Hour > 24) Alarm_1_Hour = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
        }
        if (Set == 3) {
          Alarm_1_Minute = Alarm_1_Minute + 1;
          if (Alarm_1_Minute > 60) Alarm_1_Minute = 0;
          if (Alarm_1_Minute < 0) Alarm_1_Minute = 59;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          lcd.clear();
          Alarm_1_Action = Alarm_1_Action + 1;
          if (Alarm_1_Action > 5) Alarm_1_Action = 1;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_1_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_1_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_1_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_1_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_1_Action == 5) lcd.print("Custom         ");
        }
      }

      if (!Minus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_1_ON = 0;
          if (Alarm_1_ON > 1) Alarm_1_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_1_Hour = Alarm_1_Hour - 1;
          if (Alarm_1_Hour < 0) Alarm_1_Hour = 23;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_1_Minute = Alarm_1_Minute - 1;
          if (Alarm_1_Minute < 0) Alarm_1_Minute = 59;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 1: "));
          if (Alarm_1_ON == 1) lcd.print("ON");
          if (Alarm_1_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          Alarm_1_Action = Alarm_1_Action - 1;
          if (Alarm_1_Action < 1) Alarm_1_Action = 5;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_1_Hour);
          lcd.print(F(":"));
          if (Alarm_1_Minute < 10) lcd.print("0");
          lcd.print(Alarm_1_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_1_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_1_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_1_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_1_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_1_Action == 5) lcd.print("Custom         ");
        }
      }
    }
  }




  if (Menu_Mode_Selection == 2) {

    // Alarm 2 Settings

    int Set = 1;
    Menu_Mode_Selection = 1;
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F(">Alarm 2: "));
    if (Alarm_2_ON == 1) lcd.print("ON");
    if (Alarm_2_ON == 0) lcd.print("OFF");
    lcd.setCursor(0, 1);
    lcd.print(" Time: ");
    lcd.print(Alarm_2_Hour);
    lcd.print(F(":"));
    if (Alarm_2_Minute < 10) lcd.print("0");
    lcd.print(Alarm_2_Minute);


    Menu_Complete_Alarms = false;
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(100);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {

        if (Set == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Alarm 2"));
          lcd.setCursor(0, 1);
          lcd.print(F("Settings Saved"));
          delay(2000);
          EEPROM.write(5, 1);
          EEPROM.write(6, Alarm_2_Hour);
          EEPROM.write(7, Alarm_2_Minute);
          EEPROM.write(8, Alarm_2_ON);
          EEPROM.write(88, Alarm_2_Action);
          Menu_Mode_Selection = 0;
          Menu_Complete_Alarms = true;
        }

        if (Set == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_2_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_2_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_2_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_2_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_2_Action == 5) lcd.print("Custom         ");
          Set = Set + 1;
        }

        if (Set == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          lcd.print("<");
          Set = Set + 1;
        }

        if (Set == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          Set = Set + 1;
        }
      }

      if (!Plus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_2_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_2_Hour = Alarm_2_Hour + 1;
          if (Alarm_2_Hour > 24) Alarm_2_Hour = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_2_Minute = Alarm_2_Minute + 1;
          if (Alarm_2_Minute > 60) Alarm_2_Minute = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          lcd.clear();
          Alarm_2_Action = Alarm_2_Action + 1;
          if (Alarm_2_Action > 5) Alarm_2_Action = 1;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_2_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_2_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_2_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_2_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_2_Action == 5) lcd.print("Custom         ");
        }
      }

      if (!Minus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_2_ON = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_2_Hour = Alarm_2_Hour - 1;
          if (Alarm_2_Hour < 0) Alarm_2_Hour = 23;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_2_Minute = Alarm_2_Minute - 1;
          if (Alarm_2_Minute < 0) Alarm_2_Minute = 59;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 2: "));
          if (Alarm_2_ON == 1) lcd.print("ON");
          if (Alarm_2_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          Alarm_2_Action = Alarm_2_Action - 1;
          if (Alarm_2_Action < 1) Alarm_2_Action = 5;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_2_Hour);
          lcd.print(F(":"));
          if (Alarm_2_Minute < 10) lcd.print("0");
          lcd.print(Alarm_2_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_2_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_2_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_2_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_2_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_2_Action == 5) lcd.print("Custom         ");
        }
      }
    }
  }



  if (Menu_Mode_Selection == 3) {

    // Alarm 3 Settings

    int Set = 1;
    Menu_Mode_Selection = 1;
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F(">Alarm 3: "));
    if (Alarm_3_ON == 1) lcd.print("ON");
    if (Alarm_3_ON == 0) lcd.print("OFF");
    lcd.setCursor(0, 1);
    lcd.print(" Time: ");
    lcd.print(Alarm_3_Hour);
    lcd.print(F(":"));
    if (Alarm_3_Minute < 10) lcd.print("0");
    lcd.print(Alarm_3_Minute);


    Menu_Complete_Alarms = false;
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(100);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {

        if (Set == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Alarm 3"));
          lcd.setCursor(0, 1);
          lcd.print(F("Settings Saved"));
          delay(2000);
          EEPROM.write(9, 1);
          EEPROM.write(10, Alarm_3_Hour);
          EEPROM.write(11, Alarm_3_Minute);
          EEPROM.write(12, Alarm_3_ON);
          EEPROM.write(89, Alarm_3_Action);
          Menu_Mode_Selection = 0;
          Menu_Complete_Alarms = true;
        }

        if (Set == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_3_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_3_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_3_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_3_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_3_Action == 5) lcd.print("Custom         ");
          Set = Set + 1;
        }

        if (Set == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          lcd.print("<");
          Set = Set + 1;
        }

        if (Set == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          Set = Set + 1;
        }
      }

      if (!Plus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_3_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_3_Hour = Alarm_3_Hour + 1;
          if (Alarm_3_Hour > 24) Alarm_3_Hour = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_3_Minute = Alarm_3_Minute + 1;
          if (Alarm_3_Minute > 60) Alarm_3_Minute = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          lcd.clear();
          Alarm_3_Action = Alarm_3_Action + 1;
          if (Alarm_3_Action > 5) Alarm_3_Action = 1;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_3_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_3_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_3_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_3_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_3_Action == 5) lcd.print("Custom         ");
        }
      }

      if (!Minus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_3_ON = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_3_Hour = Alarm_3_Hour - 1;
          if (Alarm_3_Hour < 0) Alarm_3_Hour = 23;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_3_Minute = Alarm_3_Minute - 1;
          if (Alarm_3_Minute < 0) Alarm_3_Minute = 59;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_3_ON == 1) lcd.print("ON");
          if (Alarm_3_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          Alarm_3_Action = Alarm_3_Action - 1;
          if (Alarm_3_Action < 1) Alarm_3_Action = 5;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_3_Minute < 10) lcd.print("0");
          lcd.print(Alarm_3_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_3_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_3_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_3_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_3_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_3_Action == 5) lcd.print("Custom         ");
        }
      }
    }
  }



  if (Menu_Mode_Selection == 4) {

    // Alarm 4 Settings

    int Set = 1;
    Menu_Mode_Selection = 1;
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F(">Alarm 4: "));
    if (Alarm_4_ON == 1) lcd.print("ON");
    if (Alarm_4_ON == 0) lcd.print("OFF");
    lcd.setCursor(0, 1);
    lcd.print(" Time: ");
    lcd.print(Alarm_4_Hour);
    lcd.print(F(":"));
    if (Alarm_4_Minute < 10) lcd.print("0");
    lcd.print(Alarm_4_Minute);


    Menu_Complete_Alarms = false;
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(100);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {

        if (Set == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Alarm 4"));
          lcd.setCursor(0, 1);
          lcd.print(F("Settings Saved"));
          delay(2000);
          EEPROM.write(130, 1);
          EEPROM.write(131, Alarm_4_Hour);
          EEPROM.write(132, Alarm_4_Minute);
          EEPROM.write(133, Alarm_4_ON);
          EEPROM.write(134, Alarm_4_Action);
          Menu_Mode_Selection = 0;
          Menu_Complete_Alarms = true;
        }

        if (Set == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_4_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_4_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_4_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_4_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_4_Action == 5) lcd.print("Custom         ");
          Set = Set + 1;
        }

        if (Set == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          lcd.print("<");
          Set = Set + 1;
        }

        if (Set == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          Set = Set + 1;
        }
      }

      if (!Plus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_4_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_4_Hour = Alarm_4_Hour + 1;
          if (Alarm_4_Hour > 24) Alarm_4_Hour = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_4_Minute = Alarm_4_Minute + 1;
          if (Alarm_4_Minute > 60) Alarm_4_Minute = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          lcd.clear();
          Alarm_4_Action = Alarm_4_Action + 1;
          if (Alarm_4_Action > 5) Alarm_4_Action = 1;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_4_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_4_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_4_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_4_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_4_Action == 5) lcd.print("Custom         ");
        }
      }

      if (!Minus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_4_ON = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_4_Hour = Alarm_4_Hour - 1;
          if (Alarm_4_Hour < 0) Alarm_4_Hour = 23;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_3_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_4_Minute = Alarm_4_Minute - 1;
          if (Alarm_4_Minute < 0) Alarm_4_Minute = 59;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 4: "));
          if (Alarm_4_ON == 1) lcd.print("ON");
          if (Alarm_4_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          Alarm_4_Action = Alarm_3_Action - 1;
          if (Alarm_4_Action < 1) Alarm_4_Action = 5;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_4_Hour);
          lcd.print(F(":"));
          if (Alarm_4_Minute < 10) lcd.print("0");
          lcd.print(Alarm_4_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_4_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_4_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_4_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_4_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_4_Action == 5) lcd.print("Custom         ");
        }
      }
    }
  }



  if (Menu_Mode_Selection == 5) {

    // Alarm 5 Settings

    int Set = 1;
    Menu_Mode_Selection = 1;
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F(">Alarm 5: "));
    if (Alarm_5_ON == 1) lcd.print("ON");
    if (Alarm_5_ON == 0) lcd.print("OFF");
    lcd.setCursor(0, 1);
    lcd.print(" Time: ");
    lcd.print(Alarm_5_Hour);
    lcd.print(F(":"));
    if (Alarm_5_Minute < 10) lcd.print("0");
    lcd.print(Alarm_5_Minute);


    Menu_Complete_Alarms = false;
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(100);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {

        if (Set == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Alarm 5"));
          lcd.setCursor(0, 1);
          lcd.print(F("Settings Saved"));
          delay(2000);
          EEPROM.write(135, 1);
          EEPROM.write(136, Alarm_5_Hour);
          EEPROM.write(137, Alarm_5_Minute);
          EEPROM.write(138, Alarm_5_ON);
          EEPROM.write(139, Alarm_5_Action);
          Menu_Mode_Selection = 0;
          Menu_Complete_Alarms = true;
        }

        if (Set == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_5_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_5_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_5_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_5_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_5_Action == 5) lcd.print("Custom         ");
          Set = Set + 1;
        }

        if (Set == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          lcd.print("<");
          Set = Set + 1;
        }

        if (Set == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          Set = Set + 1;
        }
      }

      if (!Plus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_5_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_5_Hour = Alarm_5_Hour + 1;
          if (Alarm_5_Hour > 24) Alarm_5_Hour = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 3: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_5_Minute = Alarm_5_Minute + 1;
          if (Alarm_5_Minute > 60) Alarm_5_Minute = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          lcd.clear();
          Alarm_5_Action = Alarm_5_Action + 1;
          if (Alarm_5_Action > 5) Alarm_5_Action = 1;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_5_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_5_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_5_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_5_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_5_Action == 5) lcd.print("Custom         ");
        }
      }

      if (!Minus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_5_ON = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_5_Hour = Alarm_5_Hour - 1;
          if (Alarm_5_Hour < 0) Alarm_5_Hour = 23;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_5_Minute = Alarm_5_Minute - 1;
          if (Alarm_5_Minute < 0) Alarm_5_Minute = 59;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 5: "));
          if (Alarm_5_ON == 1) lcd.print("ON");
          if (Alarm_5_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          Alarm_5_Action = Alarm_5_Action - 1;
          if (Alarm_5_Action < 1) Alarm_5_Action = 5;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_5_Hour);
          lcd.print(F(":"));
          if (Alarm_5_Minute < 10) lcd.print("0");
          lcd.print(Alarm_5_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_5_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_5_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_5_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_5_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_5_Action == 5) lcd.print("Custom         ");
        }
      }
    }
  }



  if (Menu_Mode_Selection == 6) {

    // Alarm 6 Settings

    int Set = 1;
    Menu_Mode_Selection = 1;
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(F(">Alarm 5: "));
    if (Alarm_6_ON == 1) lcd.print("ON");
    if (Alarm_6_ON == 0) lcd.print("OFF");
    lcd.setCursor(0, 1);
    lcd.print(" Time: ");
    lcd.print(Alarm_6_Hour);
    lcd.print(F(":"));
    if (Alarm_6_Minute < 10) lcd.print("0");
    lcd.print(Alarm_6_Minute);


    Menu_Complete_Alarms = false;
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(100);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {

        if (Set == 4) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Alarm 6"));
          lcd.setCursor(0, 1);
          lcd.print(F("Settings Saved"));
          delay(2000);
          EEPROM.write(140, 1);
          EEPROM.write(141, Alarm_6_Hour);
          EEPROM.write(142, Alarm_6_Minute);
          EEPROM.write(143, Alarm_6_ON);
          EEPROM.write(144, Alarm_6_Action);
          Menu_Mode_Selection = 0;
          Menu_Complete_Alarms = true;
        }

        if (Set == 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_6_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_6_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_6_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_6_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_6_Action == 5) lcd.print("Custom         ");
          Set = Set + 1;
        }

        if (Set == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 6: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          lcd.print("<");
          Set = Set + 1;
        }

        if (Set == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 6: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          Set = Set + 1;
        }
      }

      if (!Plus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_6_ON = 1;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 6: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_6_Hour = Alarm_6_Hour + 1;
          if (Alarm_6_Hour > 24) Alarm_6_Hour = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 6: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_6_Minute = Alarm_6_Minute + 1;
          if (Alarm_6_Minute > 60) Alarm_6_Minute = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 5: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          lcd.clear();
          Alarm_6_Action = Alarm_6_Action + 1;
          if (Alarm_6_Action > 5) Alarm_6_Action = 1;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_6_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_6_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_6_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_6_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_6_Action == 5) lcd.print("Custom         ");
        }
      }

      if (!Minus_Key_X) {
        if (Set == 1) {
          lcd.clear();
          Alarm_6_ON = 0;
          lcd.setCursor(0, 0);
          lcd.print(F(">Alarm 5: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
        }
        if (Set == 2) {
          lcd.clear();
          Alarm_6_Hour = Alarm_6_Hour - 1;
          if (Alarm_6_Hour < 0) Alarm_6_Hour = 23;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 6: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time:>");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
        }
        if (Set == 3) {
          lcd.clear();
          Alarm_6_Minute = Alarm_6_Minute - 1;
          if (Alarm_6_Minute < 0) Alarm_6_Minute = 59;
          lcd.setCursor(0, 0);
          lcd.print(F(" Alarm 6: "));
          if (Alarm_6_ON == 1) lcd.print("ON");
          if (Alarm_6_ON == 0) lcd.print("OFF");
          lcd.setCursor(0, 1);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          lcd.print("<");
        }
        if (Set == 4) {
          Alarm_6_Action = Alarm_6_Action - 1;
          if (Alarm_6_Action < 1) Alarm_6_Action = 5;
          lcd.setCursor(0, 0);
          lcd.print(" Time: ");
          lcd.print(Alarm_6_Hour);
          lcd.print(F(":"));
          if (Alarm_6_Minute < 10) lcd.print("0");
          lcd.print(Alarm_6_Minute);
          lcd.setCursor(0, 1);
          lcd.print("Do:");
          if (Alarm_6_Action == 1) lcd.print("Uscita Z1      ");
          if (Alarm_6_Action == 2) lcd.print("Uscita Z2      ");
          if (Alarm_6_Action == 3) lcd.print("Taglia sul filo");
          if (Alarm_6_Action == 4) lcd.print("Partenza rapida");
          if (Alarm_6_Action == 5) lcd.print("Custom         ");
        }
      }
    }
  }




  // Settaggio orario Tagliaerba

  if (Menu_Mode_Selection == 7) {
    // Settaggio orario Tagliaerba
    Menu_Mode_Selection = 0;
    lcd.clear();
    lcd.setCursor(0, 0);  // Spaces to the right & down
    Time t = rtc.time();
    int set_hour = t.hr;
    int set_min = t.min;
    lcd.print(F("Time : "));
    lcd.print(set_hour);
    lcd.print(F(":"));
    if (set_min < 10) lcd.print("0");
    lcd.print(set_min);
    Serial.print(F("Time : "));
    Serial.print(set_hour);
    Serial.print(F(":"));
    if (set_min < 10) Serial.print("0");
    Serial.println(set_min);

    Menu_Complete_Alarms = false;
    delay(500);
    while (Menu_Complete_Alarms == false) {
      Read_Membrane_Keys();
      delay(50);
      //Enter Code Here to Cycle until stop key is pressed.
      if (!Start_Key_X) {
        Serial.println(F("Settings Saved"));
        Menu_Complete_Alarms = true;
        lcd.clear();
        lcd.print(F("Time : "));
        lcd.print(set_hour);
        lcd.print(":");
        if (set_min < 10) lcd.print("0");
        lcd.print(set_min);
        lcd.setCursor(0, 1);
        rtc.writeProtect(false);
        rtc.halt(false);
        Serial.print(F("Clock : "));
        Serial.print(set_hour);
        Serial.print(":");
        if (set_min < 10) Serial.print("0");
        Serial.println(set_min);
        Time t(2024, 03, 29, set_hour, set_min, 00, Time::kSaturday);  // Year XXXX, Month XX, Day XX, Hour XX, Minute XX, Second, kXYZday
        rtc.time(t);
        delay(200);
        rtc.writeProtect(true);
        rtc.halt(true);
        rtc.time(t);
        lcd.print(F("TIME SAVATO"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
      if (!Plus_Key_X) {
        set_min = set_min + 1;
        if (set_min > 59) {
          set_min = 0;
          set_hour = set_hour + 1;
        }
        if (set_hour > 23) set_hour = 0;
        lcd.clear();
        lcd.print(F("Time : "));
        lcd.print(set_hour);
        lcd.print(":");
        if (set_min < 10) lcd.print("0");
        lcd.print(set_min);
      }
      if (!Minus_Key_X) {
        set_min = set_min - 1;
        if (set_min < 0) {
          set_min = 59;
          set_hour = set_hour - 1;
        }
        if (set_hour < 0) set_hour = 23;
        lcd.clear();
        lcd.print(F("Time : "));
        lcd.print(set_hour);
        lcd.print(":");
        if (set_min < 10) lcd.print("0");
        lcd.print(set_min);
      }
      if (!Stop_Key_X) {
        Serial.println(F("Time Set Cancelled"));
        Menu_Complete_Alarms = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Time Set"));
        lcd.setCursor(0, 1);
        lcd.print(F("Cancellato"));
        delay(2000);
        lcd.clear();
        Menu_Mode_Selection = 0;
      }
    }
    Serial.print(F("Time : "));
    delay(1000);
  }

  if (Menu_Complete_Alarms = true) Print_Membrane_Switch_Input_Alarms();
}
