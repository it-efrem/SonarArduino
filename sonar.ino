#include <Servo.h>

Servo servo;

const int SonarTrig = 2; 
const int SonarEcho = 3; 
const int SonarServo = 4;

const int delayOfMeasuring = 30; //ms

unsigned int time_us=0;
unsigned int distance_sm=0;

int ServoStartAngle = 0;
int ServoMaxAngle = 180;
int ServoAngle = 0;

void getDistance()
{
  //Получение растояния
    digitalWrite(SonarTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(SonarTrig, LOW);
    time_us=pulseIn(SonarEcho, HIGH);
    distance_sm=time_us/58;

  //Вывод данных в порт
    Serial.print(ServoAngle);
    Serial.print('=');
    Serial.print(distance_sm); 
    Serial.println(); 
}

void setup() 
{ 
  pinMode(SonarTrig, OUTPUT); 
  pinMode(SonarEcho, INPUT); 
  pinMode(SonarServo, OUTPUT);
  servo.attach(4);
    
  Serial.begin(9600); 
}

void loop() 
{ 
  //Вращение радара
    for (int i = 0; i <= ServoMaxAngle; i++)
    {
      ServoAngle = i;
      servo.write(ServoAngle);
      getDistance();
      delay(delayOfMeasuring);
    }
    for (int i = ServoMaxAngle; i >= 0; i--)
    {
      ServoAngle = i;
      servo.write(ServoAngle);
      getDistance();
      delay(delayOfMeasuring);
    } 
}
