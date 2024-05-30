#ifndef PARSER_H
#define PARSER_H

#include <cstdint>

struct ParsedData {
    int8_t temperature;
    uint8_t humidity;
    uint16_t power;
    uint8_t month;
};

void parseData(uint8_t *packedData, ParsedData &data);

#endif
