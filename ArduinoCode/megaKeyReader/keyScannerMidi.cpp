#include "megaKeyReader.h"

unsigned char ucPreviousKeyState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucDebouce1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucDebouce2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucDebouce3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucDebouce4[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char ucDebouce5[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void readManuals(){
  unsigned char ucSet = 0;
  unsigned char ucKeyStateDiff = 0;
  unsigned char ucDebouncedKeyState = 0;

  for(unsigned char c=0x80; (unsigned) c>0x08; c >>=1){
    PORTL = c; //set the scanner pin
    //PA7 = 29-> PA0 = 22
//    Serial.print("PORTA: ");
//    binaryCharPrint(PINA);
//    Serial.print(" PORTC: ");
//    binaryCharPrint(PINC);
//    Serial.print("|");
   // Serial.print("H");
    delay(1);
    //do pins 1-8
    ucDebouncedKeyState = debounce(PINC, ucSet);
    ucKeyStateDiff = ucDebouncedKeyState^ucPreviousKeyState[ucSet*2];

    if(ucKeyStateDiff!=0x00){ //if there is a change in key state
      parseLeft(ucKeyStateDiff, PINC, (36+(16*ucSet)), 12);
    }
    ucPreviousKeyState[ucSet*2] = PINC;
    

    //do pins 9-16
    ucDebouncedKeyState = debounce(PINA, ucSet+1);
    ucKeyStateDiff = ucDebouncedKeyState^ucPreviousKeyState[ucSet*2+1];
    if(ucKeyStateDiff!=0x00){ //if there is a change in key state
      parseRight(ucKeyStateDiff, PINA, (36+(16*ucSet+8)), 12);
    }
    ucPreviousKeyState[ucSet*2+1] = PINA;
    ucSet++;
    
  }

  //Actung, usese Both ucSet's.
  for(unsigned char c=0x08; c; c >>=1){
    PORTL = c; //set the scanner pin
    delay(1);
    //do pins 1-8
    ucDebouncedKeyState = debounce(PINC, ucSet);
    ucKeyStateDiff = ucDebouncedKeyState^ucPreviousKeyState[ucSet*2];

    if(ucKeyStateDiff!=0x00){ //if there is a change in key state
      parseLeft(ucKeyStateDiff, PINC, (36+(16*(ucSet%4))), 13);
    }
    ucPreviousKeyState[ucSet*2] = PINC;
    

    //do pins 9-16
    ucDebouncedKeyState = debounce(PINA, ucSet+1);
    ucKeyStateDiff = ucDebouncedKeyState^ucPreviousKeyState[ucSet*2+1];
    if(ucKeyStateDiff!=0x00){ //if there is a change in key state
      parseRight(ucKeyStateDiff, PINA, (36+(16*(ucSet%4)+8)), 13);
    }
    ucPreviousKeyState[ucSet*2+1] = PINA;
    ucSet++;
  }

  
 // Serial.println("");
}

unsigned char debounce(unsigned char currentbits, char number){
  unsigned char temp1, temp2, temp3, debounced=0;

   for (unsigned char mask = 0x01; mask; mask <<= 1){
    temp1 = (currentbits&mask)^(ucDebouce1[number]&mask);
    temp2 = (ucDebouce2[number]&mask)^(ucDebouce3[number]&mask);
    temp3 = (ucDebouce4[number]&mask)^(ucDebouce5[number]&mask);
    //check for equality
    if((temp1 | temp2 | temp3) == 0x00){
      debounced |= mask&currentbits;
    }

    

  ucDebouce1[number] = currentbits;
  ucDebouce2[number] = ucDebouce1[number]; //shift
  ucDebouce3[number] = ucDebouce2[number];
  ucDebouce4[number] = ucDebouce3[number];
  ucDebouce5[number] = ucDebouce4[number];

  
  }
return debounced;
}

void parseLeft(char changedBits, char thePort, char offset, char channel){
  unsigned char count = 0;
 for (unsigned char mask = 0x01; mask; mask <<= 1) {
       if(mask&changedBits?'1':'0' == 1){ //key is depressed, so do somthing
          //figure out what state to send out the MIDI port
          if(thePort&mask?'1':'0' == 1){ //if depressed
            midiCommandToQueue(channel, _NOTEON, (count+offset));
          }else{
            midiCommandToQueue(channel, _NOTEOFF, (count+offset));
          }
       }
       count++;
   }
}

void midiCommandToQueue(char channel, char cmd, char pitch){
    
    char chanelCommand = (cmd & 0xF0) | (channel & 0x0F);
    
    //push channel and command
    midiMessageBuffer.put(chanelCommand);
    //push pitch
    midiMessageBuffer.put(pitch);
    //push velocity
    midiMessageBuffer.put(_DEFAULTVELOCITY);
}


void parseRight(char changedBits, char thePort, char offset, char channel){
  unsigned char count = 0;
 for (unsigned char mask = 0x80; mask; mask >>= 1) {
       if(mask&changedBits?'1':'0' == 1){ //key is depressed, so do somthing
          //figure out what state to send out the MIDI port
          if(thePort&mask?'1':'0' == 1){ //if depressed
            midiCommandToQueue(channel, _NOTEON, (count+offset));
          }else{
            midiCommandToQueue(channel, _NOTEOFF, (count+offset));
          }
       }
       count++;
   }
}

void binaryCharPrint(char theNumber){
  //thanks http://forum.arduino.cc/index.php?topic=45679.0
  for (unsigned char mask = 0x80; mask; mask >>= 1) {
       Serial.print(mask&theNumber?'1':'0');
   } 

}

void binaryIntPrint(int theNumber){
  //thanks http://forum.arduino.cc/index.php?topic=45679.0
  for (unsigned int mask = 0x8000; mask; mask >>= 1) {
       Serial.print(mask&theNumber?'1':'0');
   }
  
}

