
/* Perimieter Wire Collision Motion
  ************************************************************************************/
  void Test_Mower_Check_Wire()  {
  
  ADCMan.run();
  // ADCMan.setCapture(pinPerimeterLeft, 1, 0);

  if (millis() >= nextTime)  {
    nextTime = millis() + 50;
    if (perimeter.isInside(0) != inside) {
      inside = perimeter.isInside(0);
      counter++;
    }
  }

  /* Prints Values to the Serial Monitor of mag, smag and signal quality.  */
  Serial.print(F("Inside (1) or Outside (0):  "));
  Serial.print((perimeter.isInside(0)));
  Serial.print(F("     MAG: "));
  Serial.print((int)perimeter.getMagnitude(0));
  Serial.print(F("    smag: "));
  Serial.print((int)perimeter.getSmoothMagnitude(0));
  Serial.print(F("     qaulity: "));
  Serial.println((perimeter.getFilterQuality(0)));


  
  lcd.setCursor(0,0);
  lcd.print("IN/Out:");
  lcd.setCursor(8,0);
  lcd.print(perimeter.isInside(0));
  lcd.setCursor(0,1);
  lcd.print("MAG:");
  lcd.setCursor(8,1);
  lcd.print(perimeter.getMagnitude(0)); 


}


void Test_Relay() {
  
  digitalWrite(Relay_Motors, HIGH);
  Serial.println("Relay OFF");
  lcd.print("Relay OFF");
  delay(1000);
  lcd.clear();
  digitalWrite(Relay_Motors, LOW);
  Serial.println("Relay ON");
  lcd.print("Relay ON");
  delay(1000);
  lcd.clear();
}


void Test_Wheel_Amps () {
    Serial.println("Test Wheel Amps");
    Turn_On_Relay();
    delay(300);
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
      for (int i = 0; i <= 100; i++) {
        Read_Serial1_Nano(); 
        Calculate_Wheel_Amps();
        Test_Check_Wheel_Amps();             
        Serial.println("");
        }
       
         Motor_Action_Stop_Motors();
         Turn_Off_Relay();
}


void Test_Wheel_Motors() {

  digitalWrite(Relay_Motors, LOW);
  delay(200);
  if (I == 1) {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ACCENSIONE RUOTE");
  lcd.setCursor(1,0);
  lcd.print("Test");
  delay(1000);
  lcd.clear();

  lcd.print("RIMUOVI TUTTE LE");
  lcd.setCursor(0,1);
  lcd.print("LAME!!!");
  delay(1000);
  lcd.clear();


  lcd.print("<-- GIRA A SX");
  delay(500);
  SetPins_ToTurnLeft();
  Motor_Action_Go_Full_Speed();
  delay(1000);
  Motor_Action_Stop_Motors();
  delay(1000);
  lcd.clear();

  lcd.print("GIRA A DX -->");
  delay(500);
  SetPins_ToTurnRight();
  Motor_Action_Go_Full_Speed();
  delay(2000);
  Motor_Action_Stop_Motors();
  delay(1000);
  lcd.clear();

  lcd.print("AVANTI");
  delay(500);
  SetPins_ToGoForwards();
  Motor_Action_Go_Full_Speed();
  delay(1000);
  Motor_Action_Stop_Motors(); 
  delay(1000);
  lcd.clear();

  lcd.print("INDIETRO");
  delay(500);
  SetPins_ToGoBackwards();   
  Motor_Action_Go_Full_Speed();
  delay(1000);
  Motor_Action_Stop_Motors();  
  delay(1000);
  lcd.clear();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DINAMICO");
  lcd.setCursor(0,1);
  PWM_Left = 120;
  PWM_Right = 120;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);

  
  lcd.clear();
  lcd.setCursor(0,1);  
  PWM_Left = 255;
  PWM_Right = 0;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();

  lcd.clear();
  lcd.setCursor(0,1);
  PWM_Left = 255;
  PWM_Right = 0;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);


  lcd.clear();
  lcd.setCursor(0,1);  
  PWM_Left = 150;
  PWM_Right = 0;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0,1);  
  PWM_Left = 255;
  PWM_Right = 0;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);

    lcd.clear();
  lcd.setCursor(0,1);  
  PWM_Left = 255;
  PWM_Right = 0;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();

  lcd.clear();
  lcd.setCursor(0,1);
  PWM_Left = 0;
  PWM_Right = 255;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);


  lcd.clear();
  lcd.setCursor(0,1);  
  PWM_Left = 0;
  PWM_Right = 150;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0,1);  
  PWM_Left = 0;
  PWM_Right = 255;
  lcd.print("SX:");
  lcd.print(PWM_Left);
  lcd.print("   DX:");
  lcd.print(PWM_Right);
  SetPins_ToGoForwards();
  Motor_Action_Dynamic_PWM_Steering();
  delay(2000);

  
  Motor_Action_Stop_Motors();  
  delay(1000);

  lcd.clear();
  lcd.print("TEST FINITO");
  delay(1000);
  lcd.clear();

  I = 2;
  }
  digitalWrite(Relay_Motors, HIGH);
  delay(200);
}     



