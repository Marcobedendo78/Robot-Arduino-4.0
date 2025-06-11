
/*
Codice con selezione tra driver L298 o BTS7960.
La potenza per BTS7960 è sempre al massimo (255), mentre per L298 si mantiene la logica classica.
*/
#define USE_DRIVER_L298      0   // imposta a 1 per usare L298N
#define USE_DRIVER_BTS7960   1   // imposta a 1 per usare BTS7960 (IBT-2)

#if (USE_DRIVER_L298 == USE_DRIVER_BTS7960)
  #error "Devi abilitare solo uno tra USE_DRIVER_L298 e USE_DRIVER_BTS7960"
#endif

#include <EEPROM.h>
#include <RunningMedian.h>
#include <TimerOne.h>

#if USE_DRIVER_L298
  #define pinIN1       9    // IN2
  #define pinIN2       2    // IN1
  #define pinPWM       3    // Non collegare nulla, il driver va alla massima potenza
  #define pinEnable    5    // Non collegare nulla. Sul driver mettere ponticello su Enable per funzionare a massima potenza
#elif USE_DRIVER_BTS7960
  #define pinRPWM      9    // RPWM
  #define pinLPWM      2    // LPWM
  #define pinREN       7    // R_EN + L_EN (collegati insieme al pin 7)
#endif

#define VER "1.0"
#define pinLED 13

volatile int step = 0;
volatile boolean enableSender = true;

#ifdef USE_DEVELOPER_TEST
int8_t sigcode[] = {
  1,0,0,0,0,
  1,0,0,0,0,
  -1,0,0,0,0,
  1,0,0,0,0
};
#else
int8_t sigcode[] = {
  1,1,-1,-1,1,-1,1,-1,-1,1,-1,1,1,-1,-1,1,-1,-1,1,-1,-1,1,1,-1
};
#endif

void timerCallback(){
  if (enableSender){
  #if USE_DRIVER_L298
    if (sigcode[step] == 1) {
      digitalWrite(pinIN1, LOW);
      digitalWrite(pinIN2, HIGH);
      digitalWrite(pinEnable, HIGH);
    }
    else if (sigcode[step] == -1) {
      digitalWrite(pinIN1, HIGH);
      digitalWrite(pinIN2, LOW);
      digitalWrite(pinEnable, HIGH);
    }
    else {
      digitalWrite(pinEnable, LOW);
    }
  #elif USE_DRIVER_BTS7960
    if (sigcode[step] == 1) {
      analogWrite(pinRPWM, 255);
      analogWrite(pinLPWM, 0);
    }
    else if (sigcode[step] == -1) {
      analogWrite(pinRPWM, 0);
      analogWrite(pinLPWM, 255);
    }
    else {
      analogWrite(pinRPWM, 0);
      analogWrite(pinLPWM, 0);
    }
  #endif
    step++;
    if (step == sizeof(sigcode)) {
      step = 0;
    }
  }
  else {
  #if USE_DRIVER_L298
    digitalWrite(pinEnable, LOW);
  #elif USE_DRIVER_BTS7960
    analogWrite(pinRPWM, 0);
    analogWrite(pinLPWM, 0);
  #endif
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println("Avvio trasmettitore filo perimetrale");
  Serial.println("Versione: " VER);

#if USE_DRIVER_L298
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinPWM, OUTPUT);
  pinMode(pinEnable, OUTPUT);
  digitalWrite(pinEnable, HIGH);
#elif USE_DRIVER_BTS7960
  pinMode(pinRPWM, OUTPUT);
  pinMode(pinLPWM, OUTPUT);
  pinMode(pinREN, OUTPUT);
  digitalWrite(pinREN, HIGH);
#endif

  pinMode(pinLED, OUTPUT);

  int T = 1000000 / 9615;
  Timer1.initialize(T);
  Timer1.attachInterrupt(timerCallback);
}

void loop(){
  digitalWrite(pinLED, enableSender);
}
