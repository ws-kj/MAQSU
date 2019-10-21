#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//pins
RF24 radio(4, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  float test[1];
  test[0] = 1.0;
  radio.write(&test, sizeof(test));
  delay(250);

}
