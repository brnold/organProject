#include "megaKeyReader.h"

ByteBuffer midiMessageBuffer;

void setup() {
  // put your setup code here, to run once:
  //PA7 = 29-> PA0 = 22
  DDRA = B00000000;//set as inputs
  //PC = 37 -> PC0 = 30
  DDRC = B00000000;
  DDRL = B11111111;
  //midi is 31250
  //Serial.begin(115200);
  Serial.begin(31250);

  midiMessageBuffer.init(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  readManuals();
//  PORTL = 0x08;
//  binaryCharPrint(PINC);
//  binaryCharPrint(debounce(PINC, 1));
//  Serial.println("%");
  while(midiMessageBuffer.getSize() >0){
    Serial.write(midiMessageBuffer.get());
  }
  
  delayMicroseconds(5);
  //delay(500);

}
