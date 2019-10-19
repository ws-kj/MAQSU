
//  MAQSU - Communication Test Code v0.1
//  Reciever
//  By Will Savage

    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    
    RF24 radio(2, 10); // CE, CSN
    
    const byte address[6] = "00001";
    
    void setup() {
      Serial.begin(9600);
      Serial.println("Reciever Online");
      radio.begin();
      radio.openReadingPipe(0, address);
      radio.setPALevel(RF24_PA_LOW);
      radio.startListening();
      
    }
    void loop() {
      if (radio.available()) {
        delay(1000);
        char text[64];
        radio.read(&text, sizeof(text));
        Serial.println(text);
      }
    }
