// MAQSU - MQ Sensor Test Code v0.1

#include <MQUnifiedsensor.h>

MQUnifiedsensor MQ135(A2, 135);
MQUnifiedsensor MQ7(A1, 7);
int mq5 = 2;

bool calibrated = false;

float CO2, CO;

void setup() {
  Serial.begin(9600);
  MQ135.inicializar();
  MQ7.inicializar();
}

void loop() {
  MQ135.update();
  MQ7.update();
  //MQ135.setRL(1);
  
  if(!calibrated) {
    //MQ135.setR0(MQ135.calibrate());
    //MQ7.setR0(MQ7.calibrate());
    calibrated = true;
  }

  readSensors();
  writeData();

  delay(5000);

}

void readSensors() {
  CO2 =  MQ135.readSensor("CO2"); 
  //CO2 = CO2*100.0;

  CO =  MQ7.readSensor("CO");

}

void writeData() {
  Serial.println("--------------------------------------");
  Serial.println("MQ-135 Data");
  Serial.print("Voltage: ");Serial.print(MQ135.getVoltage(false));Serial.println("V");
  Serial.print("R0: ");Serial.print(MQ135.getR0());Serial.println(" Ohm");
  Serial.print("CO2: ");Serial.print(CO2,2);Serial.println(" ppm");
  Serial.println();
  Serial.println("MQ-7 Data");
  Serial.print("Voltage: ");Serial.print(MQ7.getVoltage(false));Serial.println(" V"); 
  Serial.print("R0: ");Serial.print(MQ7.getR0());Serial.println(" Ohm"); 
  Serial.print("CO: ");Serial.print(CO,2);Serial.println(" ppm");
  Serial.println("--------------------------------------");
}
