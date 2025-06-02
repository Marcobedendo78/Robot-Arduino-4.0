void CalibraGyroZ() {
  long somma = 0;
  for (int i = 0; i < 500; i++) {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x47); // GYRO_ZOUT_H
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 2, true);
    int16_t z = Wire.read() << 8 | Wire.read();
    somma += z;
    delay(2);
  }
  gyroBias = somma / 500;
  Serial.print("Bias GYRO Z calcolato: ");
  Serial.println(gyroBias);
}

void Get_GYRO_Reading() {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(true); 
    Wire.requestFrom(MPU_addr, 14, true);

    if (Wire.available() == 14) {
        // Salta acc e temp se non usi
        Wire.read(); Wire.read(); // AcX
        Wire.read(); Wire.read(); // AcY
        Wire.read(); Wire.read(); // AcZ
        Wire.read(); Wire.read(); // Temp
        Wire.read(); Wire.read(); // GyX
        Wire.read(); Wire.read(); // GyY

        GyZ = Wire.read() << 8 | Wire.read(); // GyZ

        // Calcolo velocità angolare in °/s (sensibilità = 131 per ±250°/s)
        gyroZrate = -(GyZ - gyroBias) / 131.0;

        // Calcolo del deltaTime
        unsigned long now = millis();
        float deltaTime = (now - lastTime) / 1000.0;
        lastTime = now;

        // Integrazione per ottenere Gyro Heading cumulato (in gradi)
        Gyro_Heading += gyroZrate * deltaTime;

        // Normalizza tra 0° e 360°
        if (Gyro_Heading < 0) Gyro_Heading += 360;
        if (Gyro_Heading >= 360) Gyro_Heading -= 360;

        Serial.print("GyZrate: "); Serial.print(gyroZrate);
        Serial.print(" | Gyro Heading: "); Serial.println(Gyro_Heading);
    }
}
