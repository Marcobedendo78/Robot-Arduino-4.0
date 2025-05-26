// FUNZIONI BUSSOLA
//*****************************************************************************

/* Calcola la direzione della bussola come direzione e gradi della bussola di bordo */
void Get_Compass_Reading() {

  // displays a star on the LCD to show compass is being used.
  lcd.setCursor(7, 1);
  lcd.print("*");
  Vector norm = compass.readNormalize();
  delay(30);
  Heading = atan2(norm.YAxis, norm.XAxis);  // Calculate heading
  lcd.setCursor(7, 1);
  lcd.print("/");

  // Imposta l'angolo di declinazione. Trova la declinazione della tua posizione su: http://magnetic-declination.com/
  // (+) Positive or (-) for negative,
  float Declination_Angle = (4.0 + (1.0 / 60.0)) / (180 / PI);  // Declinazione magnetica Fratta Polesine +4 1'
  Heading += Declination_Angle;

  if (Heading < 0) {  // Correct for heading < 0deg and heading > 360deg
    Heading += 2 * PI;
  }
  if (Heading > 2 * PI) {
    Heading -= 2 * PI;
  }

  Compass_Heading_Degrees = Heading * 180 / M_PI;  // Converti in gradi
  //Serial.print(F("Comp H:"));
  //Serial.print(Heading);
  Serial.print(F("Comp°:"));
  Serial.print(Compass_Heading_Degrees);
  Serial.print("|");
  delay(5);
  lcd.setCursor(7, 1);
  lcd.print(" ");
  delay(100);
}







// Ruota il tosaerba nell'orientamento corretto per il percorso ottimale del cavo perimetrale
// Evitare di seguire l'intero cavo per tornare alla docking station docking
void Compass_Turn_Mower_To_Home_Direction() {
  //Ferma i motori
  Motor_Action_Stop_Motors;
  delay(2000);
  Print_LCD_Compass_Home();
  delay(1000);
  Compass_Target = Home_Wire_Compass_Heading;
  lcd.clear();

  // Invertire leggermente il tosaerba
  SetPins_ToGoBackwards();
  Motor_Action_Go_Slow_Speed();
  delay(300);  //Prima era 800 21/06/2022
  Motor_Action_Stop_Motors;
  delay(1000);
  Get_Compass_Reading();
  SetPins_ToTurnLeft();
  // Questo fa girare un po' il tosaerba per garantire che venga letta una lettura reale della bussola (calibrazione).
  SetPins_ToTurnLeft();          // Chiama la funzione motore girare a sinistra
  Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di svolta
  delay(500);
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  delay(500);
  SetPins_ToTurnLeft();          // Chiama la funzione motore girare a sinistra
  delay(100);                    // Prima era 100 27/09/2022
  Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di svolta
  delay(100);
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  delay(500);
  lcd.clear();
  lcd.print("Bussola Set");
  Motor_Action_Stop_Motors();
  Get_Compass_Reading();
  delay(2000);
  Turn_To_Compass_Heading();
}


