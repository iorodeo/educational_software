/* photogate_period
 
This program calculates the period of a pendulum passing through a photogate timer. 
Period is calculated from the time between every other high to low transition time. 
Measurements are returned to the Serial monitor in seconds.
Created : Sept. 2012 by IO Rodeo                                                                                             
                                                                                                             
*/


#include <Streaming.h>

#define LED_PIN 10
#define ANALOG_PIN 0
#define THRESHOLD 500
#define SENSOR_HIGH 0
#define SENSOR_LOW 1



void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN,OUTPUT);
}

void loop() {
    int sensorVal;
    
    unsigned long timeLow2High;
    unsigned long period;
    static int state;
    static bool isFirst = true;  
    static unsigned long timeHigh2Low[3];          // Create an array of 3 values to store timepoints.
    static unsigned int countHigh2Low = 0;
    static unsigned int countLow2High = 0;
    
    sensorVal = analogRead(ANALOG_PIN);             // Voltage measurements from the photogate 
    
    // Initialize and assign sensor state. Runs only once, when the program is first run. 
    if (isFirst) {
      if (sensorVal > THRESHOLD) {
        state = SENSOR_HIGH;
      }
      
      else {
        state = SENSOR_LOW;
      }
      timeHigh2Low[0] = micros();
      isFirst = false;
    }


    // Take action based on sensor state.
    if (state == SENSOR_HIGH) {                 
     
      if (sensorVal <= THRESHOLD) {                 // sensor is in high but value is below threshold ! Object is moving into beam.
        state = SENSOR_LOW;                         // switch states
        timeHigh2Low[countHigh2Low] = micros();     //  take time measurement
        countHigh2Low = ++ countHigh2Low;           // increment count by one
      
      }
    }
    
    if (state == SENSOR_LOW) {                 
     
      if (sensorVal >= THRESHOLD) {                 // sensor is in low but value is above threshold ! Object is moving into beam.
        state = SENSOR_HIGH;                        // switch states
       
      }
    }
    
    
    // Calculate period from the counts
    
    if (countHigh2Low == 3) {
      countHigh2Low = 0;
      period = timeHigh2Low[2] - timeHigh2Low[0];
      Serial << "period: " << float(period)/1000000.0 << " seconds" << endl;       // Print period to Serial Monitor in seconds.
      
    }
     
  }
    
    
