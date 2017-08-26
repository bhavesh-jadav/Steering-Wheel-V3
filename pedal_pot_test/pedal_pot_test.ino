
#include <SmoothAnalogInput.h>
#include <EEPROM.h>

SmoothAnalogInput acc;
SmoothAnalogInput brk;
SmoothAnalogInput clt;

int accmin, accmax;
int brkmin, brkmax;
int cltmin, cltmax;
long steerpos, laststeerpos, accpos, brkpos, cltpos;
int val;

void EEPROMWriteInt(int address, int value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, two);
  EEPROM.write(address + 1, one);
}

int EEPROMReadInt(int address)
{
  //Read the 4 bytes from the eeprom memory.
  int two = EEPROM.read(address);
  int one = EEPROM.read(address + 1);
  
  //Return the recomposed long by using bitshift.
  return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
}

void calibratePadals()
{
  Serial.println("Calibrating up side");
  digitalWrite(13, HIGH);
  int i = 0, sample = 30;
  delay(4000);
  
  while(i < sample)
  {
    accmin = acc.read();
    i++;
  }
  
  i = 0;
  while(i < sample)
  {
    brkmin = brk.read();
    i++;
  }
  
  i = 0;
  while(i < sample)
  {
    cltmin = clt.read();
    i++;
  }

  Serial.println("Calibrating low side");
  digitalWrite(13, LOW);
  for(int j = 0; j < 15; j++)
  {
    delay(100);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
  }
  digitalWrite(13, HIGH);
  delay(4000);
  i = 0;
  while(i < sample)
  {
    accmax = acc.read();
    i++;
  }
  
  i = 0;
  while(i < sample)
  {
    brkmax = brk.read();
    i++;
  }
  
  i = 0;
  while(i < sample)
  {
    cltmax = clt.read();
    i++;
  }

  accmin += 10;
  accmax -= 10;
  brkmin += 10;
  brkmax -= 10;
  cltmin += 10;
  cltmax -= 10;

  EEPROMWriteInt(0, accmin);
  EEPROMWriteInt(2, brkmin);
  EEPROMWriteInt(4, cltmin);
  EEPROMWriteInt(6, accmax);
  EEPROMWriteInt(8, brkmax);
  EEPROMWriteInt(10, cltmax);
  
  Serial.println("Done calibrating");
  

  digitalWrite(13, LOW);
}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  acc.attach(A0);
  brk.attach(A1);
  clt.attach(A2);

  delay(100);
  //calibratePadals();

  accmin = EEPROMReadInt(0);
  accmax = EEPROMReadInt(6);
  brkmin = EEPROMReadInt(2);
  brkmax = EEPROMReadInt(8);
  cltmin = EEPROMReadInt(4);
  cltmax = EEPROMReadInt(10);

  Serial.print(accmin);
  Serial.print("  ");
  Serial.print(brkmin);
  Serial.print("  ");
  Serial.print(cltmin);
  Serial.print("  ");
  Serial.print(accmax);
  Serial.print("  ");
  Serial.print(brkmax);
  Serial.print("  ");
  Serial.print(cltmax);
  Serial.println();
  delay(3000);
}

void loop() {
  accpos = acc.read();
  Serial.print(accpos);
  Serial.print("  ");
  accpos = map(accpos, accmin, accmax, 0, 65535);
  if(accpos < 3000)
    accpos = 0;
  else if(accpos > 62000)
    accpos = 65535;
  Serial.print(accpos);
  Serial.print("  ");

  brkpos = brk.read();
  Serial.print(brkpos);
  Serial.print("  ");
  brkpos = map(brkpos, brkmin, brkmax, 0, 65535);
  if(brkpos < 3000)
    brkpos = 0;
  else if(brkpos > 62000)
    brkpos = 65535;
  Serial.print(brkpos);
  Serial.print("  ");

  cltpos = clt.read();
  Serial.print(cltpos);
  Serial.print("  ");
  cltpos = map(cltpos, cltmin, cltmax, 0, 65535);
  if(cltpos < 3000)
    cltpos = 0;
  else if(cltpos > 62000)
    cltpos = 65535;
  Serial.print(cltpos);
  Serial.println();
}
