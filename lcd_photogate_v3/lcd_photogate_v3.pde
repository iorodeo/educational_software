#include <NewSoftSerial.h>
#include "lcd_photogate.h"
#include "SerialLCD.h"
#include "Sensor.h"
#include "DisplayHandler.h"
#include "util/atomic.h"

NewSoftSerial softSerial(8,7);
DisplayHandler display(softSerial);
Sensor photogate;

void setup() {
    softSerial.begin(115200);
    delay(2500);
    display.init();
    display.showSplashScreen();
    timerInit();
}

void loop() {
    uint16_t period;
    uint16_t currValue;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        //currValue = photogate.getCurrValue();
        currValue = photogate.getMinValueInBuf();
        period = photogate.getPeriod();
    }

    // Update display
    display.setBrightness();
    display.updatePeriodText(period);
    display.updateTracePlot(currValue);
    delay(LOOP_DELAY);
}

ISR(TIMER2_OVF_vect) {
    // Timer 2 overflow interrupt service routine. Take photogate 
    // sensor readings.
    photogate.update();
}

void timerInit(void) {
    // Initialize timer 2 - 1kHz overflow for reading photogate 
    TCCR2A = TIMER_TCCR2A;
    TCCR2B = TIMER_TCCR2B;
    OCR2A = TIMER_OCR2A;
    // Timer 2 overflow interrupt enable 
    TIMSK2 |= (1<<TOIE2) | (0<<OCIE2A);
    TCNT2 = 0;
}

