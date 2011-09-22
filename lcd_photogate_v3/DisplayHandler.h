#ifndef _DISPLAY_HANDLER_H_
#define _DISPLAY_HANDLER_H_
#include "WProgram.h"
#include "SerialLCD.h"

#define DISPLAY_POT_AIN A7
#define DISPLAY_SPLASH_TIMEOUT 2000 
#define DISPLAY_TRACE_BUF_LEN 128
#define DISPLAY_CLEAR_OFFSET 15

class DisplayHandler {
    private:
        SerialLCD _lcd;
        uint8_t _tracePlotPos;
        uint8_t _traceClearPos;
        uint16_t _traceBuf[DISPLAY_TRACE_BUF_LEN];
    public:
        DisplayHandler(Print &port);
        void init();
        void setBrightness();
        void showSplashScreen();
        void updatePeriodText(uint16_t period);
        void updateTracePlot(uint16_t value);
};

#endif
