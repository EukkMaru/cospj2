#include "process_manager.h"
#include "opcode.h"
#include "byte_op.h"
#include "setting.h"
#include <cstring>
#include <iostream>
#include <ctime>
using namespace std;

#define POWER_OFFSET 120
#define TEMP_OFFSET 15

ProcessManager::ProcessManager()
{
  this->num = 0;
}

void ProcessManager::init()
{
}

// TODO: You should implement this function if you want to change the result of the aggregation
uint8_t *ProcessManager::processData(DataSet *ds, int *dlen)
{
  uint8_t *ret, *p;
  int num, len;
  HouseData *house;
  Info *info;
  TemperatureData *tdata;
  HumidityData *hdata;
  PowerData *pdata;
  char buf[BUFLEN];
  ret = (uint8_t *)malloc(BUFLEN);
  int tmp, avg_temp, avg_power, month;
  unsigned int avg_humid;
  time_t ts;
  struct tm *tm;

  tdata = ds->getTemperatureData();
  hdata = ds->getHumidityData();
  num = ds->getNumHouseData();

  avg_temp = (int) tdata->getValue();
  avg_temp += TEMP_OFFSET; // adding the offset will shift the range from -15~48 to 0~63, which can then be encoded in 6 bits

  avg_humid = (unsigned int) hdata->getValue();
  avg_humid = static_cast<unsigned int>(avg_humid * 63 / 100);

  avg_power = 0;
  for (int i=0; i<num; i++)
    avg_power += (int) ds->getHouseData(i)->getPowerData()->getValue();
  avg_power /= num;
  avg_power -= POWER_OFFSET;

  ts = ds->getTimestamp();
  tm = localtime(&ts);
  month = tm->tm_mon + 1;

  // initializing the memory to send to the network manager
  memset(ret, 0, BUFLEN);
  *dlen = 0;
  p = ret;

  uint32_t packed = 0; // 00000000 00000000 00000000 00000000

  /*
  We can reduce the number of bits using bitwise AND operations.
  For example, if we have an 8-bit value 0b10101010, we can use only the final 6 digit of the value by &ing it with 0b00111111 (0X3F). We can then use the left shift operations (<<) to account for the missing bits.

  Bitwise OR
  */
  packed |= (avg_temp & 0x3F);
  /*
  Since avg_temp is a singned integer, its range in 6 bits is from -32 to 31. We can apply the offset of 17 to shift the range to -15 to 48, which roughly corresponds to the range of temperature values in the real world.
  This way, we can fit the temperature value in 6 bits.

  Example: 23 -> 23 + 17 = 40 -> 0b00101000 (0x28)
  0x28 & 0x3F = 0x28 (Trim 6 digits)
  packed = 0b0000...0000 | 0b101000 = 0b0000...101000
  > adds our data to the right most 6 bits of the packed variable
  */
  packed |= (avg_humid & 0x3F) << 6;
  /*
  Avg_humid has been scaled down to 0-63, which fits in 6 bits.
  
  Example: 50% -> 50 * 63 / 100 = 31.5 -> 31
  We can bit shift the value by 6 bits to the left to make room for the next value.
  0x1F = 0b00011111 << 6 = 0b00011111000000
  We can OR the packed variable with the shifted value to add the humidity data to the packed variable.
  */

  //same for power, month, but with different bit size and offset
  packed |= (avg_power & 0xFF) << 12;
  packed |= (month & 0x0F) << 20;

  // If we were to add more information, we can do the same by trimming the bits to their minimum rather than a multiple of 4, and shifting them to the left by the appropriate amount. In that case we should use another macro to save the value to memory as the total packed value is now more than 3 bytes.

  /*
  Example:
  packed |= (additional_data & data_size) << 24;
  VAR_TO_MEM_4BYTES_BIG_ENDIAN(packed, p);
  *dlen += 4;
  */

  // saving the values in the memory
  VAR_TO_MEM_3BYTES_BIG_ENDIAN(packed, p);
  *dlen += 3;

  return ret;
}
