
//  MAQSU - Transmitter Device
//  By Will Savage

//  --- Pins ---
//  Ax = Analog Pin x | x = Digital Pin x
//  4   - RF24 (CE)
//  2   - DHT11
//  10  - RF24  (CSN)
//  A7  - MQ-5
//  A6  - MQ-7
//  A5  - MPL3115A2 (SCL)
//  A4  - MPL3115A2 (SDA)
//  A3  - MQ-135


//  --- Data Array ---
//  0 - tplus
//  1 - mq135
//  2 - mq7
//  3 - mq5
//  4 - temperature
//  5 - humidity
//  6 - logStr

//includes
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

//pins
int readyled = 3;
int errorled = 5;
RF24 radio(4, 10);    // CE, CSN
SimpleDHT11 dht11(2); 
int mq135 = A3;  
int mq7   = A7;
int mq5   = A6;
Adafruit_MPL3115A2 mpl = Adafruit_MPL3115A2();  //Uses I2C predetermined pins

//average MQ output for relative calculation
float mq135avg = 160.0;
float mq7avg   = 400.0; //not calibrated
float mq5avg   = 300.0; //not calibrated 

//relative MQ values;
float mq135rel;
float mq7rel;
float mq5rel;

//output values
float mq135val;
float mq7val;
float mq5val;
byte temp;
byte hum;
float alt;
String logStr;

//radio information
const byte address[6] = "00001";

//data array to send
float data[7];

//time since startup in seconds
int tplus = 0;

void setup() { 
  //led setup
  pinMode(readyled, OUTPUT);
  pinMode(errorled, OUTPUT);

  
  digitalWrite(readyled, LOW);
  digitalWrite(errorled, HIGH);
  
  //serial setup
  Serial.begin(9600);

  //wait for sensors to calibrate
  calibrate(true);
  
  //radio setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  //all is well
  Serial.println("Transmitter Online");
  digitalWrite(readyled, HIGH);
  digitalWrite(errorled, LOW);
}

void loop() {
  readMPL();
  readDHT();
  readMQ();

  writeToArray();

  radio.write(&data, sizeof(data));
  tplus += 2;
  delay(2000);
}

//calibrate mq sensors
void calibrate(bool dev) {
  //skip calibration if testing for development
  if(dev) {
    delay(6000);
    
    //get avg values
    for(int i=0; i<10000; i++) {
      mq135val += analogRead(mq135);
      mq7val   += analogRead(mq7);
      mq5val   += analogRead(mq5);
    }
  } else {
    for(int i=0; i<10000; i++) {
      //set program-wide average
      mq135avg = mq135val / 10000.0;
      mq7avg   = mq7val   / 10000.0;
      mq5avg   = mq5val   / 10000.0;
    }
    //wait to heat up
    delay(60000);
  
    //get avg values
    for(int i=0; i<10000; i++) {
      mq135val += analogRead(mq135);
      mq7val   += analogRead(mq7);
      mq5val   += analogRead(mq5);
    }
  
    //set program-wide average
    mq135avg = mq135val / 10000.0;
    mq7avg   = mq7val   / 10000.0;
    mq5avg   = mq5val   / 10000.0;
  }
}

//read altitude data from MPL
void readMPL() {
  if(!mpl.begin()) {
    Serial.println("Couldn't find MPL");
    return;
  }

  alt = mpl.getAltitude();
  Serial.println(alt);

  
}

//get voltage output from MQ sensors
void readMQ() {
  //reset values
  mq135val = 0.0;
  mq7val   = 0.0;
  mq5val   = 0.0;

  //get average reading
  for(int i=0; i<1000; i++) {
    mq135val += analogRead(mq135);
    mq7val   += analogRead(mq7);
    mq5val   += analogRead(mq5);
  } 

  //average
  mq135val = mq135val / 1000.0;
  mq7val   = mq7val   / 1000.0;
  mq5val   = mq5val   / 1000.0;

  //get relative values
  mq135rel = mq135val / mq135avg;
  mq7rel = mq7val / mq7avg;
  mq5rel = mq5val / mq5avg;
}

//get temperature and humidity data from DHT11
void readDHT() {
  //reset values
  byte _temp =  0;
  byte _hum = 0;

  //check for read error
  int err = SimpleDHTErrSuccess;
  if((err = dht11.read(&_temp, &_hum, NULL)) != SimpleDHTErrSuccess) {
    logStr = strcat("DHT11 read failed, err= ", + err);
    digitalWrite(errorled, HIGH);
    return;
  } else {
    digitalWrite(errorled, LOW);
  }

  //set higher scope variables to lower scope
  temp = (_temp * 9/5) + 32;
  hum = _hum;
}

//puts all data in array
void writeToArray() {
  data[0] = tplus;
  data[1] = mq135rel;
  data[2] = mq7rel;
  data[3] = mq5rel;
  data[4] = temp;
  data[5] = hum;
  data[6] = alt;

  for(int i=0; i<7; i++) {
    Serial.println(data[i]);
  }
  
}
