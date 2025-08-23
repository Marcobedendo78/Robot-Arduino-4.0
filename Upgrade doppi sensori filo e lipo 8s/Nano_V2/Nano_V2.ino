#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11);  // RX, TX

//Amp Sensor Variables.
int RawValueAmp= 0;
int RawValueVolt = 0;
int RawWheelAmp = 0 ;
int Raining = 5;
int Charging = 0;


// Trasmissione dei valori grezzi del sensore ad Arduino MEGA
float  AmpsTX;
float  VoltsTX;
float  WheelAmpsTX;
bool   Test;
int    Wheel_Blocked;
float  AmpLimit = 0.5 ;


void setup(){ 
 Serial.begin(9600);
 mySerial.begin(1200);
 Test = 0;

 pinMode (A1, INPUT);
 pinMode (A2, INPUT);
 pinMode (A3, INPUT);
 pinMode (A6, INPUT);



 if (Test == 1) {
  RawValueAmp = 510;
  RawValueVolt = 500;
 }
}

void Calculate_Volt_Amp() {


// Calcola il valore dell'amplificatore dai perni di ricarica
 int mVperAmp = 185;                          // 66 per sensore 30A - 100 per sensore 20A - 185 per sensore 5A
 int ACSoffset = 2500; 
 double VoltageAmp = 0;
 double Amps = 0;
 VoltageAmp = (RawValueAmp / 1024.0) * 5000; // Gets you mV
 AmpsTX =  ((VoltageAmp - ACSoffset) / mVperAmp);

 VoltageAmp = (RawWheelAmp / 1024.0) * 5000; // Gets you mV
 WheelAmpsTX =  ((VoltageAmp - ACSoffset) / mVperAmp);

// Calcola il valore del sensore di tensione dalla batteria

 float vout = 0.0;
 float R1 = 75000;      // Resistore da 75kΩ per lipo 8s in origine era 30kΩ (originale del sensore)
 float R2 = 7500;       // Resistore originale da 7.5kΩ 
 vout = (RawValueVolt * 5.0) / 1024.0; // see text
 VoltsTX = vout / (R2/(R1+R2));

}

 
void TX_Volts()  {  
  mySerial.print(VoltsTX);
  mySerial.println("\j");
  Serial.print("VoltsTX = ");
  Serial.print(VoltsTX);
  Serial.print("|");
}

void TX_Raw_Value_Volts()  {  
  mySerial.print(RawValueVolt);
  mySerial.println("\j");
}

void TX_Charge()  {
  mySerial.print(Charging);
  mySerial.println("\q");
}

void TX_Raw_Value_Charge()  {
  mySerial.print(RawValueAmp);
  mySerial.println("\q");
}

void TX_Raining()  {
  mySerial.print(Raining);
  mySerial.println("\w");
}

void  TX_Wheel_Blocked () {
  mySerial. print (RawWheelAmp);
  // mySerial.print (Wheel_Blocked);
  mySerial. println ( " \ s " );
}

void loop(){

// Legge i valori del sensore del numero grezzo di amp e volt 
 if (Test == 0) { 
 RawValueAmp = analogRead(A1);
 RawValueVolt = analogRead(A2);
 Raining = analogRead(A3);
 RawWheelAmp = analogRead(A6);
 }

  if (Test == 1 )   {
 RawValueAmp = RawValueAmp + 1;
 if (RawValueAmp > 550) RawValueAmp = 500;
 RawValueVolt = RawValueVolt + 1;
 if (RawValueVolt > 550) RawValueVolt = 500;
  }


 Serial.print("VoltsTX Raw = ");
 Serial.print(RawValueVolt);
 Serial.print("|");
 Serial.print("AmpsTX Raw = ");
 Serial.print (RawValueAmp);
 Serial.print("|");
 Serial.print("Wheel Raw = ");
 Serial.print (RawWheelAmp);
 Serial.print("|");


 Calculate_Volt_Amp();

 Serial.print("VoltsTX = ");
 Serial.print(VoltsTX);
 Serial.print("|");

 if (AmpsTX < 0.4) Charging = 0;
 if (AmpsTX > 0.4) Charging = 4;
 Serial.print("Charging = ");  
 Serial.print(Charging);
 Serial.print("|");

 Serial.print("AmpsTX = ");
 Serial.print (AmpsTX);
 Serial.print("|");

 Serial.print("Rain Sensor Raw = ");
 Serial.print (Raining);
 Serial.print("|");

if (Raining < 100) Raining = 0;
if (Raining >= 100) Raining = 1;

Serial.print( "Raining = ");
Serial.print(Raining);
Serial.print("|");

Serial.print( "Wheel Amps = ");
Serial.print(WheelAmpsTX);
Serial.print("|");

if (WheelAmpsTX > AmpLimit) {
  Serial.println("!! Wheel Blocked !!");
  Wheel_Blocked = 4;
  }
if (WheelAmpsTX <= AmpLimit) {
  Wheel_Blocked = 0;
  }



 Serial.println("");
 
 //TX_Charge();
 TX_Raw_Value_Charge(); 
 delay(5); 
 //TX_Volts();
 TX_Raw_Value_Volts(); 
 delay(5);

 TX_Raining();
 delay(5);

 TX_Wheel_Blocked();

     
} 
