#include <Wire.h>
#include <VL6180X.h>
#include "I2Cdev.h"


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
int mov;
//end

//Consts
int LEFT = 0;
int RIGHT = 2;
int FORWARD = 1;
int STOP = 3;
int STRAFERIGHT = 5;
int STRAFELEFT = 4;
int BACK = 99;

int arr_size = 0;
int arr[4];

//Variable for deadend detection
int north = 11;
int south = 22;
int east = 33;
int west = 44;
bool deadEndEast = false;
bool deadEndSouth = false;

struct ScanData
{
  int laserLeft;
  int laserFront;
  int laserRight;
};

ScanData data;


struct car1
{
    int E;  // 4
    int S;  //3
    int N;  //2
    int W;  //1
};

car1 car;
int j;

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
  

  car.S = FORWARD;
  car.E = LEFT;
  car.W = RIGHT;
  car.N = BACK;
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

void loop() {

    for (j = 0; j < 4 ; j++)
    {
      arr[j] = STOP;
    }
    arr_size = 0;
    
    //read in info
    Scan();
    
    // front sensor blocks
    // 4 case : 1 deadend // 2 turn left only // 3 turn right only // 4 turn left or right.
    //Front laser detect wall that is in 125mm range
    if(data.laserFront < 125)
    {
        MoveDir(STOP);
  
        WriteMsg("Front Sensor Action", false);
        //Check if the LEFT turn is available
        if(data.laserLeft > 400)
        {
            WriteMsg("Left", true);
            arr[arr_size] = LEFT;
            arr_size++;
        }

        //Check if the RIGHT turn is available
        if(data.laserRight > 400)
        {
            WriteMsg("Right", true);
            arr[arr_size] = RIGHT;
            arr_size++;
        }

        //Hit a deadend, turn around
        else
        {
            WriteMsg("Reverse", true);
            update_orientation(BACK);
            TurnDeadEnd(LEFT);

//            if(deadEndDirection() == east){
//              deadEndEast = true;
//            }
//            else if(deadEndDirection() == south){
//              deadEndSouth = true;
//            }

            //After turned, move forward for 500ms then STOP
            MoveDir(FORWARD);
            delay(500);
            MoveDir(STOP);

            //Scan again, update laser data
            Scan();
        }
    }

    //WHILE car is moving
    //Check if car is able to turn LEFT
    else if(data.laserLeft > 400)
    {
        delay(175);
        MoveDir(STOP);
        WriteMsg("Left Sensor Action", true);
        arr[arr_size] = LEFT;
        arr_size++; 
    }

    //WHILE car is moving
    //Check if car is able to turn RIGHT
    else if(data.laserRight > 400)
    {
        delay(175);
        MoveDir(STOP);
        WriteMsg("Right Sensor Action", true);
        arr[arr_size] = RIGHT;
        arr_size++;
    } 

    //Car will move FORWARD if nothing is detected
    arr[arr_size] = FORWARD;
    arr_size++;
    printArray();
    
    mov = move_decider();
    Serial.print("MOVE:");
    printDir(mov);

    if(mov == FORWARD){
      MoveDir(FORWARD);
      Serial.println("MOVE FORWARD");
    }
    else{
      update_orientation(mov);
      TurnIntersection(mov);
      Serial.println("TURNED");

      //After turned, move forward for 500ms then STOP
      MoveDir(FORWARD);
      delay(300);
      MoveDir(STOP);

      //Scan again, update laser data
      Scan();
    }

    //Check side wall
    CheckSide(LEFT, data.laserLeft);
    CheckSide(RIGHT, data.laserRight);
}

void print_current_orientation()
{
    Serial.print(car.E);
    Serial.print(" points to East \n");
    Serial.print(car.S);
    Serial.print(" points to South \n");
    Serial.print(car.N);
    Serial.print(" points to North \n");
    Serial.print(car.W);
    Serial.print(" points to West \n");

}

int update_orientation(int turn)
{
    if (car.E == turn)
    {
        car.E = FORWARD;
        car.S = RIGHT;
        car.W = BACK;
        car.N = LEFT;
        return 1;
    }
    
    if (car.S == turn)
    {
        car.E = LEFT;
        car.S = FORWARD;
        car.W = RIGHT;
        car.N = BACK;
        return 1;
    }
    if ( car.N == turn)
    {
        car.E = RIGHT;
        car.S = BACK;
        car.W = LEFT;
        car.N = FORWARD;
        return 1;
    }
    if ( car.W == turn)
    {
        car.E = BACK;
        car.S = LEFT;
        car.W = FORWARD;
        car.N = RIGHT;
        return 1;
    }
    if (turn == BACK)
    {
        int temp;
        car.E = temp;
        car.E = car.W;
        car.W = temp;
        
        car.S = temp;
        car.S = car.N;
        car.N = temp;
        return 1;
    }
    return 0;
}

