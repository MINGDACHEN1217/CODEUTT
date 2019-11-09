#include <Arduino.h>
#include "PortIO.h"

void initPort(const int *pins,int dir, const int number) {
  int pindir;
  int i;
  for(i=0;i<number;i+=1)
  {
    pindir = (dir >> i) & 1;
    if (pindir == 1) {
      pinMode(pins[i], OUTPUT);
    }
    else {
      pinMode(pins[i], INPUT);
    }
  }
}

void writePort(const int *pins,int value, const int number) {
  int pinvalue;
  int i;
  for(i=0;i<number;i+=1)
  {
    pinvalue = (value>> i) & 1;
    digitalWrite(pins[i], pinvalue);
  }
}

int readPort(const int *pins, const int number) {
unsigned int pinvalue,portvalue=0;
for(int i=0;i<number;i+=1)
{
		pinvalue = digitalRead(pins[i]);
		portvalue |= (pinvalue & 1) << i;
	}
  return portvalue;
}
