
#define _NOTEON 0x90
#define _NOTEOFF 0x80
#define _DEFAULTVELOCITY 0x69

void readManuals(void);
void parseLeft(char changedBits, char thePort, char offset, char channel);
void midiCommandToQueue(char channel, char cmd, char pitch);
void parseRight(char changedBits, char thePort, char offset, char channel);
void binaryCharPrint(char theNumber);
void binaryIntPrint(int theNumber);
unsigned char debounce(unsigned char currentbits, char number);
