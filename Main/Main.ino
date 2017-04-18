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
int16_t gz;
unsigned long prev_time = 0;
unsigned long current_time = 0;
float time_step = 0.0;
float angle_change = 0;
float total_angle_change = 0;
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
  Wire.begin();
  MoveInit();
  LaserInit();
  accelgyro.initialize();
}

void loop() {
//Read Data
  data.laserRight = sensor1.readRangeSingleMillimeters();
  delay(20);
  
  data.laserFront = sensor2.readRangeSingleMillimeters();
  delay(20);
  
  data.laserLeft = sensor3.readRangeSingleMillimeters();
  delay(20);
  
//////////////////Data scan Debuger////////////////
//  Serial.print("Start Scan: \t");
//  Serial.print(data.laserLeft);
//  Serial.print("\t");
//  Serial.print(data.laserFront);
//  Serial.print("\t");
//  Serial.print(data.laserRight);
//  Serial.print("\n");

  //MoveDir(FORWARD);
  digitalWrite(pinI3,HIGH); //motor A counter-clockwise
  digitalWrite(pinI4,LOW);
  digitalWrite(pinI2,LOW); //motor B clockwise
  digitalWrite(pinI1,HIGH);
 
  if(data.laserFront < laserActionThreshhold)
  {
    //MoveDir(STOP);
    digitalWrite(pinI4,LOW); //motor A stop
    digitalWrite(pinI3,LOW);
    digitalWrite(pinI2,LOW); //motor B stop
    digitalWrite(pinI1,LOW);
    delay(1000);
    
    total_angle_change = 0;
    time_step = 0;
    current_time = 0;
    prev_time = 0;

    while(total_angle_change < 90){  
      //Turn right
      digitalWrite(pinI3,HIGH); //motor A counter-clockwise
      digitalWrite(pinI4,LOW);
      digitalWrite(pinI1,LOW); //motor B counter-clockwise
      digitalWrite(pinI2,HIGH);
      
      current_time = millis();
      time_step = (current_time - prev_time) / 1000.0;
      prev_time = current_time;
      gz= accelgyro.getRotationZ();
      
      gz = gz/131;   //131 is a gyro scale based on datasheet
      
      angle_change = abs(gz * time_step);
      total_angle_change += angle_change;
    }
  }

  if(data.laserLeft < 200)
  {
    //StrafeLEFT
    digitalWrite(pinI3,HIGH); //motor A counter-clockwise
    digitalWrite(pinI4,LOW);
    digitalWrite(pinI2,LOW); //motor B stop
    digitalWrite(pinI1,LOW);
    
    delay(50);

    //Move forward
    digitalWrite(pinI3,HIGH); //motor A counter-clockwise
    digitalWrite(pinI4,LOW);
    digitalWrite(pinI2,LOW); //motor B clockwise
    digitalWrite(pinI1,HIGH);
  }
 
  if(data.laserRight < 200)
  {
    digitalWrite(pinI4,LOW); //motor A stop
    digitalWrite(pinI3,LOW);
    digitalWrite(pinI2,LOW); //motor B clockwise
    digitalWrite(pinI1,HIGH);
    
    delay(50);
    
    //Move forward
    digitalWrite(pinI3,HIGH); //motor A counter-clockwise
    digitalWrite(pinI4,LOW);
    digitalWrite(pinI2,LOW); //motor B clockwise
    digitalWrite(pinI1,HIGH);
  }
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
  sensor1.setScaling(2);
  sensor1.setTimeout(500);
  sensor1.setAddress(0x30);
  
  //Sensor 2
  digitalWrite(sensor2_pin, HIGH);
  delay(50);
  sensor2.init();
  sensor2.configureDefault();
  sensor2.setScaling(2);
  sensor2.setTimeout(500);
  sensor2.setAddress(0x32);

  //Sensor 3
  digitalWrite(sensor3_pin, HIGH);
  delay(50);
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setScaling(2);
  sensor3.setTimeout(500);
  sensor3.setAddress(0x34);

  delay(1000);
}
