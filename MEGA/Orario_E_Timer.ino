// digital clock display of the time
void DisplayTime() {
  Serial.print(F("Time:"));
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.print(buf);
}

void Print_Time_On_Serial() {
  Serial.print(F("Time:"));
  Time t = rtc.time();
  Serial.print(t.hr);
  Serial.print(":");
  if (t.min < 10) Serial.print("0");
  Serial.print(t.min);
  Serial.print(".");
  if (t.sec < 10) Serial.print("0");
  Serial.print(t.sec);
}


void Activate_Alarms() {

  Time t = rtc.time();

  // Manual ALARM 1
  if (Alarm_1_ON == 1) {
    if ((t.hr == Alarm_1_Hour) && (t.min == Alarm_1_Minute)) {
      Serial.println("");
      Serial.println(F("ALARM 1"));
      delay(2000);
      // Insert action for Alarm 1 Here
      if (Alarm_1_Action == 1) {
        Blade_Override = 1;
        Exit_Zone = 1;
        Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_1_Action == 2) {
        Blade_Override = 1;
        Exit_Zone = 2;
        Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_1_Action == 3) {
        Blade_Override = 1;
        Exit_Zone = 3;
        Track_Wire_Itterations = Track_Wire_Zone_3_Cycles;
        Manouver_Exit_To_Zone_X();
      }
    }
  }


  // Manual ALARM 2
  if (Alarm_2_ON == 1) {
    if ((t.hr == Alarm_2_Hour) && (t.min == Alarm_2_Minute)) {
      Serial.println("");
      Serial.println(F("ALARM 2"));
      delay(2000);

      if (Alarm_2_Action == 1) {
        Blade_Override = 1;
        Exit_Zone = 1;
        Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_2_Action == 2) {
        Blade_Override = 1;
        Exit_Zone = 2;
        Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_2_Action == 3) {
        Blade_Override = 1;
        Exit_Zone = 3;
        Track_Wire_Itterations = Track_Wire_Zone_3_Cycles;
        Manouver_Exit_To_Zone_X();
      }
    }
  }

  // Manual ALARM 3
  if (Alarm_3_ON == 1) {
    if ((t.hr == Alarm_3_Hour) && (t.min == Alarm_3_Minute)) {
      Serial.println("");
      Serial.println(F("ALARM 3"));
      delay(2000);
      if (Alarm_3_Action == 1) {
        Blade_Override = 1;
        Exit_Zone = 1;
        Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_3_Action == 2) {
        Blade_Override = 1;
        Exit_Zone = 2;
        Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_3_Action == 3) {
        Blade_Override = 1;
        Exit_Zone = 3;
        Track_Wire_Itterations = Track_Wire_Zone_3_Cycles;
        Manouver_Exit_To_Zone_X();
      }
    }
  }

  // Manual ALARM 4
  if (Alarm_4_ON == 1) {
    if ((t.hr == Alarm_4_Hour) && (t.min == Alarm_4_Minute)) {
      Serial.println("");
      Serial.println(F("ALARM 4"));
      delay(2000);
      if (Alarm_4_Action == 1) {
        Blade_Override = 1;
        Exit_Zone = 1;
        Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_4_Action == 2) {
        Blade_Override = 1;
        Exit_Zone = 2;
        Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_4_Action == 3) {
        Blade_Override = 1;
        Exit_Zone = 3;
        Track_Wire_Itterations = Track_Wire_Zone_3_Cycles;
        Manouver_Exit_To_Zone_X();
      }
    }
  }

  // Manual ALARM 5
  if (Alarm_5_ON == 1) {
    if ((t.hr == Alarm_5_Hour) && (t.min == Alarm_5_Minute)) {
      Serial.println("");
      Serial.println(F("ALARM 3"));
      delay(2000);
      if (Alarm_5_Action == 1) {
        Blade_Override = 1;
        Exit_Zone = 1;
        Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_5_Action == 2) {
        Blade_Override = 1;
        Exit_Zone = 2;
        Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_5_Action == 3) {
        Blade_Override = 1;
        Exit_Zone = 3;
        Track_Wire_Itterations = Track_Wire_Zone_3_Cycles;
        Manouver_Exit_To_Zone_X();
      }
    }
  }

  // Manual ALARM 6
  if (Alarm_6_ON == 1) {
    if ((t.hr == Alarm_6_Hour) && (t.min == Alarm_6_Minute)) {
      Serial.println("");
      Serial.println(F("ALARM 3"));
      delay(2000);
      if (Alarm_6_Action == 1) {
        Blade_Override = 1;
        Exit_Zone = 1;
        Track_Wire_Itterations = Track_Wire_Zone_1_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_6_Action == 2) {
        Blade_Override = 1;
        Exit_Zone = 2;
        Track_Wire_Itterations = Track_Wire_Zone_2_Cycles;
        Manouver_Exit_To_Zone_X();
      }
      if (Alarm_6_Action == 3) {
        Blade_Override = 1;
        Exit_Zone = 3;
        Track_Wire_Itterations = Track_Wire_Zone_3_Cycles;
        Manouver_Exit_To_Zone_X();
      }
    }
  }
}


