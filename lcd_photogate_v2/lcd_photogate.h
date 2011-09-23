#ifndef _LCD_PHOTOGATE_H_
#define _LCD_PHOTOGATE_H_

#define LOOP_DELAY 25

// Timer parameters - 1kHz overflow on timer 2
#define TIMER_TCCR2A 0b00000011 
#define TIMER_TCCR2B 0b00001100  
#define TIMER_OCR2A 249 

#endif
