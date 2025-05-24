//////////////////////////////////////////////////////////////////////////////////////////////////////
// Robot tagliaerba Arduino                                                                         //
// Versione modificata da Bedendo Marco                                                             //
// Versione con Driver motori BTS7960 e motori E192-2S.12.180                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////

//Libraries for Perimeter Wire Receiver
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "drivers.h"
#include "adcman.h"
#include "perimeter.h"

//Libraries for Real Time Clock
#include <stdio.h>
#include <DS1302.h>

//Libraries for ic2 Liquid Crystal
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);



//Librerie per la funzione Calendario di taglio
#include <TimeLib.h>
#include <TimeAlarms.h>
AlarmId id;

//Compass Setup
#include <DFRobot_QMC5883.h>
DFRobot_QMC5883 compass;


//Pin di setup per Arduino MEGA

//Perimeter Wire Pins
#define pinPerimeterLeft A5        // Pin rievamento filo perimetrale normalmente A5
#define pinPerimeterRight A4       // lascia aperto
#define pinLED LED_BUILTIN


//Pins Orologio in tempo reale
const int kCePin   = 29;  // RST
const int kIoPin   = 30;  // DAT
const int kSclkPin = 31;  // CLK

DS1302 rtc(kCePin, kIoPin, kSclkPin);

//Configurazione del sonar per i sonar anteriori 1-3
#define echoPin1 34   //S1 Sonar centrale               
#define trigPin1 35
#define echoPin2 36   //S2 Sonar SX
#define trigPin2 37
#define echoPin3 38   //S3 Sonar DX
#define trigPin3 39

//Reed paraurti
#define Bumper_Switch_Frnt_RH  46               // Definire il Pin 47 sul MEGA per rilevare il microinterruttore DX
#define Bumper_Switch_Frnt_LH  47               // Definire il Pin 47 sul MEGA per rilevare il microinterruttore SX


//Pulsanti di controllo
#define Start_Key 50 // Pulsante Start
#define Plus_Key 51  // Pulsante +
#define Minus_Key 52 // Pulsante -
#define Stop_Key 53  // Pulsante Stop


//Pin Setup for the wheel Motor Bridge Controller Configurazione dei pin per il controller dei driver motori
//Motore DX
#define ENAPin 7                // PIN L_EN + R_EN  
#define IN1Pin 6                // PIN RPWM
#define IN2Pin 5                // PIN LPWM
//Motore SX
#define ENBPin 2                // PIN L_EN + R_EN
#define IN3Pin 4                // PIN LPWM
#define IN4Pin 3                // PIN RPWM

//Motore Lama
#define RPWM 8
#define L_EN 9
#define R_EN 10