void Turn_To_Compass_Heading() {

  Serial.println("");
  Serial.println("- - - - - - - - -");
  Serial.println(F("Compass Home 2 Activated"));
  delay(1000);

  // Print info to LCD
  lcd.setCursor(0, 0);
  lcd.print("Target: ");
  lcd.print(Compass_Target);

  // Print to Serial Monitor
  Serial.print("Compass Target : ");
  Serial.println(Compass_Target);

  // Imposta i limiti per l'errore di precisione della bussola richiesto.
  Heading_Lower_Limit_Compass = Compass_Target - 20;
  Heading_Upper_Limit_Compass = Compass_Target + 20;

  int Attemps_Compass_Turn = 0;
  float Compass_Last = 0;
  int Bad_Reading = 0;

  if (Compass_Target < 180) {

    Serial.println("Compass Target < 180 logic used");
    float Limit = Compass_Target + 180;
    Serial.print("Left Turn Limit : ");
    Serial.print(Compass_Target);
    Serial.print(" to ");
    Serial.println(Limit);

    Serial.print("Right Turn Limit : ");
    Serial.print(Limit);
    Serial.print(" to 360° & 0 to ");
    Serial.println(Compass_Target);

    Get_Compass_Reading();
    delay(500);
    Get_Compass_Reading();
    delay(500);

    while ((Compass_Heading_Degrees < Heading_Lower_Limit_Compass) || (Compass_Heading_Degrees > Heading_Upper_Limit_Compass) && (Attemps_Compass_Turn < 100) && (Bad_Reading < 5)) {

      delay(200);
      Get_Compass_Reading();

      // Ricontrolla se la lettura della bussola restituita è plausibile.
      if (Attemps_Compass_Turn > 5) {
        if (Compass_Heading_Degrees - Compass_Last > 50) {
          Serial.print("Bad Compass Reading ");
          Serial.print("Compass Heading Degrees = ");
          Serial.print(Compass_Heading_Degrees);
          Serial.print("  Compass Last = ");
          Serial.print(Compass_Last);
          Serial.print("  Degrees - Last = ");
          Serial.println(Compass_Heading_Degrees - Compass_Last);

          Bad_Reading = Bad_Reading + 1;
          lcd.setCursor(15, 1);
          lcd.print("x");
          Get_Compass_Reading;
          delay(100);
        } else {
          lcd.setCursor(15, 1);
          lcd.print("o");
        }
      }
      Compass_Last = Compass_Heading_Degrees;  // sotres the last good compass reading



      delay(100);
      Serial.print("Compass Heading : ");
      Serial.print(Compass_Heading_Degrees);
      Serial.print("|");

      if ((Compass_Heading_Degrees > Compass_Target) && (Compass_Heading_Degrees < Limit)) {
        SetPins_ToTurnLeft();
        Serial.print(F("Spin Left"));
        Serial.print(F("|"));
        delay(100);
      }
      if (((Compass_Heading_Degrees > Limit) && (Compass_Heading_Degrees < 360)) || ((Compass_Heading_Degrees > 0) && (Compass_Heading_Degrees < Compass_Target))) {
        SetPins_ToTurnRight();
        Serial.print(F("Spin Right"));
        Serial.print(F("|"));
        delay(100);
      }


      // Gira il tosaerba e ottieni la lettura della bussola
      Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
      delay(100);                    // Prima era 30 19/04/2022
      Get_Compass_Reading;
      Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
      delay(100);                    // Prima era 30 19/04/2022
      Get_Compass_Reading;
      Motor_Action_Stop_Motors();
      lcd.setCursor(0, 1);
      lcd.print(Compass_Heading_Degrees);


      Attemps_Compass_Turn = Attemps_Compass_Turn + 1;
      Serial.print("Atempts:");
      Serial.print(Attemps_Compass_Turn);
      Serial.print("|");
      Serial.println("");
    }
  }

  if (Compass_Target >= 180) {

    Serial.println("Compass Target > 180 logic used");

    float Limit = Compass_Target - 180;

    Serial.print("Left Turn Limit : ");
    Serial.print(Compass_Target);
    Serial.print(" to 360° & 0 to ");
    Serial.println(Limit);

    Serial.print("Right Turn Limit : ");
    Serial.print(Compass_Target);
    Serial.print(" to ");
    Serial.println(Limit);

    while ((Compass_Heading_Degrees < Heading_Lower_Limit_Compass) || (Compass_Heading_Degrees > Heading_Upper_Limit_Compass) && (Attemps_Compass_Turn < 100) && (Bad_Reading < 5)) {

      delay(200);
      Get_Compass_Reading();

      // Double check if the compass reading returned is plausable.
      if (Attemps_Compass_Turn > 5) {
        if (Compass_Heading_Degrees - Compass_Last > 50) {
          Serial.println("Bad Compass Reading");
          Bad_Reading = Bad_Reading + 1;
          lcd.setCursor(15, 1);
          lcd.print("x");
          Get_Compass_Reading;
          delay(100);
        } else {
          lcd.setCursor(15, 1);
          lcd.print("o");
        }
      }
      Compass_Last = Compass_Heading_Degrees;  // memorizzare l'ultima lettura valida della bussola



      delay(100);
      Serial.print("Compass Heading : ");
      Serial.print(Compass_Heading_Degrees);
      Serial.print("|");

      if (((Compass_Heading_Degrees > Compass_Target) && (Compass_Heading_Degrees < 360)) || ((Compass_Heading_Degrees > 0) && (Compass_Heading_Degrees < Limit))) {
        SetPins_ToTurnLeft();
        Serial.print(F("Spin Left"));
        Serial.print(F("|"));
        delay(100);
      }
      if ((Compass_Heading_Degrees > Limit) && (Compass_Heading_Degrees < Compass_Target)) {
        SetPins_ToTurnRight();
        Serial.print(F("Spin Right"));
        Serial.print(F("|"));
        delay(100);
      }

      Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
      delay(100);                    // Prima era 30 19/04/2022
      Get_Compass_Reading;
      delay(100);
      Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
      delay(100);                    // Prima era 30 19/04/2022
      Get_Compass_Reading;
      Motor_Action_Stop_Motors();

      Attemps_Compass_Turn = Attemps_Compass_Turn + 1;
      Serial.print("Atempts:");
      Serial.print(Attemps_Compass_Turn);
      Serial.print("|");
      Serial.println("");
    }
  }

  if (Bad_Reading > 5) {
    lcd.clear();
    lcd.print("Bad Compass Readings");
    lcd.setCursor(0, 1);
    lcd.print("Riparti");
    delay(1000);
    SetPins_ToTurnLeft();          // Richiama la funzione di girare a sinistra
    Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
    delay(100);                    // Prima era 30 19/04/2022
    Get_Compass_Reading;
    SetPins_ToTurnLeft();          // Richiama la funzione di girare a sinistra
    Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
    delay(100);                    // Prima era 30 19/04/2022
    Get_Compass_Reading;
    SetPins_ToTurnLeft();          // Richiama la funzione di girare a sinistra
    Motor_Action_Go_Slow_Speed();  // Imposta la velocità del movimento di rotazione
    delay(100);                    // Prima era 30 19/04/2022
    Get_Compass_Reading;
    Turn_To_Compass_Heading();
  }
}


