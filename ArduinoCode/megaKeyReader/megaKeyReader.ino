#include "megaKeyReader.h"

ByteBuffer midiMessageBuffer;

void setup() {
//this is to see if Git works
  
/**
 * PORT A, C, and L are used for the manulas
 * PORT K, B and pins are used for the pedals
 */


  //PA7 = 29-> PA0 = 22
  DDRA = B00000000;//set as inputs
  //PC = 37 -> PC0 = 30
  DDRC = B00000000; //set as inputs
  DDRK = B11111111; //set as outputs
  //----------Pedals-----------
  DDRL = 0x00; 
  DDRB = 0x00;
  DDRG = 0x03;
  
  //midi is 31250
  //Serial.begin(115200);
  Serial.begin(31250);

  midiMessageBuffer.init(100);
}



void loop() {
  // put your main code here, to run repeatedly:
  readManuals();
  //testPedals();
  readPedals();
//  PORTL = 0x08;
//  binaryCharPrint(PINC);
//  binaryCharPrint(debounce(PINC, 1));
//  Serial.println("%");
//-----------------------
  while(midiMessageBuffer.getSize() >0){
    Serial.write(midiMessageBuffer.get());
  }
  
  delayMicroseconds(5);
  //delay(500);

}