//Relay Switch
#define Relay_Motors 24         
                               


  //Variabili globali


  //Variabili rilevazione filo perimetrale
  Perimeter perimeter;
  unsigned long nextTime = 0;
  int counter = 0;
  boolean inside = true;
  int Wire_Detected;

  int Loop_Cycle_Mowing = 0;
              
  //Variabili dei sonar
  long duration1 = 0;                     // Tempo necessario affinché il ping del sonar venga ricevuto dal pin dell'eco.
  long duration2 = 0;                     // Tempo necessario affinché il ping del sonar venga ricevuto dal pin dell'eco.
  long duration3 = 0;                     // Tempo necessario affinché il ping del sonar venga ricevuto dal pin dell'eco.

  int distance1 = 999;                    // Distanza calcolata dal Sonar Centrale
  int distance2 = 999;                    // Distanza calcolata dal Sonar SX
  int distance3 = 999;                    // Distanza calcolata dal Sonar DX

  int distance_blockage;     

  int Sonar_Hit_1_Total;
  int Sonar_Hit_2_Total;
  int Sonar_Hit_3_Total;
  bool Sonar_Hit_Any_Total;
  bool Sonar_Hit_1 = 0;
  bool Sonar_Hit_2 = 0;
  bool Sonar_Hit_3 = 0;
  bool Sonar_Hit   = 0;

  // Variabili del paraurti
  bool Bump_Frnt_LH;
  bool Bump_Frnt_RH;
  bool Bumper;
  
  //Dati di funzionamento del tosaerba
  int Compass_Steering_Status;
  
  //Variabili di stato del tosaerba
  bool Mower_Docked;
  bool Mower_Parked;
  bool Mower_Running;
  bool Mower_Parked_Low_Batt;
  bool Mower_Error;
  bool Manuel_Mode;

  //Variabili dei pulsanti di controllo
  byte  Start_Key_X;
  byte  Plus_Key_X;
  byte  Minus_Key_X; 
  byte  Stop_Key_X;
  bool  Menu_Complete_Settings;
  bool  Menu_Complete_Alarms;
  bool  Menu_Complete_Sensors;
  bool  Menu_Complete_Motion;
  bool  Menu_Complete_NAVI;
  bool  Menu_Complete_Tracking;  
  bool  Menu_Complete;
  byte  Menu_Mode_Selection;
  int   Menu_View;
  int   Mow_Time_Set;
  int   Max_Options_Timing;
  int   Max_Options_Docked;
  int   Max_Options_Parked;
  int   Max_Options_Settings;
  int   Max_Options_Test;
  int   Max_Options_Alarms;
  int   Max_Options_Sensors;
  int   Max_Options_Motion;
  int   Max_Options_Tracking;
  int   Max_Options_NAVI;
  int   Max_Options_BETA;

  //Comunicazione seriale
  float Volts;
  int   VoltsTX;
  float Volts_Last;
  int   Zero_Volts;
  float Amps;
  float VoltageAmp;
  int   RawValueAmp;
  int   RawValueVolt;
  int   Rain_Detected;
  int   Rain_Hit_Detected = 0;
  int   Charging;
  //float Battery_Voltage_Last;
  float Amps_Last;
  int   Volts_Outside_Reading;
  byte  OK_Nano_Data_Volt_Received; 
  byte  OK_Nano_Data_Charge_Received;
  byte  Charge_Hits = 0;
  byte  Docked_Hits = 0;
  bool  Charge_Detected_MEGA = 0;
  int   VoltsTX_Last;
  
  //Variabili del calendario di taglio
  byte Alarm_Hour_Now;
  byte Time_Hour;
  byte Time_Minute;
  byte Time_Second;
  byte Time_Day;
  byte Time_Month;
  byte Time_Year;
  byte Time_Date;
  bool Alarm_Timed_Mow_ON = 0;
  byte Alarm_Timed_Mow_Hour;                         // Mowing Hour Number 3
  byte Alarm_Timed_Mow_Minute;                       // Alarm minute 3

  int Alarm_1_Saved_EEPROM;
  int Alarm_2_Saved_EEPROM;
  int Alarm_3_Saved_EEPROM;
  int Alarm_4_Saved_EEPROM;
  int Alarm_5_Saved_EEPROM;
  int Alarm_6_Saved_EEPROM;

  String dayAsString(const Time::Day day) {
    switch (day) {
      case Time::kSunday: return "Sunday";
      case Time::kMonday: return "Monday";
      case Time::kTuesday: return "Tuesday";
      case Time::kWednesday: return "Wednesday";
      case Time::kThursday: return "Thursday";
      case Time::kFriday: return "Friday";
      case Time::kSaturday: return "Saturday";
      }
  return "(unknown day)";
  }

  //Tracciamento del cavo perimetrale
  int I; 
  int Track_Wire_Itterations;
  bool Outside_Wire;
  byte Exit_Zone;
  int MAG_Now;
  int MAG_OUT_Stop;
  int MAG_IN_Stop;
  int MAG_TURN;
  int MAG_Average_Start;
  int MAG_Last;
  byte Outside_Wire_Count = 0;
  int Tracking_Wire = 0;
  bool Wire_ON_Printed;
  int Wire_Off;
  int Wire_Refind_Tries = 0;

  int Tracking_Turn_Left;
  int Tracking_Turn_Right;
  bool Mower_Track_To_Charge;
  bool Mower_Track_To_Exit;

  bool Abort_Wire_Find;
  bool No_Wire_Found_Fwd;
  bool No_Wire_Found_Bck;
  int  Wire_Find_Attempt = 0; 

  int  PWM_Right;
  int  PWM_Left;
  int  MAG_Goal;
  int  MAG_Error;
  int  MAG_Start;
  byte PWM_Blade_Speed_Min;
  byte PWM_Blade_Speed_Max;
  bool Blade_Override = 0;

  //Variabili della bussola
  float   Compass_Heading_Degrees;
  float   Heading;
  bool    Compass_Heading_Locked = 0;
  float   Heading_Lock;
  int     Heading_Upper_Limit_Compass;
  int     Heading_Lower_Limit_Compass;
  float   Compass_Target;
  int     Compass_Leg = 0;
  
  int     error = 0;
  float   Calb_XAxis;
  float   Calb_YAxis;
  float   Calb_ZAxis;
  int     Tilt_X;
  int     Tilt_Y;
  int     Tilt_Z;
  float   X_Tilt;
  float   Y_Tilt;
  float   Z_Tilt;

  // Modello di falciatura
  int Spiral_Mow = 1;
  int Linking_Section;
  int Leg = 1;
  float Compass_Last;  

  // Stampa valori della tracciatura filo perimetrale
  int PrintInMax;
  int PrintInMid;
  int PrintInMin;
  int PrintOutMin;
  int PrintOutMid;
  int PrintOutMax;
  int PrintMAG_Now;

  // Variabili del WIFI
  float val_WIFI;

  // Impostazioni salvate EEPROM
  int   PWM_LEFT_EEPROM;
  int   PWM_RIGHT_EEPROM;
  int   PWM_BLADE_EEPROM;
  int   COMPASS_EEPROM;
  float Tracking_PID_P_EEPROM;
  int   Pattern_Mow_EEPROM;
  float Minimum_Volt_EEPROM;
  int   Compass_Home_EEPROM;
  int   Tip_Safety_EEPROM;
  int   Mower_Turn_Delay_Min_EEPROM;
  int   Mower_Turn_Delay_Max_EEPROM;
  int   Mower_Reverse_Delay_EEPROM;
  bool  Sonar_1_Activate_EEPROM;
  bool  Sonar_2_Activate_EEPROM;
  bool  Sonar_3_Activate_EEPROM;  
  int   Track_Wire_Zone_1_Cycles_EEPROM;
  int   Track_Wire_Zone_2_Cycles_EEPROM;
  int   Track_Wire_Zone_3_Cycles_EEPROM;
  bool  Use_Charging_Station_EEPROM;
  bool  CW_Tracking_To_Charge_EEPROM;
  bool  CCW_Tracking_To_Charge_EEPROM;
  bool  CW_Tracking_To_Start_EEPROM;
  bool  CCW_Tracking_To_Start_EEPROM;
  int   Max_Cycles_Straight_EEPROM;
  bool  Compass_Heading_Hold_Enabled_EEPROM;
  float CPower_EEPROM;
  int   Max_Sonar_Hit_EEPROM;
  long  maxdistancesonar_EEPROM;
  bool  Perimeter_Wire_Enabled_EEPROM;
  int   Max_Cycle_Wire_Find_EEPROM;
  int   Max_Cycle_Wire_Find_Back_EEPROM;
  int   Max_Tracking_Turn_Right_EEPROM;
  int   Max_Tracking_Turn_Left_EEPROM;
  bool  Rain_Sensor_Installed_EEPROM;
  int   Rain_Total_Hits_Go_Home_EEPROM;
  bool  WIFI_Enabled_EEPROM;
  bool  Cutting_Blades_Activate_EEPROM;
  int   Low_Battery_Instances_Chg_EEPROM;
  int   Alarm_1_Action_EEPROM;
  bool  Bumper_Activate_Frnt_EEPROM;
  int   Wheel_Amp_Sensor_ON_EEPROM;
  int   Max_Wheel_Amps_EEPROM;
  bool  Attivazione_Encoder_Ruote_EEPROM;
  int   Cycles_Max_EEPROM;
  int   Kp_EEPROM;
  int   Ki_EEPROM;
  int   Kd_EEPROM;
  
  // Sensore del assorbimento motori ruote
  int    RawWheelAmp;
  int    Wheel_Blocked;
  float  WheelAmps;
  int    Wheel_Blocked_Count;

  
  int minVal=265;
  int maxVal=402;
  
/******************************************************************************************************************************************

                   SETUP DEL ROBOT TAGLIAERBA

   I seguenti parametri di configurazione configureranno il tosaerba per il tuo giardino
   Attiva o disattiva le impostazioni per definire il comportamento che preferisci del robot tagliaerba.
  
   Le impostazioni contrassegnate con EEPROM possono essere regolate utilizzando il menu LCD del tagliaerba. Una volta modificato e salvato
   le impostazioni EEPROM sovrascriveranno le impostazioni in questo menu.
  
   Per cancellare queste impostazioni è necessario cancellare la EEPROM
    
  1 = Acceso      0 = Spento       Valore = Valore impostato per la variabile.

*******************************************************************************************************************************************/

  char Version[16] = "V1.3";

  bool Cutting_Blades_Activate    = 1;     // EEPROM            // Attiva le lame e il disco di taglio nel codice
  bool WIFI_Enabled               = 1;     // EEPROM            // Attiva le funzioni Wi-Fi
  bool Perimeter_Wire_Enabled     = 1;     // EEPROM            // Attiva l'uso del cavo perimetrale perimetrale

  // Stazione Base
  bool Use_Charging_Station       = 1;      //EEPROM            // 1 se si utilizza la stazione di docking/ricarica 0 in caso contrario
  bool CW_Tracking_To_Charge      = 0;      //EEPROM            // Tracciamento in senso orario attorno al cavo perimetrale fino alla stazione di ricarica
  bool CCW_Tracking_To_Charge     = 1;      //EEPROM            // Tracciamento in senso antiorario attorno al cavo perimetrale fino alla stazione di ricarica
  bool CW_Tracking_To_Start       = 1;      //EEPROM            // Tracciamento in senso orario attorno al cavo perimetrale durante il tracciamento fino alla posizione iniziale
  bool CCW_Tracking_To_Start      = 0;      //EEPROM            // Tracciamento in senso antiorario attorno al cavo perimetrale fino alla stazione di ricarica
  byte Docked_Filter_Hits         = 1;                          // Numero di segnali di carica da rilevare prima che il rasaerba si spenga


  // Tracciatura Filo
  int Track_Wire_Zone_1_Cycles    = 700;    //EEPROM            // Zona 1 - Numero di iterazioni eseguite dalla funzione PID prima che il rasaerba esca dalla tracciatura del filo perimetrale
  int Track_Wire_Zone_2_Cycles    = 8500;   //EEPROM            // Zone 2 - Numero di iterazioni eseguite dalla funzione PID prima che il rasaerba esca dalla tracciatura del filo perimetrale
  int Track_Wire_Zone_3_Cycles    = 12500;  //EEPROM            // Falciatura lungo il filo perimetrale
  int  Max_Tracking_Turn_Right    = 200;    //EEPROM            // Il numero massimo di comandi di svolta a destra durante il tracciamento del cavo prima che venga richiamata una funzione di ricerca del cavo rinnovata (la ruota gira)
  int  Max_Tracking_Turn_Left     = 200;    //EEPROM            // Se viene raggiunto questo valore, viene richiamata la subroutine di ricerca del filo.
  int  Max_Cycle_Wire_Find        = 600;    //EEPROM            // Numero massimo di cicli di tracciamento in avanti nella ricerca del filo prima che il rasaerba riavvii una rotazione della bussola e la ricerca del filo.
  int  Max_Cycle_Wire_Find_Back   = 50;     //EEPROM            // Numero massimo di cicli di tracciamento all'indietro nella ricerca del filo prima che il rasaerba riavvii una rotazione della bussola e la ricerca del filo.  
  int Cycles_Max                  = 10000;  //EEPROM            // Valore massimo che il robot segue il filo

  // Settaggi Bussola
  bool Compass_Activate               = 1;       //EEPROM       // Accende la bussola (deve essere 1 per attivare ulteriori funzionalità della bussola)
  bool Compass_Heading_Hold_Enabled   = 1;       //EEPROM       // Attiva la funzione di mantenimento della direzione della bussola per mantenere il tosaerba dritto
  int  Home_Wire_Compass_Heading      = 100;     //EEPROM       // Direzione del tosaerba che cercherà per trovare il cavo una volta completata la falciatura.
  float CPower                        = 2;       //EEPROM       // Ingrandimento della direzione verso PWM: forza con cui il tosaerba si corregge nella falciatura con bussola

  // Tipo di Falciatura
  int  Pattern_Mow                    = 0;       //EEPROM       // 0 = OFF |  1 = Parallel (non lavora!!) | 3 = Spirale |
  int  Max_Cycles_Spirals             = 500;                    // Sostituisce Max_Cycles per il taglio in linea retta poiché le spirali richiedono più cicli per essere completate
  float  Compass_Mow_Direction        = 80;                     // Taglio Direzione della linea quando è attivato il taglio con schema

  // Funzione di inclinazione di sicurezza - Sperimentale!
  int  Tip_Safety                     = 0;       //EEPROM       //Sperimentale e non ancora funzionante!! - Arresta tutti i motori se il rasaerba si ribalta o viene sollevato.
  int  Max_Tilt                       = 1800;
  int  Min_Tilt                       = -1800;
  int  Max_Tilt_Hits                  = 5;

  //Impostazioni alternative della bussola se la bussola QMC si blocca
  int  Compass_QMC_Refresh_Setting    = 3;                      // Con bussola QMC5883 era a (1)   1 = 200HZ and 2 = 100Hz and 3 = 50HZ (Standard) 4 = 10Hz (Se la bussola si blocca, prova una frequenza di aggiornamento diversa) 
  int  Compass_QMC_Sample_Setting     = 3;                      // Con bussola QMC5883 era a (1)   Numero di campioni per chiamata alla Bussola | 1 = 2 | 2 = 4 | 3 = 8 (Standard) |.

  // Sensore Pioggia 
  bool Rain_Sensor_Installed          = 1;  //EEPROM            // 1 = Sensore pioggia installato 0 = Nessun sensore installato.
  int  Rain_Total_Hits_Go_Home        = 10; //EEPROM            // Questo sensore ha senso solo in combinazione con una docking station per tosaerba
                                                                // poiché il tosaerba viene inviato lì per ripararsi dalla pioggia.
  // Settaggi Batteria
  float Battery_Max               = 16.8;                       // Volt massimi della batteria in Volt. 4S = 16.8V
  float Battery_Min               = 15.1;   //EEPROM            // Limite inferiore di carica della batteria prima della ricarica richiesta.
  byte  Low_Battery_Detected      = 0;                          // Impostato sempre su 0
  byte  Low_Battery_Instances_Chg = 16;     //EEPROM            // Casi di batteria scarica rilevati prima che venga richiesta una ricarica.

  // Moduli Sonar
  bool Sonar_1_Activate           = 0;      //EEPROM            // Attiva (1) Disattiva (0) Sonar 1 Centrale (NON USARE)
  bool Sonar_2_Activate           = 1;      //EEPROM            // Attiva (1) Disattiva (0) Sonar 2 SX
  bool Sonar_3_Activate           = 1;      //EEPROM            // Attiva (1) Disattiva (0) Sonar 3 DX
  int  Max_Sonar_Hit              = 1;      //EEPROM            // Numero massimo di letture sonar prima che l'oggetto venga scoperto
  long maxdistancesonar           = 40;     //EEPROM            // Distanza in cm dal rasaerba alla quale si attiverà il sonar.

  // Paraurti
  bool Bumper_Activate_Frnt       = 1;      //EEPROM            // Attiva il paraurti anteriore


  // Settaggio Motori Ruote
  int Max_Cycles_Straight          = 200;     //EEPROM           // Numero di giri che lo Sketch eseguirà prima che il rasaerba si giri comunque. Regola in base alla lunghezza del tuo giardino
  int PWM_MaxSpeed_LH              = 230;     //EEPROM           // Velocità in linea retta Ruota sinistra (guardando dal retro del tosaerba) Verrà ignorata se salvata nella EEPROM
  int PWM_MaxSpeed_RH              = 230;     //EEPROM           // Velocità in rettilineo Ruota destra - regolare per mantenere rettilineo l'allineamento del tosaerba. Verrà ignorato se salvato in EEPROM
  int Turn_Adjust                  = 40;                         // prima era 0 19/04/2022
  int PWM_TrackSpeed_LH            = 150;     //EEPROM           // Velocità del tosaerba durante la navigazione sul filo perimetrale ruota SX
  int PWM_TrackSpeed_RH            = 150;     //EEPROM           // Velocità del tosaerba durante la navigazione sul filo perimetrale ruota DX
  int PWM_Max_Slow_Speed_LH        = 140;                        //
  int PWM_Max_Slow_Speed_RH        = 140;                        //

  bool Accel_Speed_Adjustment      = 1;                          //** Solo per le rotazioni
  
  bool MAG_Speed_Adjustment        = 0;                          //** Riduce la velocià prima di rilevare il filo perimetrale
  int Slow_Speed_MAG               = -300;                       // Valore MAG grazie alla bassa velocità
  int PWM_Slow_Speed_LH            = 190;                        // Velocità in linea retta quando il tosaerba è quasi sopra il cavo Ruota sinistra.
  int PWM_Slow_Speed_RH            = 190;                        // Velocità in linea retta quando il tosaerba è quasi sopra il cavo Ruota destra.

  int Max_Motor_PWM_LH             = 180;
  int Max_Motor_PWM_RH             = 180;
  int Mower_Turn_Delay_Min         = 400;    //EEPROM            // Min Max Tempo di rotazione del tosaerba dopo l'inversione del filo. 1000 = 1 secondo
  int Mower_Turn_Delay_Max         = 800;    //EEPROM            // Il software seleziona un tempo di svolta casuale tra questi due valori
  int Mower_Reverse_Delay          = 400;    //EEPROM            // Valore di retromarcia prima di effettuare una svolta.

  bool Wheel_Amp_Sensor_ON         = 1;                          // Attiva la funzione di misura degli ampere di assorbimento dei motori delle ruote per rilevare se le ruote sono bloccate.
  float Max_Wheel_Amps             = 2.15;                       // Amperaggio massimo consentito nelle ruote prima che venga chiamato un blocco.
  int  Wheel_Blocked_Count_Max     = 2;                          // Numero di volte in cui vengono rilevate le ruote bloccate prima che abbia luogo un'azione inversa.

  // Parametri PID (regolabili)
  float Kp                         = 0.2;    //EEPROM            // Kp troppo basso? il robot corregge lentamente - Kp troppo alto? il robot “balla” a zig-zag
  float Ki                         = 0.01;   //EEPROM            // Ki alto? rischi accumulo e reazioni lente
  float Kd                         = 0.15;   //EEPROM            // Kd troppo alto? il robot sovracompensa

  // Configurazione del motore della lama
  // Blade Speed può essere modificato nel menu delle impostazioni e verrà scritto in EEPROM
  // Il numero seguente verrà quindi sovrascritto
  byte PWM_Blade_Speed            = 255;     //EEPROM            // Il segnale PWM inviato al motore della lama (velocità della lama)

  // Alarm Setup
  bool Set_Time                   = 0;       //EEPROM            // Girare su 1 per impostare l'ora su RTC (Imposta ora nella scheda Ora Set_Time_On_RTC) Dopo aver impostato l'ora, girare su 0 e ricaricare lo schizzo.


  // Se l'allarme viene modificato nelle impostazioni, verrà scritto nella EEPROM e le impostazioni seguenti verranno sovrascritte.
  // L'azione per l'allarme 1 è impostata per uscire dal dock e falciare in questo momento.
  // Per modificare questa azione vai a "void Activate_Alarms ()" 
  bool Alarm_1_ON                 = 1;       //EEPROM            // Attiva allarme 1 (1 = ON 0 = OFF)
  int  Alarm_1_Hour               = 12;      //EEPROM            // Ora di taglio numero 1
  int  Alarm_1_Minute             = 00;      //EEPROM            // Alarm Minute 1
  bool Alarm_1_Repeat             = 1;                           // Ripetere l'allarme allo stesso tempo
  int  Alarm_1_Action             = 2;       //EEPROM            // Imposta le azioni da eseguire quando viene richiamato l'allarme (1= Uscita zona1 - 2= Uscita zona 2 - 3= Taglia sul filo - 4= Partenza rapida - 5= Libero
  
  // L'azione per l'allarme 2 può essere impostata in "void Activate_Alarms()" 
  bool Alarm_2_ON                 = 1;       //EEPROM            // Attiva allarme 2 (1 = ON 0 = OFF)
  int  Alarm_2_Hour               = 15;      //EEPROM            // Ora di taglio numero 2
  int  Alarm_2_Minute             = 00;      //EEPROM            // Alarm minute 2
  bool Alarm_2_Repeat             = 1;                           // Ripetere l'allarme allo stesso tempo
  int  Alarm_2_Action             = 1;       //EEPROM            // Imposta le azioni da eseguire quando viene richiamato l'allarme (1= Uscita zona1 - 2= Uscita zona 2 - 3= Taglia sul filo - 4= Partenza rapida - 5= Libero

  // L'azione per l'allarme 3 può essere impostata in "void Activate_Alarms()"
  bool Alarm_3_ON                 = 1;       //EEPROM            // Attiva allarme 3 (1 = ON 0 = OFF)
  int  Alarm_3_Hour               = 18;      //EEPROM            // Ora di taglio numero 3
  int  Alarm_3_Minute             = 00;      //EEPROM            // Alarm minute 3
  bool Alarm_3_Repeat             = 1;                           // Ripetere l'allarme allo stesso tempo
  int  Alarm_3_Action             = 2;       //EEPROM            // Imposta le azioni da eseguire quando viene richiamato l'allarme (1= Uscita zona1 - 2= Uscita zona 2 - 3= Taglia sul filo - 4= Partenza rapida - 5= Libero

  // L'azione per l'allarme 4 può essere impostata in "void Activate_Alarms()" 
  bool Alarm_4_ON                 = 1;       //EEPROM            // Attiva allarme 4 (1 = ON 0 = OFF)
  int  Alarm_4_Hour               = 21;      //EEPROM            // Ora di taglio numero 4
  int  Alarm_4_Minute             = 00;      //EEPROM            // Alarm minute 4
  bool Alarm_4_Repeat             = 1;                           // Ripetere l'allarme allo stesso tempo
  int  Alarm_4_Action             = 2;       //EEPROM            // Imposta le azioni da eseguire quando viene richiamato l'allarme (1= Uscita zona1 - 2= Uscita zona 2 - 3= Taglia sul filo - 4= Partenza rapida - 5= Libero

  // L'azione per l'allarme 5 può essere impostata in "void Activate_Alarms()" 
  bool Alarm_5_ON                 = 0;       //EEPROM            // Attiva allarme 5 (1 = ON 0 = OFF)
  int  Alarm_5_Hour               = 12;      //EEPROM            // Ora di taglio numero 5
  int  Alarm_5_Minute             = 00;      //EEPROM            // Alarm minute 5
  bool Alarm_5_Repeat             = 1;                           // Ripetere l'allarme allo stesso tempo
  int  Alarm_5_Action             = 1;       //EEPROM            // Imposta le azioni da eseguire quando viene richiamato l'allarme (1= Uscita zona1 - 2= Uscita zona 2 - 3= Taglia sul filo - 4= Partenza rapida - 5= Libero

  // L'azione per l'allarme 6 può essere impostata in "void Activate_Alarms()" 
  bool Alarm_6_ON                 = 0;       //EEPROM            // Attiva allarme 6 (1 = ON 0 = OFF)
  int  Alarm_6_Hour               = 12;      //EEPROM            // Ora di taglio numero 6
  int  Alarm_6_Minute             = 00;      //EEPROM            // Alarm minute 6
  bool Alarm_6_Repeat             = 1;                           // Ripetere l'allarme allo stesso tempo
  int  Alarm_6_Action             = 1;       //EEPROM            // Imposta le azioni da eseguire quando viene richiamato l'allarme (1= Uscita zona1 - 2= Uscita zona 2 - 3= Taglia sul filo - 4= Partenza rapida - 5= Libero

  byte Alarm_Second               = 5;                           // Secondi


  /* Descrizione di come vengono visualizzati i valori seguenti nella stampa del monitor seriale per il filo
      funzione
     (InMax)                   Wire = 0                 (OutMax)
         |      (InMid)           |           (OutMid)     |
         |--------|--------|------|------|--------|--------|
         |        |        |      |      |        |        |
                        (InMin)       (OutMin)
  */

  // Valori di rilevamento filo
    /*Negative Values for In*/                                     // Questi valori si basano sul segnale ricevuto dal sensore a filo per il mio loop perimetrale
    int InMin = -100;                                              // Prima era -100
    int InMid = -200;                                              // Prima era -400
    int InMax = -1500;                                             // Il valore massimo del segnale ricevuto dal filo
    /*General Setup PID numbers for wire tracking*/
    float P               = 1.26;     //EEPROM                     // Fattore di moltiplicazione per l'errore misurato al centro del filo. Se il movimento è a scatti durante il monitoraggio riduce il numero
    float D               = 20;                                    // Valore di smorzamento per evitare che il rasaerba serpeggi sul cavo. Prima era a (50) 30/03/2024 
    byte Scale            = 36;                                    // Scala di stampa con tracciamento della linea del monitor seriale
  
    // Questi valori impostano la scala per la stampa del filo nel monitor seriale una volta tracciato
    int OutMin = 100;                                              // Prima era  100
    int OutMid = 200;                                              // Prima era  400
    int OutMax = 1500;                                             // Prima era  1500

    int Outside_Wire_Count_Max          = 10;                      // Se il tagliaerba è fuori dal cavo molte volte, il tagliaerba si ferma
    int Action_On_Over_Wire_Count_Max   = 3;                       // Imposta 1 per ibernare il tosaerba (spegnimento e arresto) Imposta 2 per ritrovare il giardino utilizzando la funzione di rilevamento del cavo e del sonar
                                                                   // 3 per eseguire una funzione di ricerca del filo
    int Count_Wire_Off                  = 10;                      // Numero di rilevazioni del filo spento per prcheggiare il robot

    bool Show_TX_Data                   = 0;                       // Mostra i valori ricevuti dal Nano / NodeMCU nel monitor seriale


    //Funzione di rallentamento per curve strette
    bool Boost_Turn                   = 1;                         // Attiva la funzione di rallentamento curve strette
    int Min_Track_PWM                 = 110;                       // Prima era 180 - Quando il valore PWM è inferiore a questo il rasaerba rallenta per effettuare meglio la svolta
    int Hard_Track_Turn_Delay         = 2;                         // Prima era 10


    //Valori per rotazione uscita base e rientro
    int Max_Spin_Attempts             = 150;                       // Rotazione sul filo per ritorno alla base
    int Max_Spin_Attempts_Exit        = 12;                        // Rotazione sul filo per uscita dalla base


