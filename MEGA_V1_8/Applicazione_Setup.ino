void Print_Mower_Status() {
  //Serial.print("Mower Stat....");
  if (Mower_Docked == 1) Serial.print(F("Docked:1|"));
  if (Mower_Parked == 1) Serial.print(F("Parked:1|"));
  if (Mower_Running == 1) Serial.print(F("Running:1|"));
  if (Manuel_Mode == 1) Serial.print(F("Manuel Mode:1|"));
  if (Mower_Parked_Low_Batt == 1) Serial.print(F("Park_Low_Batt:1|"));
  if (Mower_Error == 1) Serial.print(F("Mower Error:1|"));
  
}


void Setup_Tip_Safety() {
  if (Tip_Safety == 1) {
    Calibrate_Compass_Angle();
    }
}

void Prepare_Mower_from_Settings() {

  if (Use_Charging_Station == 1) {
     Mower_Docked  = 1;
     Mower_Parked  = 0;
     Mower_Running = 0;
     }


if (Use_Charging_Station == 0) {
    Mower_Docked  = 0;
    Mower_Parked  = 1;
    Mower_Running = 0;
    }
 }


void Setup_Compass() {
 
  if (Compass_Activate == 1) {
  /*Setup Bussola
  *************************************************************************/
  lcd.clear();
  lcd.print(F("Bussola  "));
  lcd.setCursor(0, 1);
  lcd.print(F("Setup"));

    //Bussola con chip HMC5883
    while (!compass.begin()){
      Serial.println(F("Could not find a valid QMC5883 sensor, check wiring!"));
      delay(500);
      }
    if (compass.isHMC()) {
      Serial.println(F("Initialize DF Robot HMC5883 Compass"));
      lcd.setCursor(7,0);
      lcd.print(F(": HMC5883"));
      delay(500);
      compass.setRange(HMC5883L_RANGE_1_3GA);
      compass.setMeasurementMode(HMC5883L_CONTINOUS);
      compass.setDataRate(HMC5883L_DATARATE_15HZ);
      compass.setSamples(HMC5883L_SAMPLES_8);
      }
   //Bussola con chip QMC5883
    else if (compass.isQMC()) {
        Serial.println(F("Initialising DF Robot QMC5883 Compass"));
        lcd.setCursor(7,0);
        lcd.print(F(": QMC5883"));
        delay(500);
        compass.setRange(QMC5883_RANGE_2GA);
        //compass.setRange(QMC5883_RANGE_8GA);
        compass.setMeasurementMode(QMC5883_CONTINOUS);
        if (Compass_QMC_Refresh_Setting == 1) compass.setDataRate(QMC5883_DATARATE_200HZ);
        if (Compass_QMC_Refresh_Setting == 2) compass.setDataRate(QMC5883_DATARATE_100HZ);
        if (Compass_QMC_Refresh_Setting == 3) compass.setDataRate(QMC5883_DATARATE_50HZ);
        if (Compass_QMC_Refresh_Setting == 4) compass.setDataRate(QMC5883_DATARATE_10HZ);
    
        if (Compass_QMC_Sample_Setting == 1) compass.setSamples(QMC5883_SAMPLES_2);
        if (Compass_QMC_Sample_Setting == 1) compass.setSamples(QMC5883_SAMPLES_4);
        if (Compass_QMC_Sample_Setting == 2) compass.setSamples(QMC5883_SAMPLES_8);
      }
  


  lcd.setCursor(0,1); 
  lcd.print(F("Fatto!             "));
  delay(500);
  lcd.clear();
}

if (Compass_Activate == 0) {
  Serial.println(F("Compass Switched off - Select 1 in setup to switch on.")); 
  }
}


void Setup_Relays() {
  Serial.println(F("Setup Relays"));
  pinMode(Relay_Motors, OUTPUT);
  delay(5);
  Turn_Off_Relay();
  delay(5);
  }

void Setup_Motor_SX_Pins() {                                 //Modifica per driver motori nuovi
  Serial.println(F("Setup Motor SX Pins"));
  pinMode(IN3Pin, OUTPUT);
  pinMode(IN4Pin, OUTPUT);
  pinMode(ENBPin, OUTPUT);
  }

void Setup_Motor_DX_Pins() {                                  //Modifica per driver motori nuovi
  Serial.println(F("Setup Motor DX Pins"));
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(ENAPin, OUTPUT);
  }

void Setup_Motor_Pins() {
  Serial.println(F("Setup Motor Pins"));
  pinMode(L_EN, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
  }


void  Turn_On_Relay() {
   Serial.print(F("Relay:ON|"));
   digitalWrite(Relay_Motors, LOW);                         // Accende il relè per l'alimentazione
   }

void  Turn_Off_Relay() {
   Serial.print(F("Relay:Off|"));
   digitalWrite(Relay_Motors, HIGH);                         // Spegne il relè per l'alimentazione
   }

void Setup_Membrane_Buttons() {
  Serial.println(F("Setup Membrane Keys"));
  pinMode(Start_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Plus_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Minus_Key, INPUT_PULLUP);            // set pin as input
  pinMode(Stop_Key, INPUT_PULLUP);            // set pin as input  
  }


void Setup_Bumper_Bar() {
  pinMode(Bumper_Switch_Frnt_RH, INPUT_PULLUP); 
  pinMode(Bumper_Switch_Frnt_LH, INPUT_PULLUP); 
}


void Setup_ADCMan() {
  Serial.println(F("ADCMAN"));
  ADCMan.init();
  ADCMan.setCapture(pinPerimeterLeft, 1, 0);   // SX
  ADCMan.setCapture(pinPerimeterRight, 1, 1);  // DX
  perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);  
  perimeter.useDifferentialPerimeterSignal = true;   // Usa differenziale per riconoscere dentro/fuori
  perimeter.speedTest();
  ADCMan.run();
}


void Setup_Check_Pattern_Mow() {
  if (Pattern_Mow == 1) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tipo di Taglio");
    lcd.setCursor(0,1);
    lcd.print("Parallelo");
    delay(1000);
    lcd.clear();
    }
  if (Pattern_Mow == 2) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tipo di Taglio");
    lcd.setCursor(0,1);
    lcd.print("Spirale");
    delay(1000);
    lcd.clear();
    }
}



void Setup_LIDAR(){
  Serial.print(F("LIDAR:ON|"));
  pinMode(RPLIDAR_MOTOR, OUTPUT);  // set pin modes
}
    
void Setup_Gyro() {
        if (GYRO_Enabled == 1) {
        Serial.println("GYRO GY-521 Activated");
        Wire.begin();
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);  
        }
    } 
