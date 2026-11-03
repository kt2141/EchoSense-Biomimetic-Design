const uint8_t PIN_TRIG = 9;
const uint8_t PIN_ECHO = 10;
const uint8_t PIN_MOTOR_1 = 3;
const uint8_t PIN_MOTOR_2 = 5;
const uint8_t PIN_MOTOR_3 = 6;
const uint8_t PIN_LED = 13;
const uint8_t PIN_BAT = A0;

const float SOUND_SPEED_CM_US = 0.0343f;
const uint16_t ECHO_TIMEOUT_US = 25000;
const uint16_t MAX_DISTANCE_CM = 400;
const float EMA_ALPHA = 0.35f;
const uint8_t WINDOW_SIZE = 5;
const uint32_t SENSOR_INTERVAL_MS = 35;
const float BATTERY_DIVIDER_RATIO = 2.0f;
const float ADC_REF_VOLT = 5.0f;
const uint16_t LOW_BATTERY_MV = 3500;

enum Zone : uint8_t { ZONE_CLEAR, ZONE_AWARE, ZONE_NEAR, ZONE_DANGER };

float rawWindow[WINDOW_SIZE] = {MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM};
uint8_t windowIndex = 0;
bool windowFilled = false;
float emaCm = MAX_DISTANCE_CM;
uint32_t lastSensorMs = 0;
uint32_t lastPulseToggleMs = 0;
bool pulseOn = true;

float measureDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  uint32_t durationUs = pulseIn(PIN_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (durationUs == 0) return MAX_DISTANCE_CM;

  float distance = (durationUs * SOUND_SPEED_CM_US) / 2.0f;
  if (distance < 0) distance = 0;
  if (distance > MAX_DISTANCE_CM) distance = MAX_DISTANCE_CM;
  return distance;
}

float median5(const float *arr) {
  float sorted[WINDOW_SIZE];
  for (uint8_t i = 0; i < WINDOW_SIZE; i++) sorted[i] = arr[i];
  for (uint8_t i = 0; i < WINDOW_SIZE - 1; i++) {
    for (uint8_t j = i + 1; j < WINDOW_SIZE; j++) {
      if (sorted[j] < sorted[i]) {
        float tmp = sorted[i];
        sorted[i] = sorted[j];
        sorted[j] = tmp;
      }
    }
  }
  return sorted[WINDOW_SIZE / 2];
}

Zone zoneFromDistance(float cm) {
  if (cm > 200.0f) return ZONE_CLEAR;
  if (cm >= 100.0f) return ZONE_AWARE;
  if (cm >= 50.0f) return ZONE_NEAR;
  return ZONE_DANGER;
}

float dutyFromDistance(float cm) {
  if (cm > 200.0f) return 0.0f;
  if (cm >= 100.0f) return 35.0f + ((200.0f - cm) / 100.0f) * 15.0f;
  if (cm >= 50.0f) return 65.0f + ((100.0f - cm) / 50.0f) * 15.0f;
  return 100.0f;
}

const char* zoneToString(Zone z) {
  switch (z) {
    case ZONE_CLEAR: return "CLEAR";
    case ZONE_AWARE: return "AWARE";
    case ZONE_NEAR: return "NEAR";
    default: return "DANGER";
  }
}

bool pulseEnvelopeForZone(Zone z, uint32_t nowMs) {
  if (z == ZONE_DANGER) return true;
  if (z == ZONE_CLEAR) return false;

  uint32_t halfPeriodMs = (z == ZONE_AWARE) ? 500 : 166;
  if (nowMs - lastPulseToggleMs >= halfPeriodMs) {
    pulseOn = !pulseOn;
    lastPulseToggleMs = nowMs;
  }
  return pulseOn;
}

void applyMotorPwm(uint8_t pwm) {
  analogWrite(PIN_MOTOR_1, pwm);
  analogWrite(PIN_MOTOR_2, pwm);
  analogWrite(PIN_MOTOR_3, pwm);
}

uint16_t readBatteryMillivolts() {
  uint16_t adc = analogRead(PIN_BAT);
  float v = (adc * ADC_REF_VOLT / 1023.0f) * BATTERY_DIVIDER_RATIO;
  return (uint16_t)(v * 1000.0f);
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_MOTOR_1, OUTPUT);
  pinMode(PIN_MOTOR_2, OUTPUT);
  pinMode(PIN_MOTOR_3, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  applyMotorPwm(0);
  Serial.begin(115200);
  Serial.println("timestamp_ms,raw_cm,filtered_cm,zone,pwm_duty");
}

void loop() {
  uint32_t nowMs = millis();
  if (nowMs - lastSensorMs < SENSOR_INTERVAL_MS) return;
  lastSensorMs = nowMs;

  float rawCm = measureDistanceCm();
  rawWindow[windowIndex] = rawCm;
  windowIndex = (windowIndex + 1) % WINDOW_SIZE;
  if (windowIndex == 0) windowFilled = true;

  float medianCm = windowFilled ? median5(rawWindow) : rawCm;
  emaCm = (EMA_ALPHA * medianCm) + ((1.0f - EMA_ALPHA) * emaCm);

  Zone zone = zoneFromDistance(emaCm);
  float duty = dutyFromDistance(emaCm);
  bool pulseOnNow = pulseEnvelopeForZone(zone, nowMs);

  uint8_t pwm = (zone == ZONE_DANGER)
    ? 255
    : (uint8_t)((duty / 100.0f) * (pulseOnNow ? 255.0f : 0.0f));
  applyMotorPwm(pwm);

  uint16_t battMv = readBatteryMillivolts();
  if (battMv < LOW_BATTERY_MV) {
    digitalWrite(PIN_LED, (nowMs / 150) % 2);
  } else {
    digitalWrite(PIN_LED, zone != ZONE_CLEAR);
  }

  Serial.print(nowMs);
  Serial.print(',');
  Serial.print(rawCm, 2);
  Serial.print(',');
  Serial.print(emaCm, 2);
  Serial.print(',');
  Serial.print(zoneToString(zone));
  Serial.print(',');
  Serial.println(duty, 1);
}
