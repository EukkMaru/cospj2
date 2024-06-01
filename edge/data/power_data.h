#ifndef __POWER_DATA_H__
#define __POWER_DATA_H__

#include "default_data.h"

class PowerData : public defaultData<PowerData> {
  public:
    PowerData(time_t timestamp, double avg);
};

#endif /* __POWER_DATA_H__ */