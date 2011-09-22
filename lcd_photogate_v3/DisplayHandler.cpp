#include "util/atomic.h"
#include "DisplayHandler.h"

DisplayHandler::DisplayHandler(Print &port) {
    _lcd.setPort(port); 
    for (int i=1; i<DISPLAY_TRACE_BUF_LEN; i++) {
        _traceBuf[i] = 0;
    }
    _tracePlotPos = 0;
    _traceClearPos = DISPLAY_CLEAR_OFFSET;
}

void DisplayHandler::init() {
    _lcd.clearScreen();
    setBrightness();
}

void DisplayHandler::setBrightness() {
    // Read Potentiometer value and use value to set brightness
    // of LCD.
    int pot;
    int brightness;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        pot = analogRead(DISPLAY_POT_AIN);
    }
    brightness = map(pot,0,1024,0,100);
    _lcd.setBrightness(brightness);
}

void DisplayHandler::showSplashScreen() {
    // Draw Splash Screen w/ IO Rodeo logo.
    _lcd.drawBox(1,1,127,63,1);
    _lcd.print("PENDULUM EXPERIMENT",8,18);
    _lcd.print("IO RODEO INC.",28,28);
    delay(DISPLAY_SPLASH_TIMEOUT);
    _lcd.clearScreen();
}

void DisplayHandler::updatePeriodText(uint16_t period) {
    // Update period text on lcd display.
    char msg[20];
    int sec;
    int frac;
    if (period == 0) {
        // We don't have a valid period measurement, display empty values
        sprintf(msg,"PERIOD: _.___ S  ");
        _lcd.print(msg,2,3);
    }
    else {
        // Get seconds and fractional seconds
        sec = period/1000;
        frac = period - sec*1000;
        sprintf(msg,"PERIOD: %d.%03d S  ",sec,frac);
        _lcd.print(msg,2,3);
    }
}

void DisplayHandler::updateTracePlot(uint16_t value) {
    // Update the streaming plot trace.
    uint16_t displayValue;
    static uint16_t displayValueLast=0;

    // Adjust value for display
    displayValue = 55 - map(value,0,1024,0,35);

    if ((displayValue > 55) || (displayValue < 20)) {
        displayValue = displayValueLast;
    }
    
    // Clear old trace values
    if (_traceClearPos == 0) {
        _lcd.setPixel(_traceClearPos,_traceBuf[_traceClearPos],0);
    }
    else {
        _lcd.drawLine(
                _traceClearPos-1,
                _traceBuf[_traceClearPos-1],
                _traceClearPos,
                _traceBuf[_traceClearPos],
                0
                );
    }
    // Clear artifacts  - not sure what causes these
    _lcd.drawLine(_traceClearPos, 19, _traceClearPos, 56, 0); 

    // Plot new trace values.
    if (_tracePlotPos == 0) {
        _lcd.setPixel(_tracePlotPos,displayValue,1);
    }
    else {
        _lcd.drawLine(
                _tracePlotPos-1,
                displayValueLast,
                _tracePlotPos,
                displayValue,
                1
                );
    }

    // Save last value and add data to trace buffer.
    displayValueLast = displayValue;
    _traceBuf[_tracePlotPos] = displayValue;

    // Update trace plot position 
    _tracePlotPos += 1;
    if (_tracePlotPos > 127) {
        _tracePlotPos = 0;
    }

    // Update trace clear position
    _traceClearPos += 1;
    if (_traceClearPos > 127) {
        _traceClearPos = 0;
    }
}
