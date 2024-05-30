#include "temperature_data.h"

TemperatureData::TemperatureData(time_t timestamp, double min, double max, double avg) : defaultData(timestamp, avg), max(max), min(min) {
    this->timestamp = timestamp;
    this->avg = avg;
    this->min = min;
    this->max = max;
    this->next = NULL;
    this->unit = "celcius";
}

void TemperatureData::setMin(double min) {
    this->min = min;
}

double TemperatureData::getMin() {
    return this->min;
}

void TemperatureData::setMax(double max) {
    this->max = max;
}

double TemperatureData::getMax() {
    return this->max;
}