#include <Wire.h>

const uint8_t IMU_ADDR = 0x68;

void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);          // SDA, SCL
  writeReg(0x6B, 0x00);        // wake up
  writeReg(0x1C, 0x10);        // accel range +/-8g
  delay(100);
}

void loop() {
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(IMU_ADDR, (uint8_t)6);

  int16_t rx = (Wire.read() << 8) | Wire.read();
  int16_t ry = (Wire.read() << 8) | Wire.read();
  int16_t rz = (Wire.read() << 8) | Wire.read();

  float ax = rx / 4096.0, ay = ry / 4096.0, az = rz / 4096.0;
  float mag = sqrt(ax*ax + ay*ay + az*az);

//Tests to run to see behavior:

  /*
  // print only when something interesting happens
  if (mag < 0.8 || mag > 1.3) {
    Serial.printf("%lu ms  |a|=%.2f\n", millis(), mag);
  } */

  /*if (++n % 10 == 0) {      // print every 10th reading, about 20 lines per second
  Serial.printf("%lu ms  |a|=%.3f  x=%.3f y=%.3f z=%.3f\n", millis(), mag, ax, ay, az);
} */
  delay(5);   // about 200 readings per second
}