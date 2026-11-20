#include <Wire.h>

const uint8_t IMU_ADDR = 0x68;
const uint8_t WHO_AM_I_REG = 0x75;

const uint8_t SDA_PIN = 21;
const uint8_t SCL_PIN = 22;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // UART speed between ESP32 and PC's serial monitor = 115200, this is the rate the devices need to be at to communicate with each other
  // 1. Start I2c on SDA = 21, SCL = 22
  Wire.begin(SDA_PIN, SCL_PIN);
  // 2. Begin a transmission to the IMU
  Wire.beginTransmission(IMU_ADDR); // opens a transmit buffer in memory targeted at 0x68
  // 3. Write the register number you want. This points to the WHO_AM_I register
  Wire.write(WHO_AM_I_REG);

  // 4. End transmission without a stop, save the return code print it
  // this sends the queued bytes but still keeps the bus active since we didn't use a STOP
  uint8_t status = Wire.endTransmission(false);
  Serial.print("Status: " );
  Serial.println(status);

  // 5. Request 1 byte from the MU 
  uint8_t bytesReceived = Wire.requestFrom(IMU_ADDR, (uint8_t)1);
  Serial.print("Bytes received: ");
  Serial.println(bytesReceived);

  // 6. Read the byte and print it in HEX
  // Note that reading the byte means to do Wire.read and copy the byte out of ESP32's temporary receive buffer into a variable into my code 

  if (Wire.available() > 0){
    uint8_t whoAmI = Wire.read(); // stores byte from buffer into a variable in our code
    Serial.print("WHO_AM_I value: 0x");
    Serial.println(whoAmI, HEX); // print in hex

  }
  else{
    Serial.println("No byte received from IMU.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
