void Check_Sonar_Sensors() {
  
  // Ping Sonar sensors

  //Clears the Trig Pin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(5);

 // Pings each sonar at a 15ms interval

 if (Sonar_2_Activate == 1) distance2 = PingSonarX(trigPin2, echoPin2, 2, 2, 2, 0, 0);         //SONAR2
 if (Sonar_1_Activate == 1) distance1 = PingSonarX(trigPin1, echoPin1, 1, 1, 1, 1, 0);         //SONAR1
 if (Sonar_3_Activate == 1) distance3 = PingSonarX(trigPin3, echoPin3, 3, 3, 3, 2, 0);         //SONAR3

 }
  


/* SONAR Function
************************************************************************************/
// Funzione per inviare un segnale al sonar e calcolare la distanza tra l'oggetto e il sonar.
// La distanza calcolata viene stampata su una stampante seriale e visualizzata con X o _ sullo schermo LCD.
// La distanza calcolata viene quindi utilizzata per la logica di evitamento dell'oggetto.
// I sonar utilizzati possono essere attivati ​​nelle impostazioni.

int PingSonarX(int trigPinX, int echoPinX, int distanceX, long durationX, int sonarX, int LCDRow, int LCDColumn) {
  pinMode(trigPinX, OUTPUT);
  pinMode(echoPinX, INPUT);
  //Imposta il trigPin allo stato alto per 10 micro secondi inviando un'onda sonora
  digitalWrite(trigPinX, HIGH);
  digitalWrite(trigPinX, LOW);
  delayMicroseconds(10);

  /*Legge l'echoPin per l'onda rimbalzata e registra il tempo in microsecondi*/
  durationX = pulseIn(echoPinX, HIGH);

  /*Calcola la distanza in cm in base al tempo misurato*/
  distanceX = durationX * 0.034 / 2;
  delay(5);

  /* Se viene misurata una distanza pari a 0, il ping del sonar non è stato ricevuto.
     La distanza viene quindi impostata a 999 cm, in modo che il ping mancante non venga visualizzato come un oggetto rilevato.*/
  if (distanceX == 0) {
    distanceX = 999;
    Serial.print(F("S"));
    Serial.print(sonarX);
    Serial.print(F(":"));
    Serial.print(F("NP!"));
    Serial.print(F("|"));
  }

  /*Stampa la lettera del sonar e la distanza misurata sul monitor seriale*/
  Serial.print(F("S"));
  Serial.print(sonarX);
  Serial.print(F(":"));
  Serial.print(distanceX);
  Serial.print(F("cm"));
  Serial.print(F("/"));

  /*Se la distanza del sonar è inferiore alla distanza massima, viene registrato un oggetto da evitare*/
  if (distanceX <= maxdistancesonar ) {
    //Prints that Sonar X has detected an object to the Mower LCD.
    lcd.setCursor(LCDRow, LCDColumn);                //sets location for text to be written
    lcd.print("X");
    delay(10);
    if (sonarX == 1) {
        Sonar_Hit_1_Total = (Sonar_Hit_1_Total + 1);
        Serial.print(Sonar_Hit_1_Total);
        }
      if (sonarX == 2) {
        Sonar_Hit_2_Total = (Sonar_Hit_2_Total + 1);
        Serial.print(Sonar_Hit_2_Total);
        }
      if (sonarX == 3) {
        Sonar_Hit_3_Total = (Sonar_Hit_3_Total + 1);
        Serial.print(Sonar_Hit_3_Total);
        }      
    if ( (Sonar_Hit_1_Total == Max_Sonar_Hit) || (Sonar_Hit_2_Total == Max_Sonar_Hit) || (Sonar_Hit_3_Total == Max_Sonar_Hit) ) {
      Sonar_Hit = 1; 
      Print_Sonar_Hit();
      Serial.println("");
      Serial.println("Sonar Hit Detected");
      }
    
    }

  /*Se la distanza del sonar è maggiore della distanza massima, non viene registrato alcun oggetto da evitare*/
  if (distanceX > maxdistancesonar) {
    //Prints that the path of Sonar X is open.
    lcd.setCursor(LCDRow, LCDColumn);                 //sets location for text to be written
    lcd.print("_");
    delay(10);
    if (sonarX == 1) {
        Sonar_Hit_1_Total = 0;
        Serial.print(Sonar_Hit_1_Total);
        }
      if (sonarX == 2) {
        Sonar_Hit_2_Total = 0;
        Serial.print(Sonar_Hit_2_Total);
        }
      if (sonarX == 3) {
        Sonar_Hit_3_Total = 0;
        Serial.print(Sonar_Hit_3_Total);
        }   
    }
   

  return distanceX;
}