void Display_Compass_Current_Heading_on_LCD() {
  lcd.setCursor(5, 1);
  lcd.print("    ");
  lcd.print(Compass_Heading_Degrees);
  delay(10);
}


void Calculate_Compass_Wheel_Compensation() {

  float Compass_Error = Compass_Heading_Degrees - Heading_Lock;  // Calcola l'errore nella direzione della bussola dalla direzione bloccata salvata

  if (Compass_Error > 180) Compass_Error = Compass_Error * -1;
  if (Compass_Error < -180) Compass_Error = Compass_Error * -1;
  Serial.print(F("C_Err:"));
  Serial.print(Compass_Error);
  Serial.print(F("|"));

  if (Compass_Error < 0) {  // Sterzare a sinistra
    Serial.print(F("SR|"));


    // With no adjustment of wheel speed according to MAG Level
    if (MAG_Speed_Adjustment == 0) {
      PWM_Left = PWM_MaxSpeed_LH;
      PWM_Right = PWM_MaxSpeed_RH + (CPower * Compass_Error);  // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
      if (PWM_Right < 0) PWM_Right = PWM_MaxSpeed_RH - 50;
    }

    if (MAG_Speed_Adjustment == 1) {
      // MAX MAG Speed Left Hand Wheel
      if (MAG_Now >= Slow_Speed_MAG) {
        PWM_Left = PWM_MaxSpeed_LH;
        PWM_Right = PWM_MaxSpeed_RH + (CPower * Compass_Error);  // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Right < 0) PWM_Right = PWM_MaxSpeed_RH - 50;
      }
      if (MAG_Now < Slow_Speed_MAG) {
        PWM_Left = PWM_Slow_Speed_LH;
        PWM_Right = PWM_Slow_Speed_RH + ((CPower / 2) * Compass_Error);  // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Right < 0) PWM_Right = PWM_Slow_Speed_RH - 20;
      }
    }
    Compass_Steering_Status = 2;
  }





  if (Compass_Error >= 0) {
    Serial.print(F("SL|"));

    // With no adjustment of wheel speed according to MAG Level
    if (MAG_Speed_Adjustment == 0) {
      PWM_Right = PWM_MaxSpeed_RH;
      PWM_Left = PWM_MaxSpeed_LH - (CPower * Compass_Error);  // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
      if (PWM_Left < 0) PWM_Left = PWM_MaxSpeed_LH - 50;
    }

    if (MAG_Speed_Adjustment == 1) {
      // MAX MAG Speed Right Hand Wheel
      if (MAG_Now >= Slow_Speed_MAG) {
        PWM_Right = PWM_MaxSpeed_RH;
        PWM_Left = PWM_MaxSpeed_LH - (CPower * Compass_Error);  // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Left < 0) PWM_Left = PWM_MaxSpeed_LH - 50;
      }
      if (MAG_Now < Slow_Speed_MAG) {
        PWM_Right = PWM_Slow_Speed_RH;
        PWM_Left = PWM_Slow_Speed_LH - ((CPower / 2) * Compass_Error);  // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Left < 0) PWM_Left = PWM_Slow_Speed_LH - 20;
      }
    }
    Compass_Steering_Status = 3;
  }
}
