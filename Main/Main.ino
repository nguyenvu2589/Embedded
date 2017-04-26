#include <Wire.h>
#include <VL6180X.h>
#include "I2Cdev.h"
#include "MPU6050.h"

//laser vars//
VL6180X sensor1;
VL6180X sensor2;
VL6180X sensor3;

int sensor1_pin = 17;
int sensor2_pin = 16;
int sensor3_pin = 15;

int laserActionThreshold = 150;
//End

//Motor Vars
int pinI1 = 12;  //define IN1 interface
int pinI2 = 13; //define IN2 interface 
int speedpinA = 14;  //enable motor A
int pinI3 = 25; //define IN3 interface 
int pinI4 = 26; //define IN4 interface 
int speedpinB = 27; //enable motor B
//end

//Accel vars
unsigned long current_time = 0;
float time_step = 0.0;
//end

//Consts
#define LEFT        0
#define RIGHT       2
#define FORWARD     1
#define STOP        3
#define STRAFERIGHT 5
#define STRAFELEFT  4

struct ScanData
{
  int laserLeft;
  int laserFront;
  int laserRight;
};

ScanData data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WriteMsg("Init Begin", true);
  Wire.begin();
  WriteMsg("Wire Loaded", true);
  MoveInit();
  WriteMsg("Motors Loaded", true);
  LaserInit();
  WriteMsg("Lasers Loaded", true);
}

void loop() {
  Scan();

  MoveDir(FORWARD);
//  if(millis() - current_time > 750)
//  {
    if(data.laserFront < 125)
    {
      delay(175);
      MoveDir(STOP);
      WriteMsg("Front Sensor Action", false);
      if(data.laserLeft > 500)
      {
        WriteMsg("Left", true);
        TurnDeadEnd(LEFT);
      }
      else if(data.laserRight > 500)
      {
        WriteMsg("Right", true);
        TurnDeadEnd(RIGHT);
      }
      else
      {
        WriteMsg("Reverse", true);
        TurnDeadEnd(LEFT);
      }
    }
    else if(data.laserLeft > 500)
    {
      delay(175);
      MoveDir(STOP);
      WriteMsg("Left Sensor Action", true);
      TurnIntersection(LEFT);
    }
    else if(data.laserRight > 500)
    {
      delay(175);
      MoveDir(STOP);
      WriteMsg("Right Sensor Action", true);
      TurnIntersection(RIGHT);    
    }
//  }
  CheckSide(LEFT, data.laserLeft);
  CheckSide(RIGHT, data.laserRight);
}

void TurnDeadEnd(int dir)
{
  delay(300);
  WriteMsg("DeadEnd Loop Start", true);
  while(true)
  {
    MoveDir(dir);
    delay(50);
    MoveDir(STOP);
    Scan();
    if(data.laserFront > 500)
    {
      WriteMsg("DeadEnd Scan Check", false);
      delay(10);
      if(data.laserFront > 500)
      {
        WriteMsg("DeadEnd Scan Confirmed", true);
        break;
      }
    }
  }
  WriteMsg("DeadEnd Turn Complete", true);
  MoveDir(dir);
  delay(50);
  MoveDir(FORWARD);
  delay(500);
  current_time = millis();
}

void TurnIntersection(int dir)
{
  delay(300);
  WriteMsg("Intersection Turn First Loop", true);
  while(true)
  {
    MoveDir(dir);
    delay(50);
    MoveDir(STOP);
    Scan();
    if(data.laserFront < 500)
    {
      delay(10);
      if(data.laserFront < 500)
      {
        break;
      }
    }
  }
  WriteMsg("Intersection Turn Second Loop", true);
  while(true)
  {
    MoveDir(dir);
    delay(50);
    MoveDir(STOP);
    Scan();
    if(data.laserFront > 500)
    {
      delay(10);
      if(data.laserFront > 500)
      {
        break;
      }
    }
  }
  WriteMsg("Intersection Turn Complete", true);
  MoveDir(dir);
  delay(50);
  MoveDir(FORWARD);
  delay(500);
  current_time = millis();
}

