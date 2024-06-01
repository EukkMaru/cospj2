#ifndef __DEFAULT_DATA_H__
#define __DEFAULT_DATA_H__

#include "info.h"
#include <ctime>
#include <random>
#include <string>

template<typename T> class defaultData {
  protected:
    time_t timestamp;
    double avg;
    string unit;
    T *next;

    defaultData(time_t timestamp, double avg) {
        this->timestamp = timestamp;
        this->avg = avg;
        this->next = NULL;
    }

  public:
    void setNext(T *data);
    T *getNext();

    void setValue(double value);
    double getValue();

    void setTimestamp(time_t timestamp);
    time_t getTimestamp();
    string getUnit();
};

template<typename T> void defaultData<T>::setNext(T* data) {
    this->next = data;
}

template<typename T> T *defaultData<T>::getNext() {
    return this->next;
}

template<typename T> void defaultData<T>::setValue(double value) {
    this->avg = value;
}

template<typename T> double defaultData<T>::getValue() {
    return this->avg;
}

template<typename T> void defaultData<T>::setTimestamp(time_t timestamp) {
    this->timestamp = timestamp;
}

template<typename T> time_t defaultData<T>::getTimestamp() {
    return this->timestamp;
}

#endif /* __DEFAULT_DATA_H__ */