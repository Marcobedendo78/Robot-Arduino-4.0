void mower_setup() {
  Init_Serial();
  Init_Serial_1();
  Init_Serial_2();
  Init_I2C();
  Load_EEPROM();
  Init_RTC();
  Init_Mower();
}

void Init_Serial() {
  Serial.begin(115200);

  Serial.println(" ");
  Serial.println(" ");
  Serial.print(F("Robot Tagliaerba Arduino :"));
  Serial.println(Version);
  Serial.println(F("==================="));
  Serial.println("");
  Serial.println(F("Avvio della configurazione del tosaerba"));
  Serial.println(F("==================="));
}

void Init_Serial_1() {
  if (NANO_Serial == true) Serial1.begin(1200);  // Aprire la porta seriale 1 per la comunicazione nano
}

void Init_Serial_2() {
  if (WIFI_Enabled == true) Serial2.begin(9600);  // Se il Wi-Fi è attivo prepara la porta seriale 2 aperta per la comunicazione NodeMCU
}

void Init_I2C() {
  Wire.begin();  // avviare l'interfaccia I2C
}

void Load_EEPROM() {
  Load_EEPROM_Saved_Data();
}

void Init_RTC() {
  if (Set_Time == 1) {
    Serial.print(F("Setting Time"));
    Set_Time_On_RTC();
  }  // avviare l'interfaccia I2C
  DisplayTime();
  Serial.println("");
}

void Init_Mower() {
  Prepare_Mower_from_Settings();
  Setup_Run_LCD_Intro();
  Setup_Compass();
  delay(100);
  Setup_Relays();
  Setup_Tip_Safety();
  Setup_Membrane_Buttons();
  Setup_ADCMan();
  Setup_Check_Pattern_Mow();
  if (Bumper_Activate_Frnt == true) Setup_Bumper_Bar();
}