/************************************************************************************************************/    



void setup() {
  Serial.begin(115200);
  Serial1.begin(1200);									                  // Aprire la porta seriale 1 per la comunicazione nano
  if (WIFI_Enabled == true) Serial2.begin(9600);					// Se il Wi-Fi è attivo prepara la porta seriale 2 aperta per la comunicazione NodeMCU
  Wire.begin();                                           // avviare l'interfaccia I2C
  Serial.println(" ");
  Serial.println(" ");  
  Serial.print(F("Robot Tagliaerba Arduino :"));
  Serial.println(Version);  
  Serial.println(F("==================="));
  Serial.println("");
  Serial.println(F("Avvio della configurazione del tosaerba"));
  Serial.println(F("==================="));
  Load_EEPROM_Saved_Data();
  if (Set_Time == 1 ) {
    Serial.print(F("Setting Time"));
    Set_Time_On_RTC();
  }
  DisplayTime();
  Serial.println("");
  Prepare_Mower_from_Settings();
  Setup_Run_LCD_Intro ();
  Setup_Compass();
  delay(100);
  Setup_Relays();
  Setup_Tip_Safety();
  Setup_Membrane_Buttons();
  Setup_Motor_Pins();
  Setup_Motor_SX_Pins();                                  //Modifica per driver motori nuovi
  Setup_Motor_DX_Pins();                                  //Modifica per driver motori nuovi
  Setup_ADCMan();
  Setup_Check_Pattern_Mow();
  if (Bumper_Activate_Frnt == true) Setup_Bumper_Bar();
  }

