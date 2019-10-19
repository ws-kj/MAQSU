
//  MAQSU - Reciever Device 
//  By Will Savage

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//pins
RF24 radio(2, 10); // CE, CSN

const byte address[6] = "00001";



void setup() {
  //serial setup
  Serial.begin(9600);
  
  //radio setup    
  //Serial.println("Reciever Online");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
}
void loop() {
  if (radio.available()) {
    float data[6];
    radio.read(&data, sizeof(data));

    for(int i = 0; i < 6; i++) {
      Serial.println(data[i]);
    }
    Serial.println();  
    
  }
}
