/*
P-BOT Junior Demo Program

>>-     Line following Mobot     -<<
>>-by e-Gizmo Mechatronix Central-<<
Date modified October 30, 2015

This demo program will make the P-BOT Juinor detect to follow
a black line path laid out on a light colored surface.
track width is 15-19mm wide.


I/O Usage
- collision sensor inputs (not used in this demo)
2- colision1 as input
3- colision2 as input
4- colision3 as input

-line sensor inputs
5- linesense1 as input low on black
6- linesense2 as input
7- linesense3 as input

-motor control output
8 - mot2 dir as output high=fwd
9 - mot2 run as output
11 - mot1 dir as output high= fwd
10 - mot1 run as output
 
 */
 //PBOT Junior Functions
int  ls1 = 5;
int  ls2 = 6;
int  ls3 = 7;
/*******NOTE: In P-BOT jr version the Motor is inverted:***********/
//Motor1 Run == LOW or 0 (PWM = 255) fullspeed; HIGH or 1 (PWM =0) stop;
//Motor1 Direction == LOW (forward); HIGH (bakcward);
//Motor 2 Run == LOW or 0 (PWM = 255) fullspeed;HIGH or 1 (PWM =0) stop;
//Motor 2 Direction == LOW (forward); HIGH (bakcward);
int  m2dir = 8;
int m2run = 9;
int m1dir = 11;
int m1run = 10;
long int rtc;

// Initialization routine
void setup()   {                
  pinMode(m2dir, OUTPUT);
  pinMode(m2run, OUTPUT);
  pinMode(m1dir, OUTPUT);
  pinMode(m1run, OUTPUT);
  pinMode(13, OUTPUT);
  rtc=millis()+10;
}
int  linesense=0;
int giveup=0;
int lastsense;
int runspeed=140;

//timers
byte  retry_dly=0;
byte  ledflsh=25;

// Main Line following program loop

void loop()                     
{
   // Hardware Timer service
  if(millis()>=rtc)
  {
    rtc=rtc+10;
    if(retry_dly>0) retry_dly--;
    if(ledflsh>0)
    {
      ledflsh--;
      if(ledflsh==0)
      {
        ledflsh=25;
        PORTB ^= 0x20;
      }
    }                 
  }      
   // read the status of colision and line sensors
  linesense=0;
  if(digitalRead(ls1)==LOW) linesense=1;
  if(digitalRead(ls2)==LOW) linesense=linesense+2;
  if(digitalRead(ls3)==LOW) linesense=linesense+4;  

  // if no line is detected (all high)
  if((linesense==0) & (retry_dly==0)){
    // REVERSE
    if(giveup<10)
    {
      if(lastsense==1) runBot(runspeed*15/10,runspeed,LOW);
      if(lastsense==3) runBot(runspeed*12/10,runspeed,LOW);
      if(lastsense==4) runBot(runspeed,runspeed*15/10,LOW);
      if(lastsense==6) runBot(runspeed,runspeed*12/10,LOW);       
      delay(40);
      giveup++;
    }
    else
    stop();
  }
  if(linesense!=0)
  {
    lastsense=linesense;
    giveup=0;
    retry_dly=50;
  }        
  //TURN SOUTH FAST
  if(linesense==1)
  {
    dirfwd();
    analogWrite(m2run, runspeed);
    analogWrite(m1run, 0);
    delay(50);        
  } 
  // GO FORWARD STRAIGTH 
  if((linesense==2)|(linesense==7))
  {
    runBot(runspeed,runspeed,HIGH);
    delay(50);
  }      
  //TURN SOUTH 
  if(linesense==3)
  {
    dirfwd();
    analogWrite(m2run, runspeed*12/10);
    delay(50);       
  }  
  //TURN NORTH FAST
  if(linesense==4)
  {
    dirfwd();
    analogWrite(m1run, runspeed);
    analogWrite(m2run, 0);
    delay(50);       
  }
  //TURN NORTH
  if(linesense==6)
  {
    dirfwd();
    analogWrite(m1run, runspeed*12/10);
    delay(50);       
  }  
}
void runBot(int spd1,int spd2, boolean direction )
{
  digitalWrite(m2dir,direction);
  digitalWrite(m1dir,direction);
  analogWrite(m1run, spd1);
  analogWrite(m2run, spd2); 
}

void dirfwd(void)
{
  digitalWrite(m2dir,HIGH);
  digitalWrite(m1dir,HIGH);
}    

void rotBot(int speed, boolean direction)
{
  digitalWrite(m2dir,direction);
  digitalWrite(m1dir,~direction);
  analogWrite(m2run, speed);
  analogWrite(m1run, speed); 
}

void Stop(void)
{
    analogWrite(m2run, 255);
    analogWrite(m1run, 255); 
}



