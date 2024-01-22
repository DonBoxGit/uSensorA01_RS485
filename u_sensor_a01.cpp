#include "u_sensor_a01.h"

unsigned char readData(unsigned char* _data, uint8_t _size) {
  do {
      for (int i = 0; i < _size; i++)
        _data[i] = uSonicSensor.read();
    } while (uSonicSensor.read() == 0xFF);
    uSonicSensor.flush();
    return _data[0];
}

bool checkSum(unsigned char* _data) {
  bool check;
  if (((_data[0] + _data[1] + _data[2]) & 0x00FF) == _data[3]) check = true;
  else check = false;
  return check;
}

uint16_t getDistance(unsigned char* _data) {
  return (_data[1] << 8) + _data[2];
}