void MoveDir(int dir)
{
  switch (dir)
  {
    case LEFT://hard left
      //Move(-1,1);
      digitalWrite(pinI4,HIGH); //motor A clockwise
      digitalWrite(pinI3,LOW);  
      
      digitalWrite(pinI2,LOW); //motor B clockwise
      digitalWrite(pinI1,HIGH);
      break;
      
    case FORWARD://forward
      //Move(1,1);
      digitalWrite(pinI3,HIGH); //motor A counter-clockwise
      digitalWrite(pinI4,LOW);
      
      digitalWrite(pinI2,LOW); //motor B clockwise
      digitalWrite(pinI1,HIGH);
      break;
      
    case RIGHT://hard right
      //Move(1,-1);
      digitalWrite(pinI3,HIGH); //motor A counter-clockwise
      digitalWrite(pinI4,LOW);

      digitalWrite(pinI1,LOW); //motor B counter-clockwise
      digitalWrite(pinI2,HIGH);
      break;
      
    case STOP://stop
      //Move(0,0);
      digitalWrite(pinI4,LOW); //motor A stop
      digitalWrite(pinI3,LOW);

      digitalWrite(pinI2,LOW); //motor B stop
      digitalWrite(pinI1,LOW);
      break;
      
    case STRAFELEFT://strafe left
      //Move(0,1);
      digitalWrite(pinI4,LOW); //motor A stop
      digitalWrite(pinI3,LOW);

      digitalWrite(pinI2,LOW); //motor B clockwise
      digitalWrite(pinI1,HIGH);
      break;
      
    case STRAFERIGHT://strafe right
      //Move(1,0);
      digitalWrite(pinI3,HIGH); //motor A counter-clockwise
      digitalWrite(pinI4,LOW);

      digitalWrite(pinI2,LOW); //motor B stop
      digitalWrite(pinI1,LOW);
      break;
  }
}

void MoveInit()
{
  digitalWrite(speedpinA,HIGH);//input a simulation value to set the speed
  digitalWrite(speedpinB,HIGH);
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);
}

void LaserInit()
{
  pinMode(sensor1_pin,OUTPUT);
  pinMode(sensor2_pin,OUTPUT);
  pinMode(sensor3_pin,OUTPUT);

  digitalWrite(sensor1_pin, LOW);
  digitalWrite(sensor2_pin, LOW);
  digitalWrite(sensor3_pin, LOW);

  //Sensor 1
  digitalWrite(sensor1_pin, HIGH);
  delay(50);
  sensor1.init();
  sensor1.configureDefault();
  sensor1.setScaling(3);
  sensor1.setTimeout(500);
  sensor1.setAddress(0x30);
  
  //Sensor 2
  digitalWrite(sensor2_pin, HIGH);
  delay(50);
  sensor2.init();
  sensor2.configureDefault();
  sensor2.setScaling(3);
  sensor2.setTimeout(500);
  sensor2.setAddress(0x32);

  //Sensor 3
  digitalWrite(sensor3_pin, HIGH);
  delay(50);
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setScaling(3);
  sensor3.setTimeout(500);
  sensor3.setAddress(0x34);

  delay(1000);
}

void Scan()
{
  data.laserRight = sensor1.readRangeSingleMillimeters();

  data.laserFront = sensor2.readRangeSingleMillimeters();

  data.laserLeft = sensor3.readRangeSingleMillimeters();
  
////////////////Data scan Debuger////////////////
  WriteMsg("Scan", false);
  WriteInt(data.laserLeft, false);
  WriteInt(data.laserFront, false);
  WriteInt(data.laserRight, true);
}

bool CheckSide(int side, int dist)
{
  if(side == LEFT)
  {//left
    if(dist < laserActionThreshold)
    {
      WriteMsg("Check Left Action", true);
      MoveDir(STRAFERIGHT);
      delay(25);
      MoveDir(FORWARD);
    }
  }
  else
  {//right
    if(dist < laserActionThreshold)
    {
      WriteMsg("Check Right Action", true);
      MoveDir(STRAFELEFT);
      delay(25);
      MoveDir(FORWARD);
    }
  }
}

void WriteMsg(String msg, bool newLine)
{
  if(newLine)
  {
    Serial.print(msg);
    Serial.print("\n");
  }
  else
  {
    Serial.print(msg);
    Serial.print("\t");
  }
}

void WriteInt(int val, bool newLine)
{
  if(newLine)
  {
    Serial.print(val);
    Serial.print("\n");
  }
  else
  {
    Serial.print(val);
    Serial.print("\t");
  }
}
