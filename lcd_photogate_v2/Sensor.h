#ifndef _SENSOR_H_
#define _SENSOR_H_
#include "WProgram.h"

#define SENSOR_AIN A0
#define SENSOR_THRESHOLD 500
#define SENSOR_HYSTERESIS 20
#define SENSOR_LED_PIN 10
#define SENSOR_TIMEOUT_CNT 2000
#define SENSOR_BUF_SZ 20

class Sensor {
    private:
        uint8_t _state;
        uint16_t _currValue;
        uint16_t _prevValue;
        uint16_t _eventIntCntr;
        uint8_t _numEvents;
        uint16_t _eventBuf[3];
        uint16_t _valueBuf[SENSOR_BUF_SZ];
        uint16_t _valueBufPos;
        bool _timeOutFlag;  
        void high2lowEvent();
        void low2highEvent();
        void updateValueBuf(uint16_t value);
        
    public:
        Sensor();
        void update();
        uint8_t getState();
        uint16_t getCurrValue();
        uint16_t getPrevValue();
        uint16_t getPeriod();
        uint16_t getMinValueInBuf();
};
#endif
