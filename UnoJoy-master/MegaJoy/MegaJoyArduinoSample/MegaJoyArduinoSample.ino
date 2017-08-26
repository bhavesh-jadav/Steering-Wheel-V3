
#include "MegaJoy.h"
#include <Encoder.h>
#include <SmoothAnalogInput.h>
#include <EEPROM.h>
#include<Wire.h>

Encoder steer(2, 3);

SmoothAnalogInput acc;
SmoothAnalogInput brk;
SmoothAnalogInput clt;

int accmin, accmax;
int brkmin, brkmax;
int cltmin, cltmax;
long steerpos, laststeerpos, accpos, brkpos, cltpos;
int calreq, index;
int sl, sr; //variables for storing left and right position of steering wheel

//For testing purpose
//uint64_t s, e; 

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
  
  digitalWrite(13, LOW);
}

void calibrateSteeringWheel()
{
  int val;
  steer.write(0);
  
  delay(1000);
  val = !digitalRead(4);
  while(val != 1)
    val = !digitalRead(4);
  sl = steer.read();
  
  delay(1000);
  val = !digitalRead(4);
  while(val != 1)
    val = !digitalRead(4);
  sr = steer.read();

  EEPROMWriteInt(12, sl);
  EEPROMWriteInt(14, sr);
  
  delay(1000);
}

void setup()
{
  //Serial.begin(9600);
  Wire.begin();
  setupMegaJoy();
  pinMode(13, OUTPUT);
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
  acc.attach(A0);
  brk.attach(A1);
  clt.attach(A2);

  delay(100);
  calreq = !digitalRead(4);
  if(calreq == 1)
    calibratePadals();

  accmin = EEPROMReadInt(0);
  accmax = EEPROMReadInt(6);
  brkmin = EEPROMReadInt(2);
  brkmax = EEPROMReadInt(8);
  cltmin = EEPROMReadInt(4);
  cltmax = EEPROMReadInt(10);

  sl = EEPROMReadInt(12);
  sr = EEPROMReadInt(14);
  steerpos = map(steerpos, sl, sr, 0, 1023);
}

void loop(){
  // Always be getting fresh data
  //s = micros();
  
  calreq = !digitalRead(4);
  if(calreq == 1)
    calibrateSteeringWheel();
    
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData); 

  /*e = micros();
  unsigned long v = e - s;
  Serial.println(v);*/
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
}

megaJoyControllerData_t getControllerData(void)
{
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();

  laststeerpos = steerpos;
  steerpos = steer.read();
  steerpos = map(steerpos, sl, sr, 0, 65535);
  if(steerpos > 65535)
    steerpos = 65535;
  else if(steerpos < 0)
    steerpos = 0;
  /*int val = abs(laststeerpos - steerpos);
  if(abs(val) > 5)
    steerpos = laststeerpos;*/
  controllerData.analogAxisArray[9] = (uint16_t)steerpos;

  //accpos = acc.read();
  accpos = analogRead(A0);
  accpos = map(accpos, accmin, accmax, 0, 65535);
  if(accpos < 20)
    accpos = 0;
  else if(accpos > 65535)
    accpos = 65535;
  controllerData.analogAxisArray[6] = (uint16_t)accpos;
  
  //brkpos = brk.read();
  brkpos = analogRead(A1);
  brkpos = map(brkpos, brkmin, brkmax, 0, 65535);
  if(brkpos < 20)
    brkpos = 0;
  else if(brkpos > 65535)
    brkpos = 65535;
  controllerData.analogAxisArray[7] = (uint16_t)brkpos;

  //cltpos = clt.read();
  cltpos = analogRead(A2);
  cltpos = map(cltpos, cltmin, cltmax, 0, 65535);
  if(cltpos < 20)
    cltpos = 0;
  else if(cltpos > 65535)
    cltpos = 65535; 
  controllerData.analogAxisArray[8] = (uint16_t)cltpos;
  
  index = 4;
  Wire.requestFrom(8, 3);
  while (Wire.available()) { 
    byte b = Wire.read(); 
    controllerData.buttonArray[index] = b;
    index++;
  }
  Wire.requestFrom(9, 1);
   while (Wire.available()) { 
    byte b = Wire.read(); 
    controllerData.buttonArray[7] = b;
  }
  
  /*controllerData.analogAxisArray[4] = val;
  controllerData.analogAxisArray[5] = val;
  controllerData.analogAxisArray[6] = val;
  controllerData.analogAxisArray[7] = val;
  controllerData.analogAxisArray[8] = val;
  controllerData.analogAxisArray[9] = val;
  controllerData.analogAxisArray[10] = val;
  controllerData.analogAxisArray[11] = val;
  */
  // And return the data!
  return controllerData;
}
