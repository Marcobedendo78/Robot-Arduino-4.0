

/* Commentalo per disabilitare le stampe e risparmiare spazio */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeMCU(D2,D3);       //RXD2 TXD3

//#define LED LED_BUILTIN

#define LED D4                       // su varie schede NODEMCU il Pin LED è diverso.....

// Dovresti ottenere il token di autenticazione nell'app Blynk.
// Vai alle Impostazioni progetto (icona a forma di dado).
char auth[] = "Blynk Token Number";

// Le tue credenziali WiFi.
// Imposta la password.
char ssid[] = "WIFI NAME";
char pass[] = "WIFI Password";

int buttonState = 0;
char  Version[16];

// RX Variables
float val_VoltNow;                  // Voltaggio della batteria da MEGA
float BatteryVoltage;               // Tensione letta dall'ultimo RX
int Loop_Cycle_Mowing;              // Ciclo del circuito di corrente da MEGA
int Mower_Parked;
int Mower_Parked_Low_Batt;
int Mower_Docked;
int Mower_Docked_Filter;
int Mower_Running;
int Mower_Running_Filter;
int Mower_Lost;
int Charge_Detected;
int Tracking_Wire;
int Compass_Heading_Lock;

int i;                              // Trasmetti codice vuoto

bool Mower_Already_Started = 0;
bool Going_Home_Already = 0;

int j;
int k;
int transmit_blynk_code = 0;

bool Manuel_Mode;
int Automatic_Mode;


int loopstatus = 0;
int All;
int RX_Data_Recieved;
int lastConnectionAttempt = millis();
int connectionDelay = 5000; // provare a riconnettersi ogni 5 secondi
BlynkTimer timer;
WidgetLCD lcd(V6);


// Questa funzione invia il tempo di attività di Arduino ogni secondo al Pin virtuale (5).
// Nell'app, la frequenza di lettura del widget deve essere impostata su PUSH. Questo significa
// che definisci la frequenza con cui inviare i dati all'app Blynk.

void myTimerEvent()  {
  if (  (BatteryVoltage > 10) && (BatteryVoltage < 15) ){
      Blynk.virtualWrite(V3, BatteryVoltage);
  }

  Blynk.virtualWrite(V5, Loop_Cycle_Mowing);


}

void setup()
{
  Serial.begin(9600);
  NodeMCU.begin(9600);
  Serial.println();
  Serial.println("Setting up NODEMCU........");
  Serial.println("");
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);
  digitalWrite(LED, HIGH);                          // Spegni la luce LED
  WIFI_Connect();                                   // Connettiti al Wi-Fi
  Clear_APP();
  lcd.clear();
  pinMode(LED, OUTPUT);


}



//Funzioni
//***********************************


void WIFI_Connect() {
  
  int mytimeout = millis() / 1000;
  Serial.println();
  Serial.println("*********************************************");
  Blynk.begin(auth, ssid, pass);
  
  
  while (Blynk.connected() != WL_CONNECTED) {
    delay(500);
      if((millis() / 1000) > mytimeout + 3){                      // provare per meno di 4 secondi a connettersi al router WiFi
      break;
    }
  }
  
  if(!Blynk.connected()) {
    digitalWrite(LED, HIGH);
    Serial.println("NODEMCU Disconnected");
    Serial.println("Reconnecting . . . . . . ");
    WIFI_Connect();

    }
  else {
    digitalWrite(LED, LOW);
    Serial.println("Connected. . . . . .");
    // Imposta una funzione da chiamare ogni secondo
    timer.setInterval(1000L, myTimerEvent);
    }
  }







void loop()  {

  loopstatus = loopstatus + 1 ;
  RX_Data_Recieved = 0;                           // reimposta i dati ricevuti bool
  
  if(!Blynk.connected()) {
    Blynk.run();
    timer.run();  
    digitalWrite(LED, HIGH);
    Serial.println("------------------------");
    Serial.println("NODEMCU Disconnected");
    Serial.println("Reconnecting ... ");
    WIFI_Connect() ;
    }
  else {
    Blynk.run();
    timer.run();  
    digitalWrite(LED, LOW);                       //Il LED è invertito su un NODEMCU...
    Receive_All_From_MEGA();
    Update_Blynk_App_With_Status();
    Serial.println("");                           // monitor seriale di nuova linea
  }


}
