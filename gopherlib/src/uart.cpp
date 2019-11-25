#include "gopher/uart.h"
#include "pros/apix.h"


// I have no idea what the signature of this method is (the parameter is an educated guess)
// But this works so ¯\_(ツ)_/¯
extern "C" int32_t vexSerialReadChar(uint8_t timeout);

// This file shamelessly stolen from https://github.com/asgard-mqp/robot_driver
// I fixed some bugs and switched to non-blocking reads

union converter {
  int32_t fullData; //occupies 4 bytes
  uint8_t bytes[4]; //occupies 4 bytes
};

void writeUart(uint8_t packet_id, int32_t value) {
  union converter out_data;
  out_data.fullData = value;
  fputc(0xFA, stdout);
  fputc(packet_id, stdout);
  uint8_t checksum = 255;

  for (int i = 0; i < 4; i++) {
    checksum -= out_data.bytes[i];
    fputc(out_data.bytes[i], stdout);
  }

  fputc(checksum, stdout);
}

// Read count bytes from serial into buf
// Returns number of bytes actually read
int readBytes(int count, uint8_t * buf) {
  for (int i = 0; i < count; i++) {
    int32_t byte = vexSerialReadChar(1);
    if (byte == -1L) {
      return i;
    }
    buf[i] = (uint8_t) byte;
  }
  return count;
}

void readUart(uint8_t *packet_id, int32_t *value) {
  union converter in_data;

  uint8_t startByte = 0;
  uint8_t data[7];

  int count = readBytes(1, data);
  if (count == 0) {
    *packet_id = 0;
    return;
  }

  startByte = data[0];

  if(startByte != 0xFA){
    *packet_id = 0;//just to make sure its denied
    return;
  }
  count = readBytes(6, &data[1]);
  if (count < 6) {
    *packet_id = 0;
    return;
  }

  *packet_id = data[1];

  uint8_t checksum_calc = 255;
  for (int i = 0; i < 4; i++) {
    in_data.bytes[i] = data[i+2];
    checksum_calc -= data[i+2];
  }

  uint8_t checksum_given = data[6];

  if (checksum_calc == checksum_given) {
	  *value = in_data.fullData;
  } else {
      *packet_id = 0;
  }
}