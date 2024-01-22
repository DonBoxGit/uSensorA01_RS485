/* 
   This sketch use A01NYUB ultrasonic sensor(Copyright DFRobot 2016)
   to measure distance and transmitting it to use RS-485 interface
   and ModBus protocol. 
*/
   
#include <ModbusRtu.h>
#include "u_sensor_a01.h"

#define ID_SLAVE                  1
#define RS485_TX_RX_MODULE_PIN    6
#define SENSOR_RESPONCE_TIME_MS   150     // 150ms - 300ms
#define U_SENSOR_BAUDRATE         9600
#define SERIAL_BAUDRATE           115200

SoftwareSerial uSonicSensor(11, 10); // RX, TX
Modbus modbus(ID_SLAVE, Serial, RS485_TX_RX_MODULE_PIN);

uint16_t modbus_array[3] = {};
unsigned char data[4] = {};
uint16_t distance = 0;
uint32_t tmr = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  uSonicSensor.begin(U_SENSOR_BAUDRATE);
  modbus.start();
  tmr = millis();
}

void loop() {
  modbus_array[0] = distance;
  modbus.poll(modbus_array, sizeof(modbus_array) / sizeof(modbus_array[0]));
  if (millis() - tmr > SENSOR_RESPONCE_TIME_MS) {
    if (readData(data, sizeof(data) / sizeof(data[0])) == 0xFF) {
      if (checkSum(data)) {
        distance = getDistance(data);
      } else distance = 0;
    }
    tmr = millis();
  }
}
