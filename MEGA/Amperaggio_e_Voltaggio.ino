void Process_Volt_Information() {
  //  Logica per come sono gestite le letture della batteria

  if (Volts < Battery_Min) {
    Low_Battery_Detected = (Low_Battery_Detected + 1);
    Serial.print(F("VLow:"));
    Serial.print(Low_Battery_Detected);
    Serial.print(F("|"));
    if (Low_Battery_Detected > Low_Battery_Instances_Chg) {
      Serial.println(F("Low Battery Detected"));
      if (Use_Charging_Station == 1) Manouver_Go_To_Charging_Station();   // Interrompe il taglio e rimanda il rasaerba alla stazione di ricarica tramite il cavo perimetrale
      if (Use_Charging_Station == 0) Manouver_Park_The_Mower_Low_Batt();  // Parcheggia il tosaerba con un avviso di batteria scarica
    }
  }

  if (Volts >= Battery_Min) {
    Serial.print(F("VLow:"));
    Serial.print(Low_Battery_Detected);
    Serial.print(F("|"));
    Low_Battery_Detected = 0;
  }
}

// controlla se il rasaerba si trova sulla stazione di ricarica
void Check_if_Charging() {

  if (Charging == 4) {  // Se il valore ricevuto è uguale a 1 o 0 come previsto, stampare il valore sul monitor seriale
    Serial.print(F("Charging:"));
    Serial.print(Charging);
    Serial.print(F("|"));
    Charge_Detected_MEGA = 1;
    Print_Charging_LCD();
    Serial.print(F("MEGA = 1|"));
  }
  if (Charging == 0) {  // Se il valore ricevuto è uguale a 1 o 0 come previsto, stampare il valore sul monitor seriale
    Serial.print(F("Charging:"));
    Serial.print(Charging);
    Serial.print(F("|"));
    Charge_Detected_MEGA = 0;
    Print_Charging_LCD();
  }
  if ((Charging != 4) && (Charging != 0)) {
    Serial.print(F("Charging:"));
    Serial.print(Charging);
    Serial.print(F("|"));
    Charge_Detected_MEGA = 0;
    Print_Charging_LCD();
  }
}

void Check_if_Docked() {

  if (Charge_Detected_MEGA == 1) {  // se gli Amp sono compresi tra questo, viene rilevata una carica. Ampere superiori a 4 sono scontati come problemi di comunicazione
    Motor_Action_Stop_Motors();
    Serial.println(F("Charging Current detected"));
    Serial.println(F("Mower Docked"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("AGGANCIATO ALLA"));
    lcd.setCursor(0, 1);
    lcd.print(F("BASE DI RICARICA"));  // Prints info to LCD display
    delay(2000);
    Mower_Docked = 1;
    Manouver_Dock_The_Mower();  // Spegne il rasaerba. E' pronto per la ricarica e il taglio di nuovo.
    lcd.clear();
  }
  Serial.println("");
}

void Calculate_Volt_Amp_Charge() {

  if (Show_TX_Data == 1) {
    Serial.print(F("Amp:"));
    Serial.print(RawValueAmp);
    Serial.print(F("|"));
    Serial.print(F("Volt:"));
    Serial.print(RawValueVolt);
    Serial.print(F("|"));
    Serial.print(F("Rain:"));
    Serial.print(Rain_Detected);
    Serial.print(F("|"));
    if (Wheel_Amp_Sensor_ON == 1) {
      Serial.print(F("WBlock:"));
      Serial.print(Wheel_Blocked);
      Serial.print(F("|"));
    }
  }

  // Calcola gli Ampere da NANO RX Data
  int mVperAmp = 185;  // Valore 66 per sensore 30A - 100 per sensore 20A - 185 per sensore 5A
  int ACSoffset = 2500;
  double VoltageAmp = 0;
  double Amps_Now = 0;
  VoltageAmp = (RawValueAmp / 1024.0) * 5000;  // Gets you mV
  Amps_Now = ((VoltageAmp - ACSoffset) / mVperAmp);
  Amps = Amps_Now;

  Serial.print(F("A:"));
  Serial.print(Amps);
  Serial.print(F("|"));


  // Calcola il Voltagio da NANO RX Data
  if (RawValueVolt > 100) {
    float vout = 0.0;
    float R1 = 30000;  // Mower 330 = 3000    Mower LAM = 30000
    float R2 = 7500;   // Mower 330 = 7500
    //float R2 = 6500;       // Mower LAM = 6500
    vout = (RawValueVolt * 5.0) / 1024.0;  // see text
    Volts = vout / (R2 / (R1 + R2));


    Volts_Last = Volts;
    Zero_Volts = 0;
  } else {
    Volts = Volts_Last;
    Zero_Volts = Zero_Volts + 1;
    if (Zero_Volts > 5) Volts = 0;
  }

  Serial.print(F("V:"));
  Serial.print(Volts);
  Serial.print(F("|"));

  if (Amps <= 0.7) Charging = 0;  //Senza caricabatterie interno era 0.4
  if (Amps > 0.7) Charging = 4;   //Senza caricabatterie interno era 0.4
}
