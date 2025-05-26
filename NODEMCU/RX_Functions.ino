// Tutte le funzioni delle informazioni ricevute
void Receive_All_From_MEGA () {

  String NodeMCU_RX_Value  = "";                                                // cambiato in stringa

  while (NodeMCU.available() > 0) {
    
    char recieved = NodeMCU.read();
    if ( recieved != '\g'  && recieved != '\c' && recieved != '\d' && recieved != '\z' && recieved != '\y' && recieved != '\o' && recieved != '\m') {   
      NodeMCU_RX_Value = NodeMCU_RX_Value +  (char)recieved;                    // Torniamo a unire i caratteri in modo corretto a Serial1_Rx_Value
      } 

      else if (recieved == '\g') {
      val_VoltNow = NodeMCU_RX_Value.toFloat();                                 // se viene trovata la fine del valore, imposta AmpsTX e cancella Serial1_Rx_Value temp var Serial1_Rx_Value utilizzata per mantenere il valore fino a \q o \j
      NodeMCU_RX_Value = ""; // changed to string
      } 
      else if (recieved == '\c') {
      Loop_Cycle_Mowing = NodeMCU_RX_Value.toInt();                             // come superiore ma per VoltsTX,
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\d') {
      Mower_Docked = NodeMCU_RX_Value.toInt();                                  // come superiore ma per VoltsTX,
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\z') {
      Mower_Running = NodeMCU_RX_Value.toInt();                                 // come superiore ma per VoltsTX,
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\y') {
      Mower_Parked = NodeMCU_RX_Value.toInt();                                  // come superiore ma per VoltsTX,
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\o') {
      Charge_Detected = NodeMCU_RX_Value.toInt();                               // come superiore ma per VoltsTX,
      NodeMCU_RX_Value = "";
      } 
      else if (recieved == '\m') {
      Tracking_Wire = NodeMCU_RX_Value.toInt();                                 // come superiore ma per VoltsTX,
      NodeMCU_RX_Value = "";
      } 
    else Serial.print("No Data Received|");
  }
  Print_RX_Values();
  Calculate_Filtered_Mower_Status();
}


void Print_RX_Values() {
        BatteryVoltage = val_VoltNow;
        Serial.print("Volt:");
        Serial.print(BatteryVoltage);
        Serial.print("|");

        Serial.print("Loop:");
        Serial.print(Loop_Cycle_Mowing);  
        Serial.print("|");  

        Serial.print("Docked:");
        Serial.print(Mower_Docked);
        Serial.print("|");  
        
        Serial.print("Running:");
        Serial.print(Mower_Running);
        Serial.print("|");  

        Serial.print("Parked:");
        Serial.print(Mower_Parked);  
        Serial.print("|");

        Serial.print("Charge:");
        Serial.print(Charge_Detected);  
        Serial.print("|");

        Serial.print("Tracking:");
        Serial.print(Tracking_Wire);  
        Serial.print("|");
}


void Calculate_Filtered_Mower_Status() {

    if (Mower_Docked == 1) {
      Mower_Docked_Filter = Mower_Docked_Filter + 1;
      if (Mower_Docked_Filter == 3) Mower_Docked_Filter = 2;
    }
    else Mower_Docked_Filter = 0;
    
    if (Mower_Running == 1) {
      Mower_Running_Filter = Mower_Running_Filter + 1;
      if (Mower_Running_Filter == 3) Mower_Running_Filter = 2;
    }
    else Mower_Running_Filter = 0;
    
}
