/* photogate_velocity
 
This program calculates the velocity of an object passing through a photogate timer. 
Velocity is calculated from the total time the object blocks the sensor and the objects width.
Measurements are returned to the Serial Monitor in 
Created : Sept. 20102 by IO Rodeo                                                                                             
                                                                                                               
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
    int pendulumWidth = 16;                         // Width of the pendulum in millimeters.
    
    unsigned long timeLow2High;
    unsigned long dt;
    static int state;
    static bool isFirst = true;  
    static unsigned long timeHigh2Low; 
    
    sensorVal = analogRead(ANALOG_PIN);             // Voltage measurements from the photogate 
    
    // Initialize and assign sensor state. Runs only once, when the program is first run. 
    if (isFirst) {
      if (sensorVal > THRESHOLD) {
        state = SENSOR_HIGH;
      }
      
      else {
        state = SENSOR_LOW;
      }
      timeHigh2Low = micros();
      isFirst = false;
    }

    // Take action based on sensor state.
    if (state == SENSOR_HIGH) {                 
     
      if (sensorVal <= THRESHOLD) {                 // sensor is in high but value is below threshold ! Object is moving into beam.
        state = SENSOR_LOW;                         // switch states
        timeHigh2Low = micros();                    //  take time measurement
//        Serial << "timeHigh2Low " << _DEC(timeHigh2Low) << endl;
      
      }
    }
    
    else {
      if (sensorVal > THRESHOLD) {                  // sensor is in low state but value is higher than thrshold ! Object is moved out of the beam.
        state = SENSOR_HIGH;                        // switch states
        timeLow2High = micros();                    // take time measurement
  
  
        dt = (timeLow2High - timeHigh2Low);           // calculate the time the sensor was blocked
        
     
        
        Serial << " Velocity: " << (float(pendulumWidth)/dt)*1000.0 << " m/s" << '\n'; // calculate velocity, convert to m/s and print to Serial Monitor
        
      }
    }
    
  }
    
    
