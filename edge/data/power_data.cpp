#include "power_data.h"

PowerData::PowerData(time_t timestamp, double avg) : defaultData(timestamp, avg) {
    this->timestamp = timestamp;
    this->avg = avg;
    this->next = NULL;
    this->unit = "kWh";
}
