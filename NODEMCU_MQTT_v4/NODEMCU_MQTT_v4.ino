#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>  // Libreria per MQTT

// Definisci i pin e i parametri
SoftwareSerial NodeMCU(D2, D3);  // RXD2 TXD3
#define LED D4

// Credenziali WiFi
const char* ssid = "FRITZ!Box 7590 WH";
const char* password = "98337882460903976014";

// Dati di connessione MQTT
const char* mqtt_server = "192.168.178.33";
const char* mqtt_user = "homeassistant";
const char* mqtt_password = "eed1aiD1De5aicie7aiyeothuuw1hoobechoow4zahd0dai7wevuo5uokookeiCh";
const char* mqtt_topic_in = "home/robot/mower/control";
const char* mqtt_topic_out = "home/robot/mower/status";
const char* mqtt_topic_discovery = "homeassistant";

WiFiClient espClient;
PubSubClient client(espClient);

// Variabili per il monitoraggio del robot
float val_VoltNow;
float BatteryVoltage;
int Loop_Cycle_Mowing;
int Mower_Parked;
int Mower_Docked;
int Mower_Running;
int Charge_Detected;
int Tracking_Wire;

int i;                              // Trasmetti codice vuoto
int transmit_MQTT_code = 0;

// Funzione per connettersi al Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Funzione per connettersi al broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Convertiamo il nome del client in un tipo const char* con WiFi.macAddress()
    String clientID = "NodeMCU_Client_" + String(WiFi.macAddress());
    if (client.connect(clientID.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_in);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  NodeMCU.begin(9600);
  Serial.println();
  Serial.println("Setting up NODEMCU........");
  Serial.println("");
  pinMode(D2, INPUT);  // RX per ricevere dal Mega
  pinMode(D3, OUTPUT); // TX per inviare al Mega
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);  // Imposta il callback per MQTT

  // Invia i messaggi di auto discovery
  sendMotionDiscovery();
  sendRobotCommandDiscovery();
  sendManualControlDiscovery();

  // Inizializza i valori
  val_VoltNow = 0.0;
  BatteryVoltage = 0.0;
  Loop_Cycle_Mowing = 0;
  Mower_Parked = 0;
  Mower_Docked = 0;
  Mower_Running = 0;
  Charge_Detected = 0;
  Tracking_Wire = 0;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Funzione per ricevere dati da MEGA
  Receive_All_From_MEGA();

  // Aggiorna Home Assistant tramite MQTT
  Update_Home_Assistant_With_Status();

  // Aggiungi eventuali altre logiche o funzioni
}
