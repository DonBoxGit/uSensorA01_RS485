/* This sketch use A01NYUB ultrasonic sensor(Copyright DFRobot 2016)
   to measure distance and transmitting it to use RS-485 protocol. */

#include <SoftwareSerial.h>

#define RS485_TX_RX_MODULE_PIN    6
#define RS485_TRANSMIT_MODE       1
#define RS485_RECEIVE_MODE        0
#define SENSOR_RESPONCE_TIME_MS   150     // 150ms - 300ms
#define U_SENSOR_BAUDRATE         9600
#define SERIAL_BAUDRATE           115200

SoftwareSerial uSonicSensor(11, 10); // RX, TX
unsigned char data[4] = {};
float distance;
uint32_t tmr = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  uSonicSensor.begin(U_SENSOR_BAUDRATE);

  pinMode(RS485_TX_RX_MODULE_PIN, OUTPUT);
  digitalWrite(RS485_TX_RX_MODULE_PIN, RS485_TRANSMIT_MODE);
  tmr = millis();
}

void loop() {
  if (millis() - tmr > SENSOR_RESPONCE_TIME_MS) {
    do {
      for (int i = 0; i < 4; i++)
        data[i] = uSonicSensor.read();
    } while (uSonicSensor.read() == 0xFF);
    uSonicSensor.flush();
  
    if (data[0] == 0xFF) {
      int sum;
      sum = (data[0] + data[1] + data[2]) & 0x00FF;
      if (sum == data[3]) {
        distance = (data[1] << 8) + data[2];
        if (distance > 280) {
          Serial.print("distance=");
          Serial.print(distance / 10);
          Serial.println("cm");
        } else Serial.println("Below the lower limit");
      } else Serial.println("ERROR");
    }
    tmr = millis();
  }
}