void Test_Mower_Blade_Motor() {
  // Spin the blade motor for 10 seconds
  digitalWrite(Relay_Motors, LOW);
  delay(200);
  lcd.print("MOTORE LAME");
  lcd.setCursor(0,1);
  lcd.print("TEST..!!");
  delay(1000);
  lcd.clear();
  lcd.print("RIMUOVI TUTTE LE");
  lcd.setCursor(0,1);
  lcd.print("LAME!!!");
  delay(4000);
  lcd.clear();
  delay(2000);
  lcd.print("MOTORE LAME");
  delay(500);
  Serial.println("LAME ACCESE");
  lcd.setCursor(0,1);
  lcd.print("ON ");
  lcd.setCursor(6,1);
  lcd.print("PWM =");
  lcd.print(PWM_Blade_Speed);
  Motor_Action_Spin_Blades();
  delay(10000);

  // Stop the blade motor spinning for 5 seconds
  lcd.clear();
  Serial.println("Blades OFF");
  lcd.print("MOTORE LAME");
  lcd.setCursor(0,1);
  lcd.print("SPENTO..  ");
  Motor_Action_Stop_Spin_Blades();
  delay(2000);
  lcd.clear();
  delay(500);

  digitalWrite(Relay_Motors, HIGH);
  delay(200);

  }


void Test_Sonar_Array()   {

  //Clears the Trig Pin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin3, LOW);


  /*Attiva tutti i sonar per rilevare oggetti davanti a te...
* I sonar non vengono attivati ​​per evitare riflessi nel sensore successivo.
     distance# reurned (trigpin#, echopin#, distance#, duration#, Sonar#, LCDColumn#, LCD Row#)
   *********************************************************************************************/
    if (Sonar_1_Activate) distance1 = PingSonarY(trigPin1, echoPin1, 1, 1, 1, 5, 0);          //SONAR1
    delay(15);
    if (Sonar_2_Activate) distance2 = PingSonarY(trigPin2, echoPin2, 2, 2, 2, 0, 0);         //SONAR2
    delay(15);
    if (Sonar_3_Activate) distance3 = PingSonarY(trigPin3, echoPin3, 3, 3, 3, 10, 0);          //SONAR3
    delay(15);
  }





/* SONAR Function
************************************************************************************/
// Funzione per inviare un segnale al sonar e calcolare la distanza tra l'oggetto e il sonar.
// La distanza calcolata viene stampata su una stampante seriale e visualizzata con X o _ sullo schermo LCD.
// La distanza calcolata viene quindi utilizzata per la logica di evitamento dell'oggetto.
// I sonar utilizzati possono essere attivati ​​nelle impostazioni.

