#include "Sensor.h"

Sensor::Sensor() {
    _state = HIGH;
    _currValue = 1024;
    _prevValue = 1024;
    _eventIntCntr = 0;
    _timeOutFlag = true;
    _numEvents = 0;
    _valueBufPos = 0;
    pinMode(SENSOR_LED_PIN,OUTPUT);
    digitalWrite(SENSOR_LED_PIN,LOW);
    for (int i=0; i<SENSOR_BUF_SZ; i++) {
        _valueBuf[i] = 1024;
    }
}

void Sensor::update() {
    int val; 
    val = analogRead(SENSOR_AIN);

    // Increment event interval counter and check for timeout
    _eventIntCntr++;
    if (_eventIntCntr > SENSOR_TIMEOUT_CNT) {
        _numEvents = 0;
        _timeOutFlag = true;
    }

    if (_state == HIGH) {
        if (val < (SENSOR_THRESHOLD - SENSOR_HYSTERESIS)) {
            high2lowEvent();
        }
    }
    else {
        if (val > (SENSOR_THRESHOLD + SENSOR_HYSTERESIS)) {
            low2highEvent();
        }
    }
    _prevValue = _currValue;
    _currValue = val;
    updateValueBuf(val);
}

void Sensor::high2lowEvent() {
    // Handle high-to-low transition event
    _state = LOW;
    digitalWrite(SENSOR_LED_PIN,HIGH);
    if (_timeOutFlag == false) {
        // Record high to low transition event
        if (_numEvents < 3) {
            _eventBuf[_numEvents] = _eventIntCntr;
        }
        else {
            _eventBuf[0] = _eventBuf[1];
            _eventBuf[1] = _eventBuf[2];
            _eventBuf[2] = _eventIntCntr;
        }
        _numEvents++;
        if (_numEvents > 3) {
            _numEvents == 3;
        }
    }
    // Resent event interval counter and time out flag.
    _eventIntCntr = 0;
    _timeOutFlag = false;
}

void Sensor::low2highEvent() {
    // Handle low-to-high transition event
    _state = HIGH;
    digitalWrite(SENSOR_LED_PIN,LOW);
}

uint16_t Sensor::getCurrValue() {
    // Return the current sensor value.
    return _currValue;
}

uint16_t Sensor::getPrevValue() {
    // Return the previous sensor value.
    return _prevValue;
}

uint16_t Sensor::getPeriod() {
    // Calculate the period of the pendulum using every other high-to-low
    // transision.
    uint16_t period;
    if (_numEvents < 3) {
        period = 0;
    }
    else {
        period = _eventBuf[2] + _eventBuf[1];
    }
    return period;
}

void Sensor::updateValueBuf(uint16_t value) {
    _valueBuf[_valueBufPos] = value;
    _valueBufPos++;
    if (_valueBufPos >= SENSOR_BUF_SZ) {
        _valueBufPos = 0;
    }
}


uint16_t Sensor::getMinValueInBuf() {
    uint16_t value;
    uint16_t minValue = 1024;
    for (int i=0; i<SENSOR_BUF_SZ; i++) {
        value = _valueBuf[i];
        if (value < minValue) {
            minValue = value;
        }
    }
    return minValue;
}
