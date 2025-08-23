
// ===== STRINGHE IN PROGMEM =====
const char msg_front[] PROGMEM = "Ostacolo Frontale";
const char msg_right[] PROGMEM = "Ostacolo Lat. DX";
const char msg_left[] PROGMEM = "Ostacolo Lat. SX";
const char msg_behind[] PROGMEM = "Ostacolo Dietro";

// ===== FUNZIONI HARDWARE MOTORE/LCD =====
void StopLidarMotor() {
    analogWrite(RPLIDAR_MOTOR, 0);
    Serial.println(F("LiDAR Motor Stopped"));
}

void StartLidarMotor() {
    analogWrite(RPLIDAR_MOTOR, 255);
    Serial.println(F("LiDAR Motor Started"));
}

void SetPins_ToTurnRightSlow() {
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
    analogWrite(ENAPin, PWM_Max_Slow_Speed_RH);
    analogWrite(ENBPin, PWM_Max_Slow_Speed_LH);
    Serial.print(F("Wheel:R_SLOW|"));
}

void SetPins_ToTurnLeftSlow() {
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
    analogWrite(ENAPin, PWM_Max_Slow_Speed_RH);
    analogWrite(ENBPin, PWM_Max_Slow_Speed_LH);
    Serial.print(F("Wheel:L_SLOW|"));
}

void UpdateDistanceMap(int angle, long distance) {
    int index = angle % ANGLE_RESOLUTION;
    if (index >= 0 && index < ANGLE_RESOLUTION) {
        distanceMap[index] = (int)distance; // salva come int (max 32k)
    }
}

void AdjustMotorSpeed(long distance, int maxSpeed) {
    int speed = map(distance, 0, 500, maxSpeed / 2, maxSpeed);
    speed = constrain(speed, 0, maxSpeed);
    analogWrite(ENAPin, speed);
    analogWrite(ENBPin, speed);
}

void AvoidObstacle(long minDistance, int angleAtMinDist) {
    if (minDistance < maxdistanceLIDAR) {
        AdjustMotorSpeed(minDistance, PWM_Max_Slow_Speed_RH);

        if ((angleAtMinDist >= 0 && angleAtMinDist <= 15) || (angleAtMinDist >= 345 && angleAtMinDist < 360)) {
            Serial.println(F("Obstacle Front Detected"));
            lcd.clear();
            lcd.setCursor(0, 0);
            char buffer[20];
            strcpy_P(buffer, msg_front);
            lcd.print(buffer);

            Motor_Action_Stop_Motors();
            delay(1000);
            SetPins_ToGoBackwards();
            Motor_Action_Go_Accel(Mower_Reverse_Delay);
            delay(Mower_Reverse_Delay);

            if (angleAtMinDist >= 0 && angleAtMinDist <= 180) {
                lcd.setCursor(0, 1);
                lcd.print(F("  Giro a Destra "));
                SetPins_ToTurnRightSlow();
            } else {
                lcd.setCursor(0, 1);
                lcd.print(F(" Giro a Sinistra"));
                SetPins_ToTurnLeftSlow();
            }

            delay(random(Mower_Turn_Delay_Min, Mower_Turn_Delay_Max));
            targetHeading = Compass_Heading_Degrees;
            SetPins_ToGoForwards();
        }

        else if (angleAtMinDist > 15 && angleAtMinDist <= 90) {
            Serial.println(F("Obstacle Lateral Right Detected"));
            lcd.clear();
            lcd.setCursor(0, 0);
            char buffer[20];
            strcpy_P(buffer, msg_right);
            lcd.print(buffer);

            if (minDistance < 500) {
                int adjustedSpeed = map(minDistance, 0, 500, PWM_Max_Slow_Speed_LH / 4, PWM_Max_Slow_Speed_LH);
                analogWrite(ENBPin, adjustedSpeed);
                analogWrite(ENAPin, PWM_Max_Slow_Speed_RH);
            }

            targetHeading = Compass_Heading_Degrees;
            SetPins_ToGoForwards();
        }

        else if (angleAtMinDist >= 270 && angleAtMinDist < 345) {
            Serial.println(F("Obstacle Lateral Left Detected"));
            lcd.clear();
            lcd.setCursor(0, 0);
            char buffer[20];
            strcpy_P(buffer, msg_left);
            lcd.print(buffer);

            if (minDistance < 500) {
                int adjustedSpeed = map(minDistance, 0, 500, PWM_Max_Slow_Speed_RH / 4, PWM_Max_Slow_Speed_RH);
                analogWrite(ENAPin, adjustedSpeed);
                analogWrite(ENBPin, PWM_Max_Slow_Speed_LH);
            }

            targetHeading = Compass_Heading_Degrees;
            SetPins_ToGoForwards();
        }
    }
}

void printData(int angle, long distance) {
    Serial.print(F("Min distance: "));
    Serial.print(distance);
    Serial.print(F(" at angle: "));
    Serial.println(angle);
}

// ===== FUNZIONE PRINCIPALE DI GESTIONE LIDAR =====
void Check_LIDAR() {
    if (RPLIDAR_Enabled) {
        if (Mower_Docked == 1 || Mower_Parked == 1) {
            StopLidarMotor();
            return;
        }

        if (IS_OK(lidar.waitPoint())) {
            long distance = lidar.getCurrentPoint().distance;
            int angle = ((int)lidar.getCurrentPoint().angle) % 360;  // CORRETTO QUI

            if (distance > 100 && distance < MAX_DISTANCE && distance < minDistance) {
              // Accetta solo angoli nei range validi
              if ((angle >= 330 || angle <= 30) || 
              (angle > 15 && angle <= 90) || 
              (angle >= 270 && angle < 345)) {

                  minDistance = distance;
                  angleAtMinDist = angle;
                }
            }


            UpdateDistanceMap(angle, distance);

            if (lidar.getCurrentPoint().startBit) {
                printData(angleAtMinDist, minDistance);
                AvoidObstacle(minDistance, angleAtMinDist);
                minDistance = maxdistanceLIDAR;
                angleAtMinDist = 0;
            }

        } else {
            StopLidarMotor();
            rplidar_response_device_info_t info;
            if (IS_OK(lidar.getDeviceInfo(info, 100))) {
                lidar.startScan();
                StartLidarMotor();
                delay(1000);
            }
        }
    }
}
