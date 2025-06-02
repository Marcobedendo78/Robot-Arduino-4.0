# Robot Arduino 4.0 – PCB Personalizzato

Questo progetto include un PCB personalizzato progettato per rendere l’installazione dei moduli, sensori e connessioni più ordinata ed efficiente per un robot tagliaerba basato su Arduino Mega e NodeMCU8266.

---

## 📦 Componenti da assemblare

![Componenti](img/20250508_174329.jpg)

---

## 🧩 Dettaglio PCB principale

![PCB parziale](img/20250508_175606.jpg)
![Misurazione 5V](img/20250508_175658.jpg)

---

## 🔧 PCB Assemblato

![PCB completo](img/20250508_194420.jpg)
![Retro PCB](img/20250508_194433.jpg)

---

## 🧠 Dettaglio moduli installati

![Moduli montati](img/20250530_201129.jpg)
![Zoom bussola](img/20250531_141824.jpg)
![Montaggio inclinato](img/20250531_142255.jpg)

---

## 🔌 Montaggio nel robot

![PCB nel robot](img/20250531_152122.jpg)

---

## 📐 PCB aggiuntivi

![PCB supplementari](img/20250602_152717.jpg)

---

## ⚡ PCB distribuzione alimentazione 5V

Questo è il piccolo PCB da montare **nella scocca centrale**.  
Serve a **distribuire il 5V e GND** a tutti i sensori frontali, evitando di portare cavi separati da dietro:

![PCB 5V nella scocca](img/20250517_190916.jpg)

---

## 📃 Descrizione tecnica

Ho creato un PCB da montare sopra Arduino Mega dove installare tutti i connettori dei vari sensori, alimentazione e ricarica della batteria. In questo modo è molto più semplice fare i collegamenti di tutte le parti.

Ho anche creato un PCB da montare nella scocca centrale, che funge da distributore di alimentazione 5V. Questo è pensato per facilitare il cablaggio nel passaggio stretto del foro centrale: così non serve portare GND e 5V per ogni singolo sensore.

Inoltre, ho disegnato un piccolo PCB per adattare i moduli NodeMCU8266 di dimensione più piccola a quello previsto dallo zoccolo della scheda principale.

---

## 🔧 Istruzioni di assemblaggio

1. Dissaldare i pin header e morsetti **originali** dai moduli: sensore tensione, corrente e relay.
2. Saldare nuovi pin header sul lato opposto per montarli direttamente sul PCB.
3. Montare e saldare il modulo **LM2596 step-down** nella sua posizione.
4. Collegare la batteria e **regolare con multimetro** l’uscita a 5.00V precisi.
5. Solo dopo questo step, procedere con la saldatura di tutti gli altri componenti (dal più basso al più alto).
6. Sul lato opposto vanno saldati:
   - i pin header per la bussola **GY-271**
   - il modulo **GY-521** (gyro)

⚠️ **Attenzione**: se non regoli i 5V prima di completare la saldatura, rischi di danneggiare tutti i moduli installati!

---

## 📬 Contatti

Per info o per richiedere i PCB:
👉 Messenger: [Marco Bedendo](https://www.facebook.com/marco.bedendo.54)

