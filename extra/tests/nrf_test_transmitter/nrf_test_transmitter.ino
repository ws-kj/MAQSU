
//  MAQSU - Communication Test Code v0.1
//  Transmitter
//  By Will Savage

//  3, 10   - RF24
//  2       - DHT11

#include <SPI.h>

#include <nRF24L01.h>
#include <RF24.h>

#include <SimpleDHT.h>

RF24 radio(3, 10); // CE, CSN

SimpleDHT11 dht11(2); //DATA

const byte address[6] = "00001";

char data[64];

void setup() {
  //serial setup
  Serial.begin(9600);
  
  

  //radio setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  //all is well
  Serial.println("Transmitter Online");
}

void loop() {
  readDHT();
  
  //const char text[64] = "Transmission";
  //radio.write(&text, sizeof(text));
  
  radio.write(&data, sizeof(data));
  delay(1000);
}

//get temperature and humidity data from DHT11
void readDHT() {
  byte temp =  0;
  byte hum = 0;
  
  int err = SimpleDHTErrSuccess;
  if((err = dht11.read(&temp, &hum, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("DHT11 read failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  char result[64];
  sprintf(result, "Temp: %iC Hum: %iH", temp, hum);
  Serial.println(result);

  //delay for smapling rate
  delay(1500);

  strcpy(data, result);

}
