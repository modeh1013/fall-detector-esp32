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
  Wire.write(0x3B);            // first accel register
  Wire.endTransmission(false);
  Wire.requestFrom(IMU_ADDR, (uint8_t)6);

  int16_t rx = (Wire.read() << 8) | Wire.read();
  int16_t ry = (Wire.read() << 8) | Wire.read();
  int16_t rz = (Wire.read() << 8) | Wire.read();

  float ax = rx / 4096.0, ay = ry / 4096.0, az = rz / 4096.0;
  float mag = sqrt(ax*ax + ay*ay + az*az);

  Serial.printf("x=%.2f y=%.2f z=%.2f |a|=%.2f g\n", ax, ay, az, mag);
  delay(20);                   // about 50 readings per second
}