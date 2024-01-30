/*******************************************************************************
*  @file       uSensorA01_RS485.ino 
*  @brief      This sketch is measure distance with ultrasonic sensor and 
*              transmitting it to use ModBusRTU protocol and RS-485 interface.
*  @copyright  DFRobot, 2016 GUN Lesser General Pulic License for A01NYUB
*              ultrasonic sensor.
*  @copyright  Samuel Marco i Armengol, 2016 GNU Lesser General Public License
*              as published by the Free Software Foundation for Modbus devices.
*  @version    2.1.1
*  @data       30.01.2024
*  @author     YRN
*******************************************************************************/

#include <ModbusRtu.h>
#include <SoftwareSerial.h>

#define ID_SLAVE                    1     /* Modbus slave address */
#define RS485_TX_RX_MODULE_PIN      6     /* Direction pin RS485 module */
#define U_SENSOR_RX_PIN             11
#define U_SENSOR_TX_PIN             10
#define U_SENSOR_RESPONCE_TIME_MS   150   /* 100ms - 300ms */
#define U_SENSOR_BAUDRATE           9600  
#define SERIAL_BAUDRATE             9600
#define AMOUNT_COIL                 1     /* 1 coil(Holding or Input register) of data in modbus_array */

SoftwareSerial uSonicSensor(U_SENSOR_RX_PIN, U_SENSOR_TX_PIN);
Modbus modbus(ID_SLAVE, Serial, RS485_TX_RX_MODULE_PIN);

uint16_t modbus_array[AMOUNT_COIL] = {};
unsigned char data[4] = {};
uint16_t distance = 0;
uint32_t tmr = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  uSonicSensor.begin(U_SENSOR_BAUDRATE);
  modbus.start();
  delay(U_SENSOR_RESPONCE_TIME_MS + 10);
}

void loop() {
  if (millis() - tmr > U_SENSOR_RESPONCE_TIME_MS) {
    do {
      for (int i = 0; i < 4; i++)
        data[i] = uSonicSensor.read();
    } while (uSonicSensor.read() == 0xff);
    uSonicSensor.flush();

    if (data[0] == 0xff) {
      int sum;
      sum = (data[0] + data[1] + data[2]) & 0x00FF;
      if (sum == data[3]) {
        distance = (data[1] << 8) + data[2];
        if (distance < 280)  /* Low limit of the ultrasonic sensor */
          distance = 1;      /* Distance is below the lower limit */   
      } else distance = 0;   /* Distance measuring is ERROR */
    }
    modbus_array[0] = distance;
    tmr = millis();
  }

  modbus.poll(modbus_array, AMOUNT_COIL);
}
