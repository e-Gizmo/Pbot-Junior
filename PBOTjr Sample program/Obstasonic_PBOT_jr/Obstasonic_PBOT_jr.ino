// e-Gizmo OBSTASONIC PBOT jr.
/*
This programs a sumobot with built in line
sensor to avoid lines and stay in a ring.
It also has an ultrasonic sensor that
tells the Pbot to ram the target but avoids
the ring if detected.

Codes by:
e-Gizmo Mechatronix Central
Taft, Manila, Philippines
http://www.e-gizmo.com
January 11, 2014
*/

#include <NewPing.h>

const int LINE_1 = 5;  //line sensor 1
const int LINE_2 = 6;  //line sensor 2
const int LINE_3 = 7;  //line sensor 3

const int DIR_ML = 8;  //Left Motor direction low=fwd
const int PWM_ML = 9;  //Motor speed
const int PWM_MR = 10; //Motor speed
const int DIR_MR = 11; //Right motor direction low=fwd

const int trigger = 13;
const int echo = 4;

int maxdistance = 200;

NewPing sonar(trigger,echo,maxdistance); // NewPing setup of pins and maximum distance.


int speed;
int ramspeed = 255;              // Ram speed
int safespeed = 150;              // Neutral speed
int stucktimer = 0;
int timer = 0;

int liner = 0;
int randomizer = 0;
int z = 0;
int y = 0;

float distance;                  // Distance measured by the ultrasonic sensor

void setup() {                
  Serial.begin(9600);

  pinMode(DIR_ML, OUTPUT);       // Set all motor driver pins to output
  pinMode(PWM_ML, OUTPUT);
  pinMode(PWM_MR, OUTPUT);
  pinMode(DIR_MR, OUTPUT);

  pinMode(LINE_1, INPUT);         // All line sensor pins should be set as input
  pinMode(LINE_2, INPUT);
  pinMode(LINE_3, INPUT);
}

void loop()  {

  ultraread();

  if(distance > 15)
  {
    safeDrive();
  }
    if((z == 0 && distance <= 15) && y == 0)
  {
    z = 1;
  }

  if(z == 1  && timer <= 10)
  {

    LEFT_TURN();
    timer++;
    stucktimer++;
    delay(40);

    if(timer == 10)
    {
      timer = 20;
      y = 1;
      z = 2;
    }
  }
 
  // If right is not the way, turn left
  if(distance <= 15 && y == 1)
  {
    y = 2;
  }
    
  if(y == 2 && timer >= 20)
  {
    RIGHT_TURN();
    timer++;
        stucktimer++;
    delay(40);
    
    if(timer == 30)
    {
      timer = 0;
      y = 0;
      z = 0;
    }
  }
  
  // If bot is stuck,
  if(stucktimer >= 100)
  {
    stucktimer = 0;
    ROTATE();
    delay(800);
  }
    



}

// PRESET FUNCTIONS:

void ultraread()
{
  delay(10);
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  distance = sonar.convert_cm(uS);
  Serial.println(distance);
}

void safeDrive()
{    

  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML, safespeed);    
  analogWrite(PWM_MR, safespeed);    

}

void WIGGLE()      // One can use this instead of the rotate
{
  digitalWrite(DIR_ML, HIGH);
  digitalWrite(PWM_ML, LOW);
  digitalWrite(PWM_MR, LOW);
  digitalWrite(DIR_MR, LOW);
  delay(200);
  digitalWrite(DIR_ML, LOW);
  digitalWrite(PWM_ML, LOW);
  digitalWrite(PWM_MR, LOW);
  digitalWrite(DIR_MR, HIGH);
  delay(200);
  digitalWrite(DIR_ML, HIGH);
  digitalWrite(PWM_ML, HIGH);
  digitalWrite(PWM_MR, HIGH);
  digitalWrite(DIR_MR, HIGH);

}

void ROTATE()    // Default reaction after timer runs out
{
  digitalWrite(DIR_ML, HIGH);
  digitalWrite(PWM_ML, LOW);
  digitalWrite(PWM_MR, LOW);
  digitalWrite(DIR_MR, LOW);
}



void STOP()
{
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML,1);
  analogWrite(PWM_MR,1);
  delay(10);
}

void CHARGE()
{
  int fulldrive = (ramspeed - 255);
  digitalWrite(DIR_ML, LOW);
  analogWrite(PWM_ML, fulldrive);
  analogWrite(PWM_MR, fulldrive);
  digitalWrite(DIR_MR, LOW);
}



void LEFT_TURN()
{     
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML,10);
  analogWrite(PWM_MR,250);
}

void RIGHT_TURN()
{
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML,250);
  analogWrite(PWM_MR,10);
}
