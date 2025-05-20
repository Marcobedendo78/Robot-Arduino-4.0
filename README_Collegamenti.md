# 🔌 Collegamenti Elettrici – Robot Tagliaerba Arduino 4.0

Questo documento elenca in dettaglio i collegamenti elettrici tra i microcontrollori (Arduino Mega e Nano) e i vari moduli e sensori utilizzati nel progetto.

---

## 🧠 Arduino Mega – Collegamenti

### 🎛️ Pulsanti di comando
| Pin Arduino Mega | Funzione        |
|------------------|-----------------|
| 50               | Pulsante Start  |
| 51               | Pulsante +      |
| 52               | Pulsante -      |
| 53               | Pulsante Stop   |
| GND              | Comune pulsanti |

---

### 🧲 Reed paraurti
| Pin Arduino Mega | Funzione  |
|------------------|-----------|
| 46               | Reed DX   |
| 47               | Reed SX   |
| GND              | Comune    |

---

### ⏱️ Orologio DS1302 RTC
| Pin Arduino Mega | Pin Modulo RTC |
|------------------|----------------|
| 29               | RST            |
| 30               | DAT            |
| 31               | CLK            |
| GND              | GND            |
| 5V               | Vcc            |

---

### 📟 Display I2C (HD44780 16x2)
| Pin Arduino Mega | Pin Display |
|------------------|-------------|
| SDA              | SDA         |
| SCL              | SCL         |
| GND              | GND         |
| 5V               | Vcc         |

---

### 🧭 Bussola digitale GY-271 (HMC5883L)
| Pin Arduino Mega | Pin Modulo |
|------------------|------------|
| SDA              | SDA        |
| SCL              | SCL        |
| GND              | GND        |
| 3.3V             | Vcc        |

---

### ⚙️ Driver BTS7960 – Motore Lama
| Pin Arduino Mega | Funzione |
|------------------|----------|
| 8                | LPWM     |
| 9                | R_EN     |
| 10               | L_EN     |
| GND              | GND      |
| 5V               | Vcc      |

---

### ⚙️ Driver BTS7960 – Motore Ruota DX
| Pin Arduino Mega | Funzione |
|------------------|----------|
| 5                | LPWM     |
| 6                | RPWM     |
| 7                | L_EN+R_EN|
| GND              | GND      |
| 5V               | Vcc      |

---

### ⚙️ Driver BTS7960 – Motore Ruota SX
| Pin Arduino Mega | Funzione |
|------------------|----------|
| 2                | L_EN+R_EN|
| 3                | RPWM     |
| 4                | LPWM     |
| GND              | GND      |
| 5V               | Vcc      |

---

### 🔁 Modulo relè 5V
| Pin Arduino Mega | Funzione |
|------------------|----------|
| A24              | IN       |
| GND              | GND      |
| 5V               | Vcc      |

---

### 🔊 Amplificatore LM386
| Pin Arduino Mega | Funzione |
|------------------|----------|
| A5               | OUT      |
| GND              | GND      |
| 5V               | Vcc      |
| GND + IN         | Ferrite 150mH |

---

### 🌐 NodeMCU ESP8266 (comunicazione Wi-Fi)
| Pin Arduino Mega | ESP8266 |
|------------------|---------|
| TX2              | D2      |
| RX2              | D3      |
| GND              | GND     |
| 5V               | VIN     |

---

### 📡 Sensori ultrasuoni
| Sonar SX (SR04) | Pin Mega |
|------------------|----------|
| ECHO             | 36       |
| TRIG             | 37       |
| GND              | GND      |
| Vcc              | 5V       |

| Sonar DX (SR04) | Pin Mega |
|------------------|----------|
| ECHO             | 38       |
| TRIG             | 39       |
| GND              | GND      |
| Vcc              | 5V       |

---

### 🔄 Collegamento con Arduino Nano
| Mega            | Nano     |
|------------------|----------|
| TX1              | D10      |
| RX1              | D11      |
| GND              | GND      |
| 5V               | 5V       |

---

## 🔧 Arduino Nano – Collegamenti

### ⚡ Sensori di corrente (ACS712)
| Pin Nano | Funzione       |
|----------|----------------|
| A1       | OUT Motore 1   |
| A6       | OUT Motore 2   |
| GND      | GND            |
| 5V       | Vcc            |

---

### 🔋 Sensore tensione (0–25 V)
| Pin Nano | Funzione |
|----------|----------|
| A2       | OUT      |
| GND      | GND      |
| 5V       | Vcc      |

---

### 🌧️ Sensore pioggia / livello acqua
| Pin Nano | Funzione |
|----------|----------|
| A3       | Segnale  |
| GND      | GND      |
| 5V       | Vcc      |

---

## 📌 Note

- Tutti i GND sono da considerare **comuni** tra moduli e microcontrollori.
- I segnali digitali devono rispettare i livelli logici dei dispositivi (5V o 3.3V).

---
