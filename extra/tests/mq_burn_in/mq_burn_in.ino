//  MAQSU - MQ Sensor Burn-In Code
//  By Will Savage
//120
//200
//240


int mq135 = A2;
int mq7   = A1;
int mq5   = A0;

float mq135val = 0.0;
float mq7val   = 0.0;
float mq5val   = 0.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  mq135val = 0.0;
  mq7val   = 0.0;
  mq5val   = 0.0;

  for(int i=0; i<100; i++) {
    mq135val += analogRead(mq135);
    mq7val   += analogRead(mq7);
    mq5val   += analogRead(mq5);
  } 
  
  mq135val = mq135val / 100.0;
  mq7val   = mq7val   / 100.0;
  mq5val   = mq5val   / 100.0;

  Serial.print("MQ135: ");Serial.println(mq135val);
  Serial.print("MQ7: ");Serial.println(mq7val);
  Serial.print("MQ5: ");Serial.println(mq5val);

  delay(2000);
}
