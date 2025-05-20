<div align="center">
  <img height="150" src="https://github.com/user-attachments/assets/ed07248b-e39b-4969-bd0f-9576c6b1846f"  />
</div>

###

<div align="center">
  <a href="www.youtube.com/@Focus978" target="_blank">
    <img src="https://img.shields.io/static/v1?message=Youtube&logo=youtube&label=&color=FF0000&logoColor=white&labelColor=&style=for-the-badge" height="25" alt="youtube logo"  />
  </a>
  <a href="	marcobedendo78@gmail.com" target="_blank">
    <img src="https://img.shields.io/static/v1?message=Gmail&logo=gmail&label=&color=D14836&logoColor=white&labelColor=&style=for-the-badge" height="25" alt="gmail logo"  />
  </a>
</div>

###

<h1 align="center">hey there 👋</h1>

###

<h3 align="left">👩‍💻  About Me</h3>

###

<h1 align="left">Bedendo Marco</h1>

###

<p align="left">Vivo in italia<br><br>- 🔭 Lavoro come programmatore CAM<br>- ⚡ Nel tempo libero Progetto ogni cosa che mi può servire e la realizzo con la stampa 3d</p>

###

<p align="left">Progetto le mia parti tramite Fusion 360 e stampo i pezzi con la mia stampante 3d Bambulab X1C</p>

###

<h3 align="left">🛠 Tools</h3>

###

<div align="left">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/arduino/arduino-original.svg" height="40" alt="arduino logo"  />
  <img width="12" />
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/vscode/vscode-original.svg" height="40" alt="vscode logo"  />
</div>

###
# 🤖 Robot Tagliaerba Arduino 4.0

Questo è un progetto DIY di un robot tagliaerba autonomo, basato su **Arduino Mega** come unità centrale di controllo, con vari microcontrollori e moduli per la gestione della navigazione, sensori, alimentazione e connettività Wi-Fi tramite MQTT.

## 📦 Componenti Elettronici

| Componente | Link | Q.tà | Prezzo unitario | Totale | Note |
|-----------|------|------|------------------|--------|------|
| Arduino Mega 2560 R3 | [Amazon](https://www.amazon.it/dp/B071W7WP35) | 1 | €25.99 | €25.99 | Unità principale |
| Arduino Nano | [Amazon](https://www.amazon.it/dp/B01LWSJBTD) | 1 | €11.99 | €11.99 | Telemetria batteria e sensori |
| NodeMCU ESP8266 | [Amazon](https://www.amazon.it/dp/B06Y1ZPNMS) | 1 | €9.99 | €9.99 | Wi-Fi MQTT |
| Arduino UNO R3 | [Amazon](https://www.amazon.it/dp/B0C1V3QT9T) | 1 | €14.99 | €14.99 | Generatore filo perimetrale |
| Display 16x2 I2C | [Amazon](https://www.amazon.it/dp/B07CQG6CMT) | 1 | €7.64 | €7.64 | Visualizzazione info |
| Sensore ultrasuoni JSN-SR04T | [Amazon](https://www.amazon.it/dp/B0CLS8B5LH) | 2 | €9.99 | €19.98 | Ostacoli |
| Sensore magnetico Reed NO | [Mouser](https://www.mouser.it/ProductDetail/934-59165-1-U-00-A) | 2 | €1.42 | €2.84 | Limiti corsa |
| Driver BTS7960 43A | [Amazon](https://www.amazon.it/dp/B0CZDQRB5C) | 3 | €9.99 | €29.97 | Motori ruote e lama |
| Driver L298 | [Amazon](https://www.amazon.it/dp/B07DK6Q8F9) | 2 | €6.99 | €13.98 | Attuatori secondari |
| Sensori corrente ACS712 | [Amazon](https://www.amazon.it/dp/B07CMXFS13) | 2 | €10.99 | €21.98 | Monitoraggio motori |
| Sensore tensione DC 2-25V | [Amazon](https://www.amazon.it/dp/B07RFJYSM4) | 1 | €7.99 | €7.99 | Batteria |
| Sensore pioggia / acqua | [Amazon](https://www.amazon.it/dp/B06XQ496SW) | 1 | €7.99 | €7.99 | Sicurezza ambientale |
| Orologio DS1302 RTC | [Amazon](https://www.amazon.it/dp/B0D8Q8BMH6) | 1 | €6.01 | €6.01 | Scheduling |
| Gy-271 HMC5883L | [Amazon](https://www.amazon.it/dp/B0CR98JLQL) | 1 | €8.48 | €8.48 | Bussola digitale |
| Modulo relè 1 canale | [Amazon](https://www.amazon.it/dp/B07V1YQQGL) | 1 | €6.99 | €6.99 | Comandi remoti |
| Buck LM2596 | [Amazon](https://www.amazon.it/dp/B07MY2NTFV) | 2 | €9.59 | €19.18 | Regolazione tensione |
| Step down 300W | [Amazon](https://www.amazon.it/dp/B07VRD5GZG) | 1 | €8.99 | €8.99 | Motori lama |
| BMS 4S 30A | [Amazon](https://www.amazon.it/dp/B09DYD4GVN) | 1 | €8.99 | €8.99 | Gestione batteria |
| Bilanciatore attivo 4S 5A | [Amazon](https://www.amazon.it/dp/B0B7CW6BRC) | 1 | €17.18 | €17.18 | Bilanciamento celle |
| Batteria LiPo 4S 6200mAh 80C | [Amazon](https://www.amazon.it/dp/B0CB6HTW7S) | 1 | €46.99 | €46.99 | Alimentazione principale |
| Motore Micromotors 22/20 RPM | [CSElettronica](https://www.cselettronica.net/default.asp?cmd=getProd&cmdID=20156) | 2 | €36.70 | €73.40 | Motori trazione |
| Motore lama 12V 30W | [Amazon](https://www.amazon.it/dp/B076M53HC9) | 1 | €27.79 | €27.79 | Rotore taglio |
| Ventole 40x40x10 24V | [Amazon](https://www.amazon.it/dp/B07WJGHTCF) | 2 | €8.99 | €17.98 | Raffreddamento |
| Alimentatore Mean Well 150W 24V | [Leroy Merlin](https://www.leroymerlin.it/prodotti/alimentatore-trasformatore-mean-well-150w-24v-6-25a-xlg-150-24-a-ip67-impermeabile-89756671.html) | 1 | €39.50 | €39.50 | Base ricarica |
| Scheda madre robot personalizzata | [MakerWorld](https://makerworld.com/it/models/420276-arduino-robot-lawnmower-robot-tagliaerba-arduino#profileId-323001) | 1 | €25.00 | €25.00 | Prodotta da Marco |

**Totale stimato**: **€497,21** (solo elettronica, escluso meccanica e struttura)

## 🔧 Architettura generale

- 🧠 **Arduino Mega**: logica principale, gestione motori e ultrasuoni
- 📶 **ESP8266**: comunicazione MQTT per comandi e telemetria
- 🔋 **Arduino Nano**: monitoraggio batteria, pioggia, correnti
- 🔄 **Arduino Uno**: generazione segnale filo perimetrale
- 📟 Display I2C: messaggi e stato
- ⚡ Alimentazione a 24V con step-down e BMS per la batteria 4S

## 🛠️ Stato

Il robot è in fase avanzata di sviluppo. Il codice è disponibile nella directory principale del [repository](https://github.com/Marcobedendo78/Robot-Arduino-4.0), insieme allo schema dei moduli collegati.

---

## 📫 Contatti

Per domande o suggerimenti puoi aprire una issue o contattare direttamente [Marco](https://github.com/Marcobedendo78).