// Set when choosing an option of 1hr or 2hr mow etc.
void Check_Timed_Mow() {

  if (Alarm_Timed_Mow_ON == 1) {
    Time t = rtc.time();
    if ((t.hr == Alarm_Timed_Mow_Hour) && (t.min == Alarm_Timed_Mow_Minute)) {
      Serial.println(F("Timed Mow Complete"));
      delay(2000);
      //Insert action for Timed Mow Alarm Here
      if (Use_Charging_Station == 1) Manouver_Go_To_Charging_Station();   // Stops the mowing and sends the mower back to the charging station via the permieter wire
      if (Use_Charging_Station == 0) Manouver_Park_The_Mower_Low_Batt();  // Parks the mower with a low battery warning
    }
  }
}


// Prints the alarms set to the serial monitor
void Display_Next_Alarm() {
  //Print_Day();

  if (Alarm_1_ON == 1) {
    Serial.print(F("|Alarm 1:"));
    Serial.print(Alarm_1_Hour);
    Serial.print(F(":"));
    if (Alarm_1_Minute < 10) Serial.print("0");
    Serial.print(Alarm_1_Minute);
    Serial.print("|");
  }
  if (Alarm_1_ON == 0) Serial.print("|Alarm 1 OFF");

  if (Alarm_2_ON == 1) {
    Serial.print(F("|Alarm 2:"));
    Serial.print(Alarm_2_Hour);
    Serial.print(F(":"));
    if (Alarm_2_Minute < 10) Serial.print("0");
    Serial.print(Alarm_2_Minute);
    Serial.print("|");
  }
  if (Alarm_2_ON == 0) Serial.print("|Alarm 2 OFF");

  if (Alarm_3_ON == 1) {
    Serial.print(F("|Alarm 3:"));
    Serial.print(Alarm_3_Hour);
    Serial.print(F(":"));
    if (Alarm_3_Minute < 10) Serial.print("0");
    Serial.print(Alarm_3_Minute);
    Serial.print("|");
  }
  if (Alarm_3_ON == 0) Serial.print("|Alarm 3 OFF");

  if (Alarm_4_ON == 1) {
    Serial.print(F("|Alarm 4:"));
    Serial.print(Alarm_4_Hour);
    Serial.print(F(":"));
    if (Alarm_4_Minute < 10) Serial.print("0");
    Serial.print(Alarm_4_Minute);
    Serial.print("|");
  }
  if (Alarm_4_ON == 0) Serial.print("|Alarm 4 OFF");

  if (Alarm_5_ON == 1) {
    Serial.print(F("|Alarm 5:"));
    Serial.print(Alarm_5_Hour);
    Serial.print(F(":"));
    if (Alarm_5_Minute < 10) Serial.print("0");
    Serial.print(Alarm_5_Minute);
    Serial.print("|");
  }
  if (Alarm_5_ON == 0) Serial.print("|Alarm 5 OFF");

  if (Alarm_6_ON == 1) {
    Serial.print(F("|Alarm 6:"));
    Serial.print(Alarm_6_Hour);
    Serial.print(F(":"));
    if (Alarm_6_Minute < 10) Serial.print("0");
    Serial.print(Alarm_6_Minute);
    Serial.print("|");
  }
  if (Alarm_6_ON == 0) Serial.print("|Alarm 6 OFF");
}

void Set_Time_On_RTC() {
  // Set_Time to 1 in the setting menu to set time.  Load the sketch then immediatley Set_Time = 0 and reload the sketch.
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(2024, 04, 14, 12, 00, 00, Time::kSaturday);  // Year XXXX, Month XX, Day XX, Hour XX, Minute XX, Second, kXYZday
  rtc.time(t);
  delay(10);
}


void Manage_Alarms() {
  Alarm_Timed_Mow_ON = 0;  // Turns off the 1 hr Alarm
  if (Alarm_1_Repeat == 0) Alarm_1_ON = 0;
  if (Alarm_2_Repeat == 0) Alarm_2_ON = 0;
  if (Alarm_3_Repeat == 0) Alarm_3_ON = 0;
  if (Alarm_4_Repeat == 0) Alarm_4_ON = 0;
  if (Alarm_5_Repeat == 0) Alarm_5_ON = 0;
  if (Alarm_6_Repeat == 0) Alarm_6_ON = 0;
}
