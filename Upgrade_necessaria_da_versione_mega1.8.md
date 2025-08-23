# 🔧 Modifiche al progetto Robot Tagliaerba

In questa sezione descrivo alcune modifiche hardware effettuate sul robot.

---

## 📡 Modifica 1: Aggiornamento sensore filo

Per migliorare la rilevazione del filo perimetrale, è stato aggiunto un **secondo sensore**.  
Sul PCB è necessario creare un collegamento dal lato Arduino Mega tra il **pin A4** e il pin del connettore **PERIMET** (quello che in origine era non utilizzato).

### Collegamento su PCB
![Modifica PCB](Modifiche%20da%20effettuare/Modifica%20PCB.jpg)

---

### Installazione dei sensori sul robot
Il robot ora utilizza due sensori:  
- **Sensore A4 → lato destro** (guardando il robot da dietro)  
- **Sensore A5 → lato sinistro**

I supporti dei sensori vanno fissati nei due fori già presenti sulla protezione inferiore.  
Dopo aver fissato i supporti del sensore DX e SX sui vecchi fori, è possibile realizzare in opera i fori supplementari per fissare i supporti.

![Doppio sensore filo perimetrale](Modifiche%20da%20effettuare/Doppio%20sensore%20filo%20perimetrale.jpg)

---

## ⚡ Modifica 2: Sensore di tensione Arduino

Per utilizzare una batteria **LiPo fino a 8S**, è necessario modificare il modulo di rilevamento tensione.  
La modifica consiste nella sostituzione della resistenza da **30kΩ** con una da **75kΩ**.

![Sensore voltaggio Arduino Mod](Modifiche%20da%20effettuare/Sensore%20voltaggio%20Arduino%20Mod.jpg)

---



## 🌀 Modifica 3: Rampa di accelerazione motori

Creata una **rampa di accelerazione** completamente regolabile sia in tempo che in modello,  
per rendere l'avvio dopo ogni manovra più soft e meno stressante per la meccanica.


## 📌 Note finali
- Queste modifiche sono state testate con successo sul campo.  
- Con il doppio sensore filo, la tracciatura risulta più stabile e precisa.  
- Con il nuovo sensore di tensione, è ora possibile monitorare in sicurezza batterie LiPo fino a 8S.
