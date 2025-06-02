void Motor_Action_Go_Mowing_Speed() {
  
  // No Mag speed adjustment active just go full speed
  if (MAG_Speed_Adjustment == 0) Motor_Action_Go_Full_Speed();  
  
  // Adjust wheel speed according to the MAG level
  if (MAG_Speed_Adjustment == 1) {
          if (MAG_Now >= Slow_Speed_MAG) {
            Motor_Action_Go_Full_Speed(); 
            }
        
          if (MAG_Now < Slow_Speed_MAG)  {
            Motor_Action_Go_Slow_Speed(); 
            }
    }
}

void Motor_Action_Go_Full_Speed()     {
      analogWrite(ENAPin, PWM_MaxSpeed_RH);                          // Velocità = 0-255  (255 è la velocità massima). La velocità è impostata nelle impostazioni
      analogWrite(ENBPin, PWM_MaxSpeed_LH);                          // AnaolgWrite invia segnali PWM Velocità = 0-255  (255 è la velocità massima)
      Serial.print(F("Wheel:FULL|"));
      }  

void Motor_Action_Go_Slow_Speed()     {
      analogWrite(ENAPin,  PWM_Slow_Speed_RH);                       // Velocità = 0-255  (255 è la velocità massima). La velocità è impostata nelle impostazioni                        
      analogWrite(ENBPin,  PWM_Slow_Speed_LH);                       // AnaolgWrite invia segnali PWM Velocità = 0-255  (255 è la velocità massima)
      Serial.print(F("Wheel:SLOW|"));
      }  

void Motor_Action_Max_Slow_Speed()     {
      analogWrite(ENAPin,  PWM_Max_Slow_Speed_RH);                   // Velocità = 0-255  (255 è la velocità massima). La velocità è impostata nelle impostazioni                        
      analogWrite(ENBPin,  PWM_Max_Slow_Speed_LH);                   // AnaolgWrite invia segnali PWM Velocità = 0-255  (255 è la velocità massima)
      Serial.print(F("Wheel:SLOW|"));
      }

void Motor_Action_Go_Accel()    {                                                     
      for (int i = 0; i < 255; i ++){
        analogWrite(ENAPin, i);
        analogWrite(ENBPin, i);
        delay(2);                                                    // Prima impostato a 3
       }
   }

void Motor_Action_Go_Track_Speed()     {
      analogWrite(ENAPin, PWM_TrackSpeed_RH);                          // Velocità = 0-255  (255 è la velocità massima). La velocità è impostata nelle impostazioni
      analogWrite(ENBPin, PWM_TrackSpeed_LH);                          // AnaolgWrite invia segnali PWM Velocità = 0-255  (255 è la velocità massima)
      Serial.print(F("Wheel:TRACK|"));
      }
   

void Motor_Action_GoFullSpeed_Out_Garage()     {
  //Speeds can be changed to give the mower a slight curve when exiting the Garage.
      analogWrite(ENAPin, PWM_Max_Slow_Speed_RH);                                       // Velocità lenta
      analogWrite(ENBPin, PWM_Max_Slow_Speed_LH);   
      Serial.print(F("Wheel:SLOW|"));
      }


// USed to turn the mower at a set speed.
void Motor_Action_Turn_Speed() {
     if ( (Accel_Speed_Adjustment == 0) ) {
      analogWrite(ENAPin, (PWM_MaxSpeed_RH - Turn_Adjust) );                                  // Cambia il valore 0 in 10 o 20 per ridurre la velocità
      analogWrite(ENBPin, (PWM_MaxSpeed_LH - Turn_Adjust) );                                  // Cambia il valore 0 in 10 o 20 per ridurre la velocità 
        }
        
          if ( (Accel_Speed_Adjustment == 1) ) {
            Motor_Action_Go_Accel(); 
            }
      }


void SetPins_ToGoForwards()     {                                     // I pin del driver motore sono impostati per consentire a entrambi i motori di spostarsi in avanti.
  digitalWrite(IN1Pin, LOW);                                          // I pin del driver motore sono impostati su alto o basso per impostare la direzione del movimento
  digitalWrite(IN2Pin, HIGH);
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, HIGH);
  Serial.print(F("Wheel:For|"));
  }


void SetPins_ToGoBackwards()      {                               // I pin del driver motore sono impostati per consentire a entrambi i motori di spostarsi indietro.
  digitalWrite(IN1Pin, HIGH);                                     // Motor 1
  digitalWrite(IN2Pin, LOW);
  digitalWrite(IN3Pin, HIGH);                                     // Motor 2
  digitalWrite(IN4Pin, LOW);
  Serial.print(F("Wheel:Rev|"));
  delay(20);
  }


