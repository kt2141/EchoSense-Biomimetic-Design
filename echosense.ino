/*
  EchoSense: conceptual firmware (Arduino Nano + HC-SR04 + 3 coin ERM motors)
  Author: Aravind K T (240169), IIT Kanpur
  Wiring (conceptual):
    HC-SR04  TRIG -> D2, ECHO -> D3, VCC -> 5V, GND -> GND
    Motors   (each via NPN transistor/MOSFET + flyback diode): D5, D6, D9 (PWM)
*/
const uint8_t TRIG_PIN = 2;
const uint8_t ECHO_PIN = 3;
const uint8_t MOTOR_PINS[3] = {5, 6, 9};
enum Zone { CLEAR, AWARE, NEAR_, DANGER };
const int AWARE_CM = 200, NEAR_CM = 100, DANGER_CM = 50;
const uint8_t N = 4;
long samples[N];
uint8_t idx = 0;

long readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long t = pulseIn(ECHO_PIN, HIGH, 25000UL);
  if (t == 0) return 400;                 // no echo: treat as clear
  return (long)(t * 0.0343 / 2.0);        // d = v * t / 2
}
Zone zoneFor(long cm) {
  if (cm < DANGER_CM) return DANGER;
  if (cm < NEAR_CM)   return NEAR_;
  if (cm < AWARE_CM)  return AWARE;
  return CLEAR;
}
void setMotors(uint8_t duty) { for (uint8_t i = 0; i < 3; i++) analogWrite(MOTOR_PINS[i], duty); }
void pulse(float rateHz, uint8_t duty) {
  unsigned long period = (unsigned long)(1000.0 / rateHz);
  setMotors((millis() % period) < (period / 3) ? duty : 0);
}
void renderHaptics(Zone z) {
  switch (z) {
    case CLEAR:  setMotors(0);    break;   // silence = clear path
    case AWARE:  pulse(1.0, 120); break;   // gentle pulse
    case NEAR_:  pulse(3.0, 180); break;   // faster pulse
    case DANGER: setMotors(255);  break;   // continuous buzz
  }
}
void setup() {
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  for (uint8_t i = 0; i < 3; i++) pinMode(MOTOR_PINS[i], OUTPUT);
  for (uint8_t i = 0; i < N; i++) samples[i] = 400;
}
void loop() {
  samples[idx] = readDistanceCm(); idx = (idx + 1) % N;
  long sum = 0; for (uint8_t i = 0; i < N; i++) sum += samples[i];
  renderHaptics(zoneFor(sum / N));
  delay(30);
}
