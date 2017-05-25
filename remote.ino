#include "config.h";
int CHA = 0;
int CHB = 0;
int left_w = 0;
int right_w = 0;


int out1 = 0;
int out2 = 0;

float currentValue1 = 0.0;
float currentValue2 = 0.0;

unsigned long previousLoopTime = 0;
unsigned long loopTime = 0;
double Deadband(double value, double limit);

void setup() {
  //////////////////output//////////////////////
  pinMode(inApin_1, OUTPUT);
  pinMode(inApin_2, OUTPUT);
  pinMode(inBpin_1, OUTPUT);
  pinMode(inBpin_2, OUTPUT);

  pinMode(pwmApin, OUTPUT);
  pinMode(pwmBpin, OUTPUT);
  ///////////////////input///////////////////////

  pinMode(ch1pin, INPUT);
  pinMode(ch2pin, INPUT);


//  pinMode(ch3pin, INPUT);
//  pinMode(ch4pin, INPUT);
//  pinMode(ch5pin, INPUT);
//  pinMode(ch6pin, INPUT);
//  pinMode(ch7pin, INPUT);
//  pinMode(ch8pin, INPUT);

  digitalWrite(inApin_1, LOW); digitalWrite(inApin_2, LOW);
  digitalWrite(inBpin_1, LOW); digitalWrite(inBpin_2, LOW);
  digitalWrite(pwmApin, LOW);  digitalWrite(pwmBpin, LOW);
  ///////////////////////////////////////////////////
  Serial.begin(19200);
}
void loop() {
  //CHA = 1500;
  //CHB = 1500;
  loopTime = micros() - previousLoopTime;

  if (loopTime >= 10000)
  {
   // Serial.println("xx");
    previousLoopTime = micros();

    CHA = pulseIn(ch1pin, HIGH) - default_pInChA;
    CHB = pulseIn(ch2pin, HIGH) - default_pInChB;
    //Serial.print(CHA); Serial.print(" "); Serial.println(CHB);
    
    CHA = Deadband(CHA, Pulse_noiseCh1);
    CHB = Deadband(CHB, Pulse_noiseCh2);
    left_w = -(CHA - CHB);
    right_w = -(CHA + CHB);
    
    Serial.print(CHA); Serial.print(" "); Serial.println(CHB);
    currentValue1 = analogRead(CS1) * 0.035; // Motor Driver 1
    currentValue2 = analogRead(CS2) * 0.035; // Motor Driver 2

    if (currentValue1 < currentLimit) out1 = OutputToMotor1(left_w);
    else out1 = OutputToMotor1(0);
    if (currentValue2 < currentLimit) out2 = OutputToMotor2(right_w);
    else out2 = OutputToMotor2(0);

    analogWrite(pwmApin, out1);
    analogWrite(pwmBpin, out2);
    
  } //end loop time //
}




///////////////////////////////////////////////////////////////////////
double Deadband(double value, double limit)
{
  double temp = 0.0;
  if (value >= limit) temp = value - limit;
  else if (value <= -limit) temp = value + limit;
  else temp = 0.0;
  return temp;
}
//motor 1
int OutputToMotor1(int value)
{
  int temp = 0;
  if (value >= 0)
  {
    // CW
    digitalWrite(inApin_1, LOW);
    digitalWrite(inBpin_1, HIGH);
    temp = map(value, 0, 500, 0, 255);
  } else {
    // CCW
    digitalWrite(inApin_1, HIGH);
    digitalWrite(inBpin_1, LOW);
    temp = map(-value, 0, 500, 0, 255);
  }
  return temp;
}
// Motor 2
int OutputToMotor2(int value)
{
  int temp = 0;
  if (value >= 0)
  {
    digitalWrite(inApin_2, LOW);
    digitalWrite(inBpin_2, HIGH);
    temp = map(value, 0, 500, 0, 255);
  } else {
    digitalWrite(inApin_2, HIGH);
    digitalWrite(inBpin_2, LOW);
    temp = map(-value, 0, 500, 0, 255);
  }
  return temp;
}
