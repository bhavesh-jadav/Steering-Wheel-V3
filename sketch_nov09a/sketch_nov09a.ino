#include <Encoder.h>
Encoder steer(2, 3);
int steerpos, laststeerpos = 0, sl = -2550, sr = 2550;
int val;
void setup() {
  // put your setup code here, to run once:
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
  Serial.begin(9600);
  steerpos = map(steerpos, sl, sr, 0, 1023);
}

void calibrateSteeringWheel()
{
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

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = !digitalRead(4);
  if(val == 1)
    calibrateSteeringWheel();
  laststeerpos = steerpos;
  steerpos = steer.read();
  steerpos = map(steerpos, sl, sr, -32768, 32767);
  if(steerpos > 32767)
    steerpos = 32767;
  else if(steerpos < -32768)
    steerpos = -32738;
  /*int val = abs(laststeerpos - steerpos);
  if(abs(val) > 10)
  {
    steerpos = laststeerpos;
    Serial.println(val);
    Serial.println(steerpos);
    Serial.println();
  }*/
  Serial.println(steerpos);
}
