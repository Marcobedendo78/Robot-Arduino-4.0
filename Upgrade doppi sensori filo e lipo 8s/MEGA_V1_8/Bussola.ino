// FUNZIONI BUSSOLA
//*****************************************************************************

/* Calcola la direzione della bussola come direzione e gradi della bussola di bordo */
void Get_Compass_Reading() {
  Vector norm = compass.readNormalize();                               // Legge e normalizza i dati X, Y, Z dal magnetometro
  
  // Calcolo della direzione con atan2(Y, X)
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Corregge la declinazione magnetica (es. Fratta Polesine +4° 1')
  const float declinationAngle = (4.0 + (1.0 / 60.0)) * (PI / 180.0);  // Declinazione magnetica Fratta Polesine +4 1'
  heading += declinationAngle;

  // Normalizza heading nell'intervallo [0, 2PI]
  if (heading < 0) heading += 2 * PI;
  if (heading >= 2 * PI) heading -= 2 * PI;

  Compass_Heading_Degrees = heading * 180.0 / PI;                      // Converti in gradi

  // Output seriale per debug
  Serial.print(F("Comp°:"));
  Serial.print(Compass_Heading_Degrees);
  Serial.print(F("|"));

  // Indicazione visiva sul display
  lcd.setCursor(7, 1);
  lcd.print(" ");
}


// Ruota il tosaerba nell'orientamento corretto per il percorso ottimale del cavo perimetrale
// Evitare di seguire l'intero cavo per tornare alla docking station docking
void Compass_Turn_Mower_To_Home_Direction() {
    //Ferma i motori
    Motor_Action_Stop_Motors();
    if (Abort_Wire_Find || Mower_Parked == 1) return;
    delay(2000);
    Print_LCD_Compass_Home();
    delay(1000);
    Compass_Target = Home_Wire_Compass_Heading;
    lcd.clear();
    if (WIFI_Enabled == 1) Get_WIFI_Commands();
    
    // Invertire leggermente il tosaerba
    SetPins_ToGoBackwards();
    Motor_Action_Go_Accel(800);
    //Motor_Action_Go_Slow_Speed();
    //delay(300);                                                     //Prima era 800 21/06/2022
    Motor_Action_Stop_Motors();
    if (Abort_Wire_Find || Mower_Parked == 1) return;
    delay(1000); 
    Get_Compass_Reading();
    SetPins_ToTurnLeft(); 
    // Questo fa girare un po' il tosaerba per garantire che venga letta una lettura reale della bussola (calibrazione).
    SetPins_ToTurnLeft();                                            // Chiama la funzione motore girare a sinistra
    Motor_Action_Go_Accel(500);
    //Motor_Action_Go_Slow_Speed();                                    // Imposta la velocità del movimento di svolta
    //delay(500);    
    Motor_Action_Stop_Motors();
    Get_Compass_Reading();
    delay(500);
    SetPins_ToTurnLeft();                                            // Chiama la funzione motore girare a sinistra
    delay(100);                                                      // Prima era 100 27/09/2022
    Motor_Action_Go_Accel(100);
    //Motor_Action_Go_Slow_Speed();                                    // Imposta la velocità del movimento di svolta
    //delay(100);  
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
  Serial.println(F("\n-- PID Compass Turn Start --"));

  float lastError = 0;
  float integral = 0;
  int Bad_Reading = 0;
  float lastHeading = 0;
  float tolerance = 30.0; // gradi accettabili

  unsigned long startTime = millis();
  unsigned long maxDuration = 10000; // 10 secondi timeout
  unsigned long lastCompassRead = 0;

  // Inizializza rotazione
  Get_Compass_Reading();
  float error = Compass_Target - Compass_Heading_Degrees;
  if (error > 180) error -= 360;
  if (error < -180) error += 360;

  // Decidi direzione una volta sola
  if (error > 0) {
    SetPins_ToTurnRight();
  } else {
    SetPins_ToTurnLeft();
  }

  // Attiva i motori alla velocità fissa
  analogWrite(ENAPin, PWM_Compass_Speed_RH);
  analogWrite(ENBPin, PWM_Compass_Speed_LH);

  while (millis() - startTime < maxDuration) {
    if (Abort_Wire_Find || Mower_Parked == 1) {
      Serial.println(F("🛑 Interrotto da STOP"));
      Motor_Action_Stop_Motors();
      return;
    }

    if (millis() - lastCompassRead >= 300) {
      Get_Compass_Reading();
      lastCompassRead = millis();

      error = Compass_Target - Compass_Heading_Degrees;
      if (error > 180) error -= 360;
      if (error < -180) error += 360;

      Serial.print("Compass: ");
      Serial.print(Compass_Heading_Degrees);
      Serial.print(" | Target: ");
      Serial.print(Compass_Target);
      Serial.print(" | Err: ");
      Serial.println(error);

      // Verifica lettura anomala
      if (abs(Compass_Heading_Degrees - lastHeading) > 50) {
        Bad_Reading++;
        lcd.setCursor(15, 1);
        lcd.print("x");
      } else {
        lcd.setCursor(15, 1);
        lcd.print("o");
      }
      lastHeading = Compass_Heading_Degrees;

      // Se l'errore è dentro la tolleranza → fermati
      if (abs(error) < tolerance) {
        Serial.println(F("✅ Orientamento corretto"));
        break;
      }

      lcd.setCursor(0, 1);
      lcd.print("Dir:");
      lcd.print((int)Compass_Heading_Degrees);
    }

    if (Bad_Reading > 5) {
      Serial.println("❌ Troppe letture errate");
      lcd.clear();
      lcd.print("Bad Compass Readings");
      lcd.setCursor(0, 1);
      lcd.print("Riparti");
      delay(1000);
      Motor_Action_Stop_Motors();
      Turn_To_Compass_Heading(); // riprova
      return;
    }
  }

  Motor_Action_Stop_Motors();
  Serial.println(F("-- PID Compass Turn Done --"));
}




void Display_Compass_Current_Heading_on_LCD() {
      lcd.setCursor(5, 1);
      lcd.print("    ");
      lcd.print(Compass_Heading_Degrees);
      delay(10);
}


void Calculate_Compass_Wheel_Compensation() {

  float Compass_Error = Compass_Heading_Degrees - Heading_Lock;          // Calcola l'errore nella direzione della bussola dalla direzione bloccata salvata
  
  if (Compass_Error > 180) Compass_Error = Compass_Error * - 1 ;
  if (Compass_Error < -180) Compass_Error = Compass_Error * - 1 ;
  Serial.print(F("C_Err:"));
  Serial.print(Compass_Error);
  Serial.print(F("|"));
 
  if (Compass_Error < 0) {                                               // Sterzare a sinistra
     Serial.print(F("SR|"));
    
    
    // Senza regolazione della velocità della ruota in base al livello MAG    
    if (MAG_Speed_Adjustment == 0) {
      PWM_Left = PWM_MaxSpeed_LH;         
      PWM_Right = PWM_MaxSpeed_RH + (CPower * Compass_Error);            // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
      if (PWM_Right < 0) PWM_Right = PWM_MaxSpeed_RH - 50;
      }
    
    if (MAG_Speed_Adjustment == 1) {
      // Ruota sinistra MAX MAG Speed
      if (MAG_Now >= Slow_Speed_MAG)  {
        PWM_Left = PWM_MaxSpeed_LH;
        PWM_Right = PWM_MaxSpeed_RH + (CPower * Compass_Error);          // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Right < 0) PWM_Right = PWM_MaxSpeed_RH - 50; 
      }
      if (MAG_Now < Slow_Speed_MAG)   {
        PWM_Left = PWM_Slow_Speed_LH;
        PWM_Right = PWM_Slow_Speed_RH + ((CPower/2) * Compass_Error);    // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Right < 0) PWM_Right = PWM_Slow_Speed_RH - 20; 
      }
    
      }
    Compass_Steering_Status = 2;
    } 




    
  if (Compass_Error >= 0) {  
    Serial.print(F("SL|"));

    // Senza regolazione della velocità della ruota in base al livello MAG
    if (MAG_Speed_Adjustment == 0) {
      PWM_Right = PWM_MaxSpeed_RH; 
      PWM_Left = PWM_MaxSpeed_LH -  (CPower * Compass_Error);             // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
      if (PWM_Left < 0) PWM_Left = PWM_MaxSpeed_LH - 50;
      }
    
    if (MAG_Speed_Adjustment == 1) {
      // Ruota destra MAX MAG Speed
      if (MAG_Now >= Slow_Speed_MAG)  {
        PWM_Right = PWM_MaxSpeed_RH; 
        PWM_Left = PWM_MaxSpeed_LH -  (CPower * Compass_Error);            // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Left < 0) PWM_Left = PWM_MaxSpeed_LH - 50;
        }
      if (MAG_Now < Slow_Speed_MAG)   {
        PWM_Right = PWM_Slow_Speed_RH;
        PWM_Left = PWM_Slow_Speed_LH -  ((CPower/2) * Compass_Error);      // Moltiplicare la differenza per D per aumentare la potenza, quindi sottrarla dal PWM
        if (PWM_Left < 0) PWM_Left = PWM_Slow_Speed_LH - 20;
      }
    
      }
    Compass_Steering_Status = 3;
    }

}


// Funzione di comodo per ottenere l'heading in gradi
float getHeading() {
  return Compass_Heading_Degrees;
}
