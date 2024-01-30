#ifndef _U_SENSOR_A_01_H_
#define _U_SENSOR_A_01_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

extern SoftwareSerial uSonicSensor;
extern unsigned char data[4];

unsigned char readData(unsigned char* _data);
bool checkSum(unsigned char* _data);
uint16_t getDistance(unsigned char* _data);

#endif /* _U_SENSOR_A_01_H_ */
