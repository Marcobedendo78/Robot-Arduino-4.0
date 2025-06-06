void Calculate_Wheel_Amps() {
  // Calculate Amp Value from Wheel Amp sensor
 int mVperAmp = 185;                                         // 66 per sensore 30A - 100 per sensore 20A - 185 per sensore 5A
 int ACSoffset = 2500; 
 double VoltageAmp = 0;
 //double Amps = 0;
 VoltageAmp = (RawWheelAmp / 1024.0) * 5000; // Gets you mV
 WheelAmps =  ((VoltageAmp - ACSoffset) / mVperAmp);
 if (WheelAmps < 0) WheelAmps = 0;                           //WheelAmps * - 1;  Prova 24/04/2024
 if (WheelAmps > 10) WheelAmps = 0;
 Serial.print(("WA:"));
 Serial.print(WheelAmps);
 Serial.print(F("|"));
 
 }


void Check_Wheel_Amps() {
  Calculate_Wheel_Amps();
  if (Wheel_Amp_Sensor_ON == 1) {
    
        if (WheelAmps >= Max_Wheel_Amps) {
            Wheel_Blocked_Count = Wheel_Blocked_Count + 1;
            if (Wheel_Blocked_Count > Wheel_Blocked_Count_Max) {    
                Serial.println(F(""));
                Serial.print(F("   Wheel Amps: "));
                Serial.print(WheelAmps);
                Serial.print(F("   Wheel Blocked Count: "));
                Serial.print(Wheel_Blocked_Count);
                Serial.print(F("   Max Wheel Amps: "));
                Serial.println(Max_Wheel_Amps);                  
                Serial.println("");
                Serial.println("!! Wheel_Blocked !!");
                Serial.print(F("|")); 
                Wheel_Blocked = 4;
                Print_LCD_Ruota();
                Manouver_Turn_Around();
                }
        }
        else {
        Wheel_Blocked = 0;
        Wheel_Blocked_Count = 0;
        }
   
  }
}



void Test_Check_Wheel_Amps() {

        Serial.print(F("   Wheel Amps: "));
        Serial.print(WheelAmps);
        Serial.print(F("   Wheel Blocked Count: "));
        Serial.print(Wheel_Blocked_Count);
        Serial.print(F("   Max Wheel Amps: "));
        Serial.println(Max_Wheel_Amps);
        
        if (WheelAmps > Max_Wheel_Amps) {
            Wheel_Blocked_Count = Wheel_Blocked_Count + 1;
            if (Wheel_Blocked_Count > Wheel_Blocked_Count_Max) {
                Serial.println("");
                Serial.print("!! Wheel_Blocked !!");
                Serial.print(F("|")); 
                Wheel_Blocked = 4;
                }
        }
        else {
          Wheel_Blocked = 0;
          Wheel_Blocked_Count = 0;
          }
}
