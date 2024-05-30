#include "opcode.h"
#include "byte_op.h"
#include "setting.h"
#include "parse.h"
#include <cstring>
#include <iostream>
#include <ctime>
using namespace std;

#define POWER_OFFSET 120
#define TEMP_OFFSET 15

void parseData(uint8_t *packedData, ParsedData &data) {
    uint32_t packed = 0;
    
    // packedData is a 3-byte array
    packed = (packedData[0] << 16) | (packedData[1] << 8) | packedData[2];
    
    data.temperature = (packed & 0x3F) - TEMP_OFFSET;
    data.humidity = (packed >> 6) & 0x3F;
    data.humidity = static_cast<uint8_t>(data.humidity * 100 / 63);
    data.power = (packed >> 12) & 0xFF;
    data.month = (packed >> 20) & 0x0F;
}