int PingSonarY(int trigPinY, int echoPinY, int distanceY, long durationY, int sonarY, int LCDRow, int LCDColumn) {
  pinMode(trigPinY, OUTPUT);
  pinMode(echoPinY, INPUT);
  //Imposta il trigPin allo stato alto per 10 micro secondi inviando un'onda sonora
  digitalWrite(trigPinY, HIGH);
  digitalWrite(trigPinY, LOW);
  delayMicroseconds(10);

  /*Legge l'echoPin per l'onda rimbalzata e registra il tempo in microsecondi*/
  durationY = pulseIn(echoPinY, HIGH);

  /*Calcola la distanza in cm in base al tempo misurato*/
  distanceY = durationY * 0.034 / 2;
  delay(5);

  /* Se viene misurata una distanza pari a 0, il ping del sonar non è stato ricevuto.
  La distanza viene quindi impostata a 999 cm in modo che il ping mancante non venga visualizzato come un oggetto rilevato..*/
  if (distanceY == 0) {
    distanceY = 999;
    Serial.print(F("SONAR "));
    Serial.print(sonarY);
    Serial.print(": ");
    Serial.println(F("NO PING ERROR REMOVED"));
  }

  /*Stampa la lettera del sonar e la distanza misurata sul monitor seriale*/
  Serial.print(F("SONAR "));
  Serial.print(sonarY);
  Serial.print(": ");
  Serial.print(distanceY);
  Serial.println(F(" cm"));
  //Serial.println(maxdistancesonar);

  /*Se la distanza del sonar è inferiore alla distanza massima, viene registrato un oggetto da evitare*/
  if (distanceY <= maxdistancesonar ) {
    //Prints that Sonar X has detected an object to the Mower LCD.
    lcd.setCursor(LCDRow, LCDColumn);                //sets location for text to be written
    lcd.print("X");
    LCDColumn = LCDColumn + 1;    
    lcd.setCursor(LCDRow, LCDColumn);                //sets location for text to be written
    lcd.print("   ");
    lcd.setCursor(LCDRow, LCDColumn);
    lcd.print(distanceY);
    delay(10);
  }

  /*Se la distanza del sonar è maggiore della distanza massima, non viene registrato alcun oggetto da evitare*/
  if (distanceY > 100) {
    //Indica che il percorso del Sonar X è aperto.
    LCDColumn = LCDColumn - 1;   
    lcd.setCursor(LCDRow, LCDColumn);                 //sets location for text to be written
    lcd.print("_");
    delay(10);
  }

  return distanceY;
  return sonarY;

}


void Test_Compass_Turn_Function() {
    digitalWrite(Relay_Motors, LOW);
    delay(200);
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    delay(2000);
    Manouver_Turn_Around();
    Turn_To_Compass_Heading();
    SetPins_ToGoForwards();
    Motor_Action_Go_Full_Speed();
    delay(2000); 
    digitalWrite(Relay_Motors, HIGH);
    }


void Test_LIDAR_LCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test LIDAR...");

  long distance = 0;
  int angle = 0;

  StartLidarMotor();
  delay(500);
  lidar.startScan();

  unsigned long start = millis();
  while (millis() - start < 5000) { // test per 5 secondi
    if (IS_OK(lidar.waitPoint())) {
      auto point = lidar.getCurrentPoint(); // compatibile con la tua libreria

      // Filtro: qualità > 0, distanza plausibile
      if (point.quality > 0 && point.distance > 100 && point.distance < 3000) {
        angle = ((int)point.angle) % 360;

        // Filtro angolare (frontale e laterale)
        if (isAngleInValidRange(angle)) {
          distance = point.distance;

          lcd.setCursor(0, 0);
          lcd.print("Dist: ");
          lcd.print(distance);
          lcd.print(" mm   ");  // spazi per pulizia

          lcd.setCursor(0, 1);
          lcd.print("Ang: ");
          lcd.print(angle);
          lcd.print((char)223); // simbolo °
          lcd.print("     ");

          Serial.print(F("LIDAR -> Dist: "));
          Serial.print(distance);
          Serial.print(F(" mm | Angle: "));
          Serial.println(angle);
        }
      }
    }
  }

  lidar.stop();
  StopLidarMotor();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Finito");
  delay(2000);
  lcd.clear();
}

bool isAngleInValidRange(int angle) {
  return (angle >= 330 || angle <= 30) || 
         (angle > 15 && angle <= 90) || 
         (angle >= 270 && angle < 345);
}


