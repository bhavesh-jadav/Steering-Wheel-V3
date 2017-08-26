#include<Wire.h>

byte val;

byte keypadMat[4][4] = {0};
byte row[4] = {2, 3, 4, 5};
byte col[4] = {6, 7, 8, 17};
byte buttonValues[3] = {0};
int index;

void setup() {
  Wire.begin(8);
  Wire.onRequest(sendDataToMaster);
  for(int i = 9; i <= 16; i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }

  for(int i = 0; i < 4; i++)
  {
    pinMode(col[i], OUTPUT);
    pinMode(row[i], INPUT);
    digitalWrite(col[i], HIGH);
    digitalWrite(row[i], HIGH);
  }
}

void sendDataToMaster()
{
  for(int i = 0; i < 3; i++)
    buttonValues[i] = 0;
  
  index = 0;
  for(int i = 9; i <= 16; i++)
  {
    val = !digitalRead(i);
    setButtonState(index, val);
    index++;
  }

  for(int i = 0; i < 4; i++)
  {
    digitalWrite(col[i], LOW);
    for(int j = 0; j < 4; j++)
    {
      val = !digitalRead(row[j]);
      if(val == 1)
        keypadMat[j][i] = 1;
      else
        keypadMat[j][i] = 0;
    }
    digitalWrite(col[i], HIGH);  
  }

  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
       setButtonState(index, keypadMat[i][j]);
       index++;
    }
  }
  for(int i = 0; i < 3; i++)
    Wire.write(buttonValues[i]);
}

void setButtonState(unsigned int index, boolean value) {
    buttonValues[index/8] |= value << (index%8);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
