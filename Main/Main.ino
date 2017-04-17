#define pinI1     12  //define IN1 interface
#define pinI2     13 //define IN2 interface 
#define speedpinA 14  //enable motor A
#define pinI3     25 //define IN3 interface 
#define pinI4     26 //define IN4 interface 
#define speedpinB 27 //enable motor B

//struct ScanData
//{
//  public int laserLeft;
//  public int laserForward;
//  public int laserRight;
//  //public int accX, accY, accZ, gyrX, gyrY, gyrZ;
//};


void setup() {
  // put your setup code here, to run once:
  MoveInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  MoveDir(0);
  delay(2000);
  MoveDir(1);
  delay(2000);
  MoveDir(2);
  delay(2000);
  MoveDir(3);
  delay(2000);
}

void MoveDir(int dir)
{
  switch (dir)
  {
    case 0://left
      Move(-1,1);
    break;
    case 1://forward
      Move(1,1);
    break;
    case 2://right
      Move(1,-1);
    break;
    case 3://stop
      Move(0,0);
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

void Scan()
{
  
}

