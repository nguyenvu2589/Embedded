/* This minimal example shows how to get single-shot range
measurements from the VL6180X.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL6180X.h>

VL6180X sensor1;
VL6180X sensor2;
VL6180X sensor3;

int sensor1_pin = 17;
int sensor2_pin = 16;
int sensor3_pin = 15;

void setup() 
{
  Serial.begin(57600);
  Wire.begin();
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

// //////////////////////I2C scanner/////////////////////
//  Serial.begin (115200); // Start Serial Communication at baud rate 115200
//  while (!Serial)  
//    {
//    }
// 
//  Serial.println ();
//  Serial.println ("I2C Address Scanner. Scanning ...");
//  byte count = 0;
//  
//  Wire.begin();
//  for (byte i = 8; i < 120; i++)
//  {
//    Wire.beginTransmission (i);
//    if (Wire.endTransmission () == 0)
//      {
//      Serial.print ("Found i2c Device Address: ");
//      Serial.print (i, DEC);
//      Serial.print (" (0x");
//      Serial.print (i, HEX);
//      Serial.println (")");
//      count++;
//      delay (1);  
//      } // end of good response
//  } // end of for loop
//  Serial.println ("Done.");
//  Serial.print ("Found ");
//  Serial.print (count, DEC);
//  Serial.println (" device(s).");
}

int dist_1 = 0;
int dist_2 = 0;
int dist_3 = 0;

void loop() 
{ 
  
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.readRangeSingleMillimeters());
  delay(10);
  Serial.print("\t");
  
  Serial.print("Sensor 2: ");
  Serial.print(sensor2.readRangeSingleMillimeters());
  delay(10);
  Serial.print("\t");
  
  Serial.print("Sensor 3: ");
  Serial.print(sensor3.readRangeSingleMillimeters());
  delay(10);

  Serial.println();
}
