#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#include "data.h"

class TemperatureData : public defaultData<TemperatureData> {
  private:
    double min;
    double max;

  public:
    TemperatureData(time_t timestamp, double min, double max, double avg);

    void setMin(double min);
    double getMin();

    void setMax(double max);
    double getMax();
};

#endif /* __TEMPERATURE_H__ */
