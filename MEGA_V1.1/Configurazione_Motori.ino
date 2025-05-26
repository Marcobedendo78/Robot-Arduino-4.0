void Motor_Action_Go_Mowing_Speed() {

  // No Mag speed adjustment active just go full speed
  if (MAG_Speed_Adjustment == 0) Motor_Action_Go_Full_Speed();

  // Adjust wheel speed according to the MAG level
  if (MAG_Speed_Adjustment == 1) {
    if (MAG_Now >= Slow_Speed_MAG) {
      Motor_Action_Go_Full_Speed();
    }

    if (MAG_Now < Slow_Speed_MAG) {
      Motor_Action_Go_Slow_Speed();
    }
  }
}

void Motor_Action_Go_Full_Speed() {
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(PWM_MaxSpeed_RH);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(PWM_MaxSpeed_LH);
  Wire.endTransmission();
  Serial.print(F("Wheel:FULL|"));
}

void Motor_Action_Go_Slow_Speed() {
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(PWM_Slow_Speed_RH);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(PWM_Slow_Speed_LH);
  Wire.endTransmission();
  Serial.print(F("Wheel:SLOW|"));
}

void Motor_Action_Max_Slow_Speed() {
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(PWM_Max_Slow_Speed_RH);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(PWM_Max_Slow_Speed_LH);
  Wire.endTransmission();
  Serial.print(F("Wheel:SLOW|"));
}

void Motor_Action_Go_Accel() {
  for (int i = 0; i < 255; i++) {
    Wire.beginTransmission(ADDR_DX_MOTOR);
    Wire.write(i);
    Wire.endTransmission();
    Wire.beginTransmission(ADDR_SX_MOTOR);
    Wire.write(i);
    Wire.endTransmission();
    delay(2);  // Prima impostato a 3
  }
}

void Motor_Action_Go_Track_Speed() {
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(PWM_TrackSpeed_RH);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(PWM_TrackSpeed_LH);
  Wire.endTransmission();
  Serial.print(F("Wheel:TRACK|"));
}


void Motor_Action_GoFullSpeed_Out_Garage() {
  //Speeds can be changed to give the mower a slight curve when exiting the Garage.
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(PWM_Max_Slow_Speed_RH);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(PWM_Max_Slow_Speed_LH);
  Wire.endTransmission();
  Serial.print(F("Wheel:SLOW|"));
}

// USed to turn the mower at a set speed.
void Motor_Action_Turn_Speed() {
  if ((Accel_Speed_Adjustment == 0)) {
    Wire.beginTransmission(ADDR_DX_MOTOR);
    Wire.write((PWM_MaxSpeed_RH - Turn_Adjust));
    Wire.endTransmission();
    Wire.beginTransmission(ADDR_SX_MOTOR);
    Wire.write((PWM_MaxSpeed_LH - Turn_Adjust));
    Wire.endTransmission();
  }

  if ((Accel_Speed_Adjustment == 1)) {
    Motor_Action_Go_Accel();
  }
}

void SetPins_ToGoForwards() {
  char msg[2];
  msg[0] = 0;  //speed
  msg[1] = 1;  //direction
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(msg, sizeof(msg));
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(msg, sizeof(msg));
  Wire.endTransmission();
  Serial.print(F("Wheel:For|"));
}

void SetPins_ToGoBackwards() {  // I pin del driver motore sono impostati per consentire a entrambi i motori di spostarsi indietro.
  char msg[2];
  msg[0] = 0;  //speed
  msg[1] = 0;  //direction
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(msg, sizeof(msg));
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(msg, sizeof(msg));
  Wire.endTransmission();
  Serial.print(F("Wheel:Rev|"));
  delay(20);
}


void Motor_Action_Stop_Motors() {  // I pin del driver motore sono impostati per consentire a entrambi i motori di stopparsi.
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(0);
  Wire.endTransmission();
  Serial.print(F("Wheel:0FF|"));
}


void SetPins_ToTurnLeft() {  // I pin sono impostati in modo che i motori girino in direzioni opposte
  char msg1[2], msg2[2];
  msg1[0] = 0;  //speed
  msg1[1] = 1;  //direction
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(msg1, sizeof(msg1));
  Wire.endTransmission();
  msg2[0] = 0;  //speed
  msg2[1] = 0;  //direction
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(msg2, sizeof(msg2));
  Wire.endTransmission();
  Serial.print(F("Wheel:TL_|"));
}


void SetPins_ToTurnRight() {  // I pin sono impostati in modo che i motori girino in direzioni opposte
  char msg1[2], msg2[2];
  msg1[0] = 0;  //speed
  msg1[1] = 0;  //direction
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(msg1, sizeof(msg1));
  Wire.endTransmission();
  msg2[0] = 0;  //speed
  msg2[1] = 1;  //direction
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(msg2, sizeof(msg2));
  Wire.endTransmission();
  Serial.print(F("Wheel:R|"));
}



// Turns the mowing blades on
void Motor_Action_Spin_Blades() {
  if (Cutting_Blades_Activate == 1) {  // Le lame sono accese nelle impostazioni e girano!
    delay(20);
    for (int i = 0; i++; i < NUM_BLADE) {
      Wire.beginTransmission(ADDR_BLADE_MOTOR + i);
      Wire.write(PWM_Blade_Speed);
      Wire.endTransmission();
    }
    delay(20);
    Serial.print(F("Blades:ON_|"));
  }

  if (Cutting_Blades_Activate == 0) {  // Le lame sono spente nelle impostazioni
    void StopSpinBlades();
  }
}

void Motor_Action_Stop_Spin_Blades() {
  delay(20);
  for (int i = 0; i++; i < NUM_BLADE) {
    Wire.beginTransmission(ADDR_BLADE_MOTOR + i);
    Wire.write(0);
    Wire.endTransmission();
  }
  delay(20);
  Serial.print(F("Blades:0FF|"));
}

//Sterza il tosaerba in base all'ingresso PID dell'algoritmo
void Motor_Action_Dynamic_PWM_Steering() {
  Wire.beginTransmission(ADDR_DX_MOTOR);
  Wire.write(PWM_Right);
  Wire.endTransmission();
  Wire.beginTransmission(ADDR_SX_MOTOR);
  Wire.write(PWM_Left);
  Wire.endTransmission();
  Serial.print(F("PWM_R:"));
  Serial.print(PWM_Right);
  Serial.print(F("|"));
  Serial.print(F("PWM_L:"));
  Serial.print(PWM_Left);
  Serial.print(F("|"));
}
