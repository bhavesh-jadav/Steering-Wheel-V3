#include<Wire.h>

byte pins[8] = {2, 3, 4, 5, 6, 7, 9, 10};
byte index;
byte values;
byte val;

void setup() {
  //Serial.begin(9600);
  Wire.begin(9);
  Wire.onRequest(sendDataToMaster);
  for(int i = 0; i < 8; i++)
  {
    pinMode(pins[i], INPUT);
    digitalWrite(pins[i], HIGH);
  }
}

void sendDataToMaster(){
  index = 0;
  values = 0;
  for(int i = 0; i < 8; i++)
  {
    val = !digitalRead(pins[i]);
    setButtonState(index, val);
    index++;
  }
  Wire.write(values);
}

void loop() {

  /*index = 0;
  values = 0;
  for(int i = 0; i < 8; i++)
  {
    val = !digitalRead(pins[i]);
    setButtonState(index, val);
    index++;
  }
  bin_prnt_byte(values);
  Serial.println();*/
}

void setButtonState(unsigned int index, boolean value) {
    values |= value << index;
}

/*void bin_prnt_byte(byte x)
{
   int n;
   for(n=0; n<8; n++)
   {
      if((x & 0x80) !=0)
      {
         Serial.print("1");
      }
      else
      {
         Serial.print("0");
      }
      x = x<<1;
   }
}*/
