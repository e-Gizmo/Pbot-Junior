// e-Gizmo OBSTAPROX PBOT junior
/*
This enables a pbot to avoid obstacles and
may also solve mazes. The program does not
include the use of the line sensor and is
optional. The codes for the line sensors
are included just case it is needed.

Codes by:
e-Gizmo Mechatronix Central
Taft, Manila, Philippines
http://wwww.e-gizmo.com
January 11, 2014
*/

// Connections:
const int IR_SENSOR = 16; // Infrared sensor
const int LINE_1 =5;      // Line sensor 1
const int LINE_2 = 6;     // Line sensor 2
const int LINE_3 = 7;     // Line sensor 3

const int DIR_ML = 8;     // Right Motor Direction LOW = FWD
const int PWM_ML = 9;     // Speed control
const int PWM_MR = 10;    // Speed control
const int DIR_MR = 11;    // Left Motor Direction LOW = FWD

int safespeed = 250;              // Neutral speed
int timer = 0;
int stucktimer = 0;
int rotate;

int z = 0;
int y = 0;


void setup() 
{                
  Serial.begin(9600);

  pinMode(DIR_ML, OUTPUT);       // Set all motor driver pins to output
  pinMode(PWM_ML, OUTPUT);
  pinMode(PWM_MR, OUTPUT);
  pinMode(DIR_MR, OUTPUT);

  pinMode(IR_SENSOR, INPUT);

  pinMode(LINE_1, INPUT);         // All line sensor pins should be set as input
  pinMode(LINE_2, INPUT);
  pinMode(LINE_3, INPUT);
}

void loop()  {

  int infraSense = digitalRead(IR_SENSOR);

  ////////////////// MOBOT Functions ///////////////////////

  if(infraSense == 0)
  {
    FORWARD();
  }


  // If the bot is about to hit the wall, turn right
  if((z == 0 && infraSense == 0) && y == 0)
  {
    z = 1;
  }

  if(z == 1  && timer <= 10)
  {

    LEFT_TURN();
    timer++;
    stucktimer++;
    delay(80);

    if(timer == 10)
    {
      timer = 20;
      y = 1;
      z = 2;
    }
  }
  
  // If right is not the way, turn left
  if(infraSense == 0 && y == 1)
  {
    y = 2;
  }
    
  if(y == 2 && timer >= 20)
  {
    RIGHT_TURN();
    timer++;
        stucktimer++;
    delay(80);
    
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

void FORWARD()
{ 
  int forwardspeed = (safespeed - 255);  
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML, forwardspeed);    
  analogWrite(PWM_MR, forwardspeed);    
}

void BACKWARD()
{    
  int backspeed = (safespeed - 255);
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML, backspeed);    
  analogWrite(PWM_MR, backspeed);    
}



void ROTATE()    // Default reaction after timer runs out
{
  digitalWrite(DIR_ML, HIGH);
  digitalWrite(DIR_MR, LOW);
  digitalWrite(PWM_ML, LOW);
  digitalWrite(PWM_MR, LOW);
}


void STOP()
{
  digitalWrite(DIR_ML, HIGH);
  digitalWrite(DIR_MR, HIGH);

  analogWrite(PWM_ML, 255);
  analogWrite(PWM_MR, 255);
  delay(10);
}


void LEFT_TURN()
{     
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML, 250);
  analogWrite(PWM_MR, 10);
}

void RIGHT_TURN()
{
  digitalWrite(DIR_ML, LOW);
  digitalWrite(DIR_MR, LOW);

  analogWrite(PWM_ML, 10);
  analogWrite(PWM_MR, 250);
}
