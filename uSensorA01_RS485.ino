/*******************************************************************************
*  @file       uSensorA01_RS485.ino 
*  @brief      This sketch is measure distance with ultrasonic sensor and 
*              transmitting it to use ModBusRTU protocol and RS-485 interface.
*  @copyright  DFRobot, 2016 GUN Lesser General Pulic License for A01NYUB
*              ultrasonic sensor.
*  @copyright  Samuel Marco i Armengol, 2016 GNU Lesser General Public License
*              as published by the Free Software Foundation for Modbus devices.
*  @version    2.0.1
*  @data       29.01.2024
*  @author     YRN
*******************************************************************************/

#include <ModbusRtu.h>
#include "u_sensor_a01.h"

#define ID_SLAVE                    1     /* Modbus slave address */
#define RS485_TX_RX_MODULE_PIN      6     /* Direction pin RS485 module */
#define U_SENSOR_RX_PIN             11
#define U_SENSOR_TX_PIN             10
#define U_SENSOR_RESPONCE_TIME_MS   150   /* 100ms - 300ms */
#define U_SENSOR_BAUDRATE           9600  
#define SERIAL_BAUDRATE             9600
#define AMOUNT_COIL                 1     /* 1 coil of 16-bit data */

SoftwareSerial uSonicSensor(U_SENSOR_RX_PIN, U_SENSOR_TX_PIN);
Modbus modbus(ID_SLAVE, Serial, RS485_TX_RX_MODULE_PIN);

uint16_t modbus_array[AMOUNT_COIL] = {};
uint16_t distance = 0;
uint32_t tmr = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  uSonicSensor.begin(U_SENSOR_BAUDRATE);
  modbus.start();
  tmr = millis();
}

void loop() {
  if (millis() - tmr > U_SENSOR_RESPONCE_TIME_MS) {
    if (readData(data) == 0xFF) {
      if (checkSum(data)) {
        distance = getDistance(data); /* Get data of distance if checkSum is OK */
      } else distance = 0;            /* Distance = 0 if checkSum is error */
    }
    modbus_array[0] = distance;
    tmr = millis();
  }

  modbus.poll(modbus_array, AMOUNT_COIL);
}
