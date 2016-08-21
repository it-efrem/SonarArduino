#include <Servo.h>

Servo servo;

const int sonarServo = 2;

const int sonar1trig = 3; 
const int sonar1echo = 4; 

const int sonar2trig = 5; 
const int sonar2echo = 6; 

const int delayOfMeasuring = 50; //ms

int servoStartAngle = 0;
int servoMaxAngle = 180;
int servoAngle = 0;

int GetDistanceSm(int trig, int echo)
{
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int time_us = pulseIn(echo, HIGH);
  return time_us / 58;
}

void SendtData()
{
  int dist1 = GetDistanceSm(sonar1trig, sonar1echo);
  //int dist2 = GetDistanceSm(sonar2trig, sonar2echo);/
  
  //Передаем информацию 4-мя байтами
    Serial.print(char(int(servoAngle-128)));
    Serial.print(char(int(dist1 - 128)));
    Serial.print(char(int(255/*dist2*/ - 128)));
    Serial.print(char(int(servoAngle + dist1 + 255/*dist2*/ - 128)));
}

void setup() 
{ 
  pinMode(sonarServo, OUTPUT);
  servo.attach(sonarServo);
  
  pinMode(sonar1trig, OUTPUT); 
  pinMode(sonar1echo, INPUT); 

  pinMode(sonar2trig, OUTPUT); 
  pinMode(sonar2echo, INPUT); 
    
  Serial.begin(9600); 
}

void loop() 
{ 
  //Вращение радара
    for (int i = 0; i <= servoMaxAngle; i++)
    {
      servoAngle = i;
      servo.write(servoAngle);
      SendtData();
      delay(delayOfMeasuring);
    }
    for (int i = servoMaxAngle; i >= 0; i--)
    {
      servoAngle = i;
      servo.write(servoAngle);
      SendtData();
      delay(delayOfMeasuring);
    } 
}
