// Routine per controllare l'interruttore del paraurti
// La variabile globale Urto viene quindi attivata in modo che il tagliaerba possa reagire all'attivazione del paraurti
void Check_Bumper() {

if (Bumper_Activate_Frnt == true) {

  if (digitalRead(Bumper_Switch_Frnt_LH))   Bump_Frnt_LH = false;    // L'interruttore non è attivato, la variabile Bump è falsa
  if (!digitalRead(Bumper_Switch_Frnt_LH))  Bump_Frnt_LH = true;     // L'interruttore è attivato e la variabile Bump è vera
  
  if (digitalRead(Bumper_Switch_Frnt_RH))   Bump_Frnt_RH = false;    // L'interruttore non è attivato, la variabile Bump è falsa
  if (!digitalRead(Bumper_Switch_Frnt_RH))  Bump_Frnt_RH = true;     // L'interruttore è attivato e la variabile Bump è vera
  
  if ((Bump_Frnt_LH == false) && (Bump_Frnt_RH == false))  Bumper = false;
  if ((Bump_Frnt_LH == true) || (Bump_Frnt_RH == true) )  {  
    Bumper = true;
    Motor_Action_Stop_Motors();
    lcd.clear();
    lcd.print("PARAURTI");
    lcd.setCursor(0,1);
    lcd.print("ATTIVATO");
    delay(600);
    lcd.clear();
    }
  }
}




void Start_Bumper_Bar_Test() {
        Setup_Bumper_Bar();
        Menu_Complete = false;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("SX Par: ");
          lcd.setCursor(0,1);
          lcd.print("DX Par: ");  

        while (Menu_Complete == false) {
          Read_Membrane_Keys();                                               // Check if a button has been pressed
          lcd.setCursor(8,0);
          if (digitalRead(Bumper_Switch_Frnt_LH))   lcd.print("        ");        // Check if the LH microswitch is not activated
          if (!digitalRead(Bumper_Switch_Frnt_LH))  lcd.print("URTO    ");        // Check if the LH microswitch is activated
        
          lcd.setCursor(8,1);
          if (digitalRead(Bumper_Switch_Frnt_RH))   lcd.print("        ");        // Check if the RH microswitch is not activated
          if (!digitalRead(Bumper_Switch_Frnt_RH))  lcd.print("URTO    ");        // Check if the RH microswitch is activated        

             // Cancel the test by pressing the Stop key
             if(!Stop_Key_X){
              Menu_Complete = true;
              lcd.clear();
              lcd.print("Stop Test");
              delay(2000);
              lcd.clear();
              }
           }
  
  
  



  
}
