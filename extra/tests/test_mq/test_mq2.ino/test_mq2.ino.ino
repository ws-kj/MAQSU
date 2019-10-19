#include <MQ135.h>

MQ135 gasSensor = MQ135(A2);  
int sensorPin = A0; 
int sensorValue = 0; 
void setup() { 
  Serial.begin(9600);
  pinMode(sensorPin, INPUT); 
} 
 
void loop() { 
  float val = 0.0;
  for(int i=0; i<100; i++) {
    val += analogRead(A0); 
  }
  val = val / 100.0;
  
  Serial.print ("raw = "); 
  Serial.println (val); 
  
  delay(1000);
}
