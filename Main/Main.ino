#include <Wire.h>
#include <VL6180X.h>
#include "I2Cdev.h"
#include "MPU6050.h"

//laser vars//
VL6180X sensor1;
VL6180X sensor2;
VL6180X sensor3;

int sensor1_pin = 15;
int sensor2_pin = 16;
int sensor3_pin = 17;

int laserActionThreshhold = 150;
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
MPU6050 accelgyro;
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
  int accX, accY, accZ, gyrX, gyrY, gyrZ;
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  MoveInit();
  LaserInit();
  accelgyro.initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
//  MoveDir(0);
//  delay(2000);
//  MoveDir(1);
//  delay(2000);
//  MoveDir(2);
//  delay(2000);
//  MoveDir(3);
//  delay(2000);
  ScanData temp = Scan();
  Serial.print(temp.accX);
  Serial.print("\t");
  Serial.print(temp.accY);
  Serial.print("\t");
  Serial.print(temp.accZ);
  Serial.print("\t");
  Serial.print(temp.gyrX);
  Serial.print("\t");
  Serial.print(temp.gyrY);
  Serial.print("\t");
  Serial.print(temp.gyrZ);
  Serial.print("\t");
  Serial.print(temp.laserLeft);
  Serial.print("\t");
  Serial.print(temp.laserFront);
  Serial.print("\t");
  Serial.print(temp.laserRight);
  Serial.print("\n");
}

void MoveDir(int dir)
{
  switch (dir)
  {
    case 0://hard left
      Move(-1,1);
      break;
    case 1://forward
      Move(1,1);
      break;
    case 2://hard right
      Move(1,-1);
      break;
    case 3://stop
      Move(0,0);
      break;
    case 4://strafe left
      Move(0,1);
      break;
    case 5://strafe right
      Move(1,0);
      break;
  }
}

//MotorA is left, MotorB is right
//1 = forward, 0 = stopped, -1 = reverse
void Move(int motorA, int motorB)
{
  if(motorA == -1)
  {//CLockwise
    digitalWrite(pinI4,HIGH);
    digitalWrite(pinI3,LOW);
  }
  else if(motorA == 0)
  {//No Movement
    digitalWrite(pinI4,LOW);//turn DC Motor B move clockwise
    digitalWrite(pinI3,LOW);
  }
  else
  {//Counter/AntiClockwise
    digitalWrite(pinI3,HIGH);//turn DC Motor B move clockwise
    digitalWrite(pinI4,LOW);
  }
  if(motorB == -1)
  {//COunter/AntiClockwise
    digitalWrite(pinI1,LOW);//turn DC Motor A move clockwise
    digitalWrite(pinI2,HIGH);
  }
  else if(motorB == 0)
  {//Not moving
    digitalWrite(pinI2,LOW);//turn DC Motor A move clockwise
    digitalWrite(pinI1,LOW);
  }
  else
  {//CLockwise
    digitalWrite(pinI2,LOW);//turn DC Motor A move clockwise
    digitalWrite(pinI1,HIGH);
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
  sensor1.setTimeout(500);
  sensor1.setAddress(0x30);
  
  //Sensor 2
  digitalWrite(sensor2_pin, HIGH);
  delay(50);
  sensor2.init();
  sensor2.configureDefault();
  sensor2.setTimeout(500);
  sensor2.setAddress(0x32);

  //Sensor 3
  digitalWrite(sensor3_pin, HIGH);
  delay(50);
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setTimeout(500);
  sensor3.setAddress(0x34);

  delay(1000);
}

ScanData Scan()
{
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  ScanData returnData;
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  returnData.accX = ax;
  returnData.accY = ay;
  returnData.accZ = az;
  returnData.gyrX = gx;
  returnData.gyrY = gy;
  returnData.gyrZ = gz;

  returnData.laserRight = sensor1.readRangeSingleMillimeters();
  delay(20);
  returnData.laserFront = sensor2.readRangeSingleMillimeters();
  delay(20);
  returnData.laserLeft = sensor3.readRangeSingleMillimeters();

  return returnData;
}

bool CheckSide(int side, int dist)
{
  if(side == 0)
  {//left
    if(dist < laserActionThreshhold)
    {
      MoveDir(STRAFERIGHT);
      delay(100);
      MoveDir(FORWARD);
    }
  }
  else
  {//right
    if(dist < laserActionThreshhold)
    {
      MoveDir(STRAFELEFT);
      delay(100);
      MoveDir(FORWARD);
    }
  }
}

