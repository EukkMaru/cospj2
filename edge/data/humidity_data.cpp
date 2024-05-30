#include "humidity_data.h"

HumidityData::HumidityData(time_t timestamp, double min, double max, double avg) : defaultData(timestamp, avg), max(max), min(min) {
    this->timestamp = timestamp;
    this->avg = avg;
    this->min = min;
    this->max = max;
    this->next = NULL;
    this->unit = "\%";
}

void HumidityData::setMin(double min) {
    this->min = min;
}

double HumidityData::getMin() {
    return this->min;
}

void HumidityData::setMax(double max) {
    this->max = max;
}

double HumidityData::getMax() {
    return this->max;
}