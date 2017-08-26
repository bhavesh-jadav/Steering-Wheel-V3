#include<Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.requestFrom(8, 3);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    byte c = Wire.read(); // receive a byte as character
    bin_prnt_byte(c);
  }
  Serial.println();
}

void bin_prnt_byte(byte x)
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
}
