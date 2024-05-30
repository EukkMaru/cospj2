#ifndef __HUMIDITY_DATA_H__
#define __HUMIDITY_DATA_H__

#include "data.h"

class HumidityData : public defaultData<HumidityData> {
  private:
    double min;
    double max;

  public:
    HumidityData(time_t timestamp, double min, double max, double avg);

    void setMin(double min);
    double getMin();

    void setMax(double max);
    double getMax();
};

#endif /* __HUMIDITY_DATA_H__ */
