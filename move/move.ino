#define pinI1     12  //define IN1 interface
#define pinI2     13 //define IN2 interface 
#define speedpinA 14  //enable motor A
#define pinI3     25 //define IN3 interface 
#define pinI4     26 //define IN4 interface 
#define speedpinB 27 //enable motor B
 
void right()
{
     digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwisen
     digitalWrite(pinI3,LOW);
     digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
     digitalWrite(pinI1,HIGH);
}
void left()
{
     digitalWrite(pinI4,LOW);//turn DC Motor B move anticlockwise
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI2,HIGH);//turn DC Motor A move clockwise
     digitalWrite(pinI1,LOW);
}
void backward()
{
     digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
     digitalWrite(pinI3,LOW);
     digitalWrite(pinI2,HIGH);//turn DC Motor A move clockwise
     digitalWrite(pinI1,LOW);
}
void forward()
{
     digitalWrite(pinI4,LOW);//turn DC Motor B move anticlockwise
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI2,LOW);//turn DC Motor A move clockwise
     digitalWrite(pinI1,HIGH);
}
 
void setup()
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
 
void loop()
{
//  left();
//  delay(2000);
  right();
  delay(2000);
//  forward();
//  delay(2000);
//  backward();
//  delay(2000); 
}