void loop() {

// Leggi le porte seriali per i dati
Read_Serial1_Nano();									                                                  // Leggi i dati seriali dal nano
Print_Mower_Status();                                                                	  // Aggiorna il monitor seriale con lo stato attuale del rasaerba.

// La falciatrice è agganciata in attesa di un comando per partire e falciare.
if (Mower_Docked == 1)          Print_LCD_Volt_Info();                                  // Stampa la tensione sullo schermo LCD
if (Mower_Docked == 1)          Check_if_Charging();
if (Mower_Docked == 1)          Print_LCD_Info_Docked();                                // Stampa le informazioni sullo schermo LCD
if (Mower_Docked == 1)          Print_Time_On_LCD(); 
if (Mower_Docked == 1)          Check_Membrane_Switch_Input_Docked();                   // Controllare i pulsanti per eventuali input
if (Mower_Docked == 1)          TestforBoundaryWire();                                  // Il test è che il cavo perimetrale è sotto tensione                                
if (Mower_Docked == 1)          Manouver_Dock_The_Mower();
if (Mower_Docked == 1)          Print_Time_On_Serial();
if (Mower_Docked == 1)          Display_Next_Alarm();
if (Mower_Docked == 1)          Activate_Alarms();


// Il tosaerba è parcheggiato pronto per essere avviato/riavviato.
if (Mower_Parked == 1)          Print_LCD_Volt_Info();                                  // Stampa la tensione sullo schermo LCD
if (Mower_Parked == 1)          Check_if_Charging();
if (Mower_Parked == 1)          Check_if_Raining_From_Nano ();                          // Controlla se il sensore dell'acqua rileva pioggia
if (Mower_Parked == 1)          Print_LCD_Info_Parked();                                // Stampa le informazioni sullo schermo LCD
if (Mower_Parked == 1)          Check_Membrane_Switch_Input_Parked();                   // Controllare i pulsanti per eventuali input
if (Mower_Parked == 1)          TestforBoundaryWire();
if (Mower_Parked == 1)          Manouver_Park_The_Mower();


// Il tosaerba è parcheggiato con batteria scarica e necessita di ricarica manuale
if (Mower_Parked_Low_Batt == 1) Print_LCD_Volt_Info();                                  // Stampa la tensione sullo schermo LCD
if (Mower_Parked_Low_Batt == 1) Print_Recharge_LCD();                                   // Stampa Ricarica sullo schermo LCD
if (Mower_Parked_Low_Batt == 1) Check_Membrane_Switch_Input_Parked();

// Il tagliaerba smarrito viene messo in modalità standby
if (Mower_Error == 1) Print_Mower_Error();                                              // Modalità di sicurezza in caso di smarrimento del tosaerba o in uno stato di errore
if (Mower_Error == 1) Check_Membrane_Switch_Input_Parked();
if (Mower_Error == 1) Manouver_Park_The_Mower();

// Il tosaerba sta tagliando l'erba.
if (Mower_Running == 1)                                                                                                   Print_LCD_Volt_Info();              // Stampa la tensione sullo schermo LCD.
if (Mower_Running == 1)                                                                                                   Process_Volt_Information();         // Agire in base alle letture di tensione.
if (Mower_Running == 1)                                                                                                   Check_if_Raining_From_Nano();       // Testare il sensore pioggia per la pioggia. Se rilevato, invia il tagliaerba a casa.
if (Mower_Running == 1)                                                                                                   Check_Membrane_Keys_Running();      // Verificare se è necessario arrestare il rasaerba tramite tastiera.
if (Mower_Running == 1)                                                                                                   Check_Timed_Mow();                  // Controlla se è arrivato il momento di tornare a casa.
if (Mower_Running == 1)                                                                                                   TestforBoundaryWire();              // Il test è che il cavo perimetrale è sotto tensione.
if ((Mower_Running == 1) && (Wheel_Amp_Sensor_ON == 1) )                                                                  Check_Wheel_Amps();                 // Test per vedere se le ruote sono bloccate.
if ((Mower_Running == 1) && (Tip_Safety == 1))                                                                            Compass_Check_Tilt_Angle();         // Prove per vedere se il tosaerba è capovolto.
if ((Mower_Running == 1) && (Wire_Detected == 1))                                                                         Check_Wire_In_Out();                // Verificare se il tosaerba è dentro o fuori dal recinto di filo.
if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0))                                                  Check_Sonar_Sensors();              // Se il tosaerba è il cavo perimetrale, controlla se i sonar non presentano ostacoli e stampa sul display LCD.
if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 0))                              Manouver_Mow_The_Grass();           // Ingressi ai motori delle ruote/motori delle lame in base all'ambiente. 
if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 0))                              Check_Bumper();                     // Se il tagliaerba è il cavo perimetrale, controllare l'attivazione del paraurti.
if ((Mower_Running == 1) && (Wire_Detected == 1) && ((Outside_Wire == 1) || (Bumper == 1))  && (Loop_Cycle_Mowing > 0))   Manouver_Turn_Around();             // Se il paraurti è attivato o il tosaerba è fuori dal cavo perimetrale, girarsi.
if ((Mower_Running == 1) && (Wire_Detected == 1) && (Outside_Wire == 0) && (Sonar_Hit == 1))                              Manouver_Turn_Around_Sonar();       // Se viene rilevato un colpo del sonar e il tosaerba è il cavo, aggirare l'ostacolo. 

// WIFI Comandi da e verso APP
if (Manuel_Mode == 1) Receive_WIFI_Manuel_Commands();
if (Manuel_Mode == 1) Print_LCD_Info_Manuel();
if ((Manuel_Mode == 0) || (Manuel_Mode == 1))Get_WIFI_Commands();                      // Dati TX e RX da NodeMCU

Serial.println(); 
  
}  // Fine Loop

  