//work on sort arr..
int k;
int move_decider()
{
    //Can the car move to the EAST...
    for(k=0; k < arr_size; k++){
      if(car.E == arr[k]){
        return arr[k];
      }
    }
    
    //Can the car move to the SOUTH...
    for(k=0; k < arr_size; k++){
      if(car.S == arr[k]){
        return arr[k];
      }
    }

    for(k=0; k<arr_size; k++){
      if(car.N == arr[k]){
        return arr[k];
      }
    }
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
}

void TurnIntersection(int dir)
{
  //Car stop for 300ms
  delay(300);
  
  WriteMsg("Intersection Turn First Loop", true);

  //Car keep make a turn in 50ms until Front Laser see everything is clear
  //that means, car has successfully make a turn
  while(true)
  {
    MoveDir(dir);
    delay(50);
    MoveDir(STOP);
    Scan();
    if(data.laserFront < 300)
    {
      break;
    }
  }
  
  WriteMsg("Intersection Turn Second Loop", true);
  while(true)
  {
    MoveDir(dir);
    delay(50);
    MoveDir(STOP);
    Scan();
    if(data.laserFront > 300)
    {
       break;
    }
  }
  
  WriteMsg("Intersection Turn Complete", true);
//  MoveDir(dir);
//  delay(50);
}

void MoveDir(int dir)
{
  switch (dir)
  {
    case 0://hard left
      //Move(-1,1);
      digitalWrite(pinI4,HIGH); //motor A clockwise
      digitalWrite(pinI3,LOW);  
      
      digitalWrite(pinI2,LOW); //motor B clockwise
      digitalWrite(pinI1,HIGH);
      break;
      
    case 1://forward
      //Move(1,1);
      digitalWrite(pinI3,HIGH); //motor A counter-clockwise
      digitalWrite(pinI4,LOW);
      
      digitalWrite(pinI2,LOW); //motor B clockwise
      digitalWrite(pinI1,HIGH);
      break;
      
    case 2://hard right
      //Move(1,-1);
      digitalWrite(pinI3,HIGH); //motor A counter-clockwise
      digitalWrite(pinI4,LOW);

      digitalWrite(pinI1,LOW); //motor B counter-clockwise
      digitalWrite(pinI2,HIGH);
      break;
      
    case 3://stop
      //Move(0,0);
      digitalWrite(pinI4,LOW); //motor A stop
      digitalWrite(pinI3,LOW);

      digitalWrite(pinI2,LOW); //motor B stop
      digitalWrite(pinI1,LOW);
      break;
      
    case 4://strafe left
      //Move(0,1);
      digitalWrite(pinI4,LOW); //motor A stop
      digitalWrite(pinI3,LOW);

      digitalWrite(pinI2,LOW); //motor B clockwise
      digitalWrite(pinI1,HIGH);
      break;
      
    case 5://strafe right
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
  sensor2.setAddress(0x31);

  //Sensor 3
  digitalWrite(sensor3_pin, HIGH);
  delay(50);
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setScaling(3);
  sensor3.setTimeout(500);
  sensor3.setAddress(0x32);

  delay(1000);
}

bool CheckSide(int side, int dist)
{
  if(side == LEFT)
  {
    if(dist < laserActionThreshold)
    {
      WriteMsg("STAFE RIGHT", true);
      MoveDir(RIGHT);
      delay(15);
      MoveDir(FORWARD);
    }
  }
  else
  {
    if(dist < laserActionThreshold)
    {
      WriteMsg("STAFE LEFT", true);
      MoveDir(LEFT);
      delay(15);
      MoveDir(FORWARD);
    }
  }


//  Scan();
//  if(data.laserLeft < laserActionThreshold){
//    while(data.laserLeft < data.laserRight){
//      MoveDir(STRAFERIGHT);
//      delay(25);
//      MoveDir(STOP);
//      Scan();
//    }
//  }
//
//  if(data.laserRight < laserActionThreshold){
//    while(data.laserRight < data.laserLeft){
//      MoveDir(STRAFELEFT);
//      delay(25);
//      MoveDir(STOP);
//      Scan();
//    }
//  } 
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

int i;
void printArray()
{
  for(i = 0; i < 4; i++){
    switch(arr[i]){
      case 0:
        Serial.print("LEFT");
        Serial.print(", ");
        break;
      case 1:
        Serial.print("FORWARD");
        Serial.print(", ");
        break;
      case 2:
        Serial.print("RIGHT");
        Serial.print(", ");
        break;
      case 3:
        Serial.print("STOP");
        Serial.print(", ");
        break;
      case 99:
        Serial.print("BACK");
        Serial.print(", ");
        break;
    }
  }
  Serial.println();
}

void printDir(int dir){
   switch(dir){
      case 0:
        Serial.print("LEFT");
        break;
      case 1:
        Serial.print("FORWARD");
        break;
      case 2:
        Serial.print("RIGHT");
        break;
      case 3:
        Serial.print("STOP");
        break;
      case 99:
        Serial.print("BACK");
        break;
    }
  Serial.println();
}

int deadEndDirection(){
  if(car.N == FORWARD){
    return north;
  }
  else if(car.S == FORWARD){
    return south;
  }
  else if(car.E == FORWARD){
    return east;
  }
  else{
    return west;
  }
}