void Motor_Action_Stop_Motors()  {                                 // I pin del driver motore sono impostati per consentire a entrambi i motori di stopparsi.
  digitalWrite(ENAPin, 0);
  digitalWrite(IN1Pin, LOW);                                       //Motor 1
  digitalWrite(IN2Pin, LOW);

  digitalWrite(ENBPin, 0);                                         //Motor 2
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, LOW);

  Serial.print(F("Wheel:0FF|"));
}


void SetPins_ToTurnLeft()       {                                  // I pin sono impostati in modo che i motori girino in direzioni opposte
    digitalWrite(IN1Pin, LOW);                                     // Motor 1
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);                                    // Motor 2
    digitalWrite(IN4Pin, LOW);
    Serial.print(F("Wheel:TL_|"));  
    }


void SetPins_ToTurnRight() {                                       // I pin sono impostati in modo che i motori girino in direzioni opposte
      digitalWrite(IN1Pin, HIGH);                                  // Motor 1
      digitalWrite(IN2Pin, LOW);
      digitalWrite(IN3Pin, LOW);                                   //Motor 2
      digitalWrite(IN4Pin, HIGH);
      Serial.print(F("Wheel:R|"));
      }



// Turns the mowing blades on
void Motor_Action_Spin_Blades()  {
  if (Cutting_Blades_Activate == 1) {                                       // Le lame sono accese nelle impostazioni e girano!
    delay(20);
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
    delay(20);
    analogWrite(RPWM, PWM_Blade_Speed);
    delay(20);
    Serial.print(F("Blades:ON_|"));
   }                 

  if (Cutting_Blades_Activate == 0) {                                     // Le lame sono spente nelle impostazioni
    void StopSpinBlades();

  }
}

void Motor_Action_Stop_Spin_Blades()  {
  delay(20);
  digitalWrite(R_EN, LOW);
  digitalWrite(L_EN, LOW);
  delay(20);
  Serial.print(F("Blades:0FF|"));
}

//Sterza il tosaerba in base all'ingresso PID dell'algoritmo
void Motor_Action_Dynamic_PWM_Steering() {
      analogWrite(ENAPin, PWM_Right);                             // ENA low = Sterzata a destra   ENB low = Sterzata a sinistra
      analogWrite(ENBPin, PWM_Left);
      Serial.print(F("PWM_R:"));
      Serial.print(PWM_Right);
      Serial.print(F("|"));
      Serial.print(F("PWM_L:"));
      Serial.print(PWM_Left);
      Serial.print(F("|"));
}
/*
float targetHeading = 90.0;  // Direzione desiderata in gradi (può essere aggiornata dopo ogni manovra)

// === PID BASSATO SULLA BUSSOLA ===
void Controllo_PID_Bussola(float headingAttuale) {
  static float pid_error = 0, pid_lastError = 0, pid_integral = 0, pid_derivative = 0;
  int baseSpeed = 150;

  pid_error = headingAttuale - targetHeading;
  if (pid_error > 180) pid_error -= 360;
  if (pid_error < -180) pid_error += 360;

  pid_integral += pid_error;
  pid_derivative = pid_error - pid_lastError;
  pid_lastError = pid_error;

  float correzione = Kp * pid_error + Ki * pid_integral + Kd * pid_derivative;

  int pwmLeft = constrain(baseSpeed - correzione, 0, 255);
  int pwmRight = constrain(baseSpeed + correzione, 0, 255);

  analogWrite(ENAPin, pwmRight);
  analogWrite(ENBPin, pwmLeft);

  Serial.print(F("PID|H:"));
  Serial.print(headingAttuale);
  Serial.print(F("|Err:"));
  Serial.println(pid_error);
}
*/

float targetHeading = 90.0;  // Direzione desiderata in gradi (può essere aggiornata dopo ogni manovra)

// === PID BASSATO SUL GYRO ===
void Controllo_PID_Gyro(float headingAttuale) {
  static float pid_error = 0, pid_lastError = 0, pid_integral = 0, pid_derivative = 0;
  int baseSpeed = 150;

  pid_error = headingAttuale - targetHeading;
  if (pid_error > 180) pid_error -= 360;
  if (pid_error < -180) pid_error += 360;

  pid_integral += pid_error;
  pid_derivative = pid_error - pid_lastError;
  pid_lastError = pid_error;

  float correzione = Kp * pid_error + Ki * pid_integral + Kd * pid_derivative;

  int pwmLeft = constrain(baseSpeed - correzione, 0, 255);
  int pwmRight = constrain(baseSpeed + correzione, 0, 255);

  analogWrite(ENAPin, pwmRight);
  analogWrite(ENBPin, pwmLeft);

  Serial.print(F("PID|H:"));
  Serial.print(headingAttuale);
  Serial.print(F("|Err:"));
  Serial.println(pid_error);
}
