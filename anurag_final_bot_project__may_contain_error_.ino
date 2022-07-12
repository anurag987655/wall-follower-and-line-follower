#define LEFT_ECHO 6
#define LEFT_TRIG 7
#define RIGHT_ECHO 8 
#define RIGHT_TRIG 9

#define MIDDLE_SENSOR 11
#define LEFT_SENSOR 12
#define RIGHT_SENSOR 13

#define LEFT_MOTOR_11 2
#define LEFT_MOTOR_12 3
#define RIGHT_MOTOR_11 4
#define RIGHT_MOTOR_12 5

int MIN_SPEED=0;
int MAX_SPEED=150;


int preerror=0;
int derror=0;
int ierror=0;
int error=0;


int kp=0;
int kd=0;
int ki=0;


int pd;


int DISTANCE_FRONT=0;
int DISTANCE_RIGHT=0;

int speedl;
int speedr;

const int SIDE_WIDTH=30;
const  int SIDE_CLEARANCE=12;


 void fastleft()
{
  digitalWrite(LEFT_MOTOR_11,LOW);
 digitalWrite(LEFT_MOTOR_12,HIGH);
 digitalWrite(RIGHT_MOTOR_11,HIGH);
 digitalWrite(RIGHT_MOTOR_12,LOW);
  
}


void moveforward()
{
  digitalWrite(LEFT_MOTOR_11,LOW);
 digitalWrite(LEFT_MOTOR_12,HIGH);
 digitalWrite(RIGHT_MOTOR_11,LOW);
 digitalWrite(RIGHT_MOTOR_12,HIGH);
   
}

void fastright()
{
  digitalWrite(LEFT_MOTOR_11,HIGH);
 digitalWrite(LEFT_MOTOR_12,LOW);
 digitalWrite(RIGHT_MOTOR_11,LOW);
 digitalWrite(RIGHT_MOTOR_12,HIGH);
}

void right()
{
  digitalWrite(LEFT_MOTOR_11,LOW);
 digitalWrite(LEFT_MOTOR_12,LOW);
 digitalWrite(RIGHT_MOTOR_11,LOW);
 digitalWrite(RIGHT_MOTOR_12,HIGH);
}

void left()
{
digitalWrite(LEFT_MOTOR_11,LOW);
 digitalWrite(LEFT_MOTOR_12,HIGH);
 digitalWrite(RIGHT_MOTOR_11,LOW);
 digitalWrite(RIGHT_MOTOR_12,LOW);
}

void stopmybot()
{
  digitalWrite(LEFT_MOTOR_11,LOW);
 digitalWrite(LEFT_MOTOR_12,LOW);
 digitalWrite(RIGHT_MOTOR_11,LOW);
 digitalWrite(RIGHT_MOTOR_12,LOW);
}


void setup() 
{
Serial.begin(9600);
pinMode(MIDDLE_SENSOR,INPUT);
pinMode(LEFT_SENSOR,INPUT);
pinMode(RIGHT_SENSOR,INPUT);

pinMode(LEFT_MOTOR_11,OUTPUT);
pinMode(LEFT_MOTOR_12,OUTPUT);
pinMode(RIGHT_MOTOR_11,OUTPUT);
pinMode(RIGHT_MOTOR_12,OUTPUT);

pinMode(LEFT_TRIG,OUTPUT);
pinMode(RIGHT_TRIG,OUTPUT);

pinMode(LEFT_ECHO,INPUT);
pinMode(RIGHT_ECHO,INPUT);
}



void loop() 
{




  if (digitalRead(MIDDLE_SENSOR))
  {
    if (!digitalRead(LEFT_SENSOR) && !digitalRead(RIGHT_SENSOR))//ls and rs not on the line 
    {
      Serial.println("move forward");
     moveforward();
      
    }
else if (digitalRead(LEFT_SENSOR) && !digitalRead(RIGHT_SENSOR))
{
  Serial.println("sharp left");
  fastleft();
}

else if (!digitalRead(LEFT_SENSOR) && digitalRead(RIGHT_SENSOR))
{
  Serial.println("sharp right");
 fastright();
}

else if (digitalRead(LEFT_SENSOR) && digitalRead(RIGHT_SENSOR))
{
  Serial.println("stop car");
  stopmybot();
}
  }
else
if (digitalRead(LEFT_SENSOR) && !digitalRead(RIGHT_SENSOR))
{
  Serial.println("slight left");
 left();
}
  

if (!digitalRead(LEFT_SENSOR) && digitalRead(RIGHT_SENSOR))
{
  Serial.println("slight right"); 
   right();
}


if (!digitalRead(LEFT_SENSOR) && !digitalRead(RIGHT_SENSOR))
{
  Serial.println("use of ultrasonic sensor ");

  long DISTANCE_LEFT=readsensor(1);
long DISTANCE_RIGHT=readsensor(2);



if ((DISTANCE_LEFT+DISTANCE_RIGHT)>SIDE_WIDTH)
{
  if ((DISTANCE_RIGHT)>(DISTANCE_LEFT))
  {
    DISTANCE_RIGHT=SIDE_WIDTH-DISTANCE_LEFT;
  }

  else if((DISTANCE_LEFT)>(DISTANCE_RIGHT))
  {
    DISTANCE_LEFT=SIDE_WIDTH-DISTANCE_RIGHT;
   }
}
if ((DISTANCE_LEFT<SIDE_WIDTH)&&(DISTANCE_RIGHT)<SIDE_WIDTH)
{
kp=1;
kd=10;

if (pd>0)
{
   speedr-=pd;
   speedl=MAX_SPEED;
}
}

else if(pd<0)
{
  speedl+=pd;
  speedr=MAX_SPEED;
}


 if ((DISTANCE_LEFT>SIDE_CLEARANCE) || (DISTANCE_RIGHT>SIDE_CLEARANCE))
 {
  kp=6;
  kd=8;
  ki=.7;

  if (pd>0)
  {
    speedr-=pd;
    speedl=MAX_SPEED;
  }
  else if(pd<0)
  {
    speedl+=pd;
    speedr=MAX_SPEED;
  }
 }




if (speedl<MIN_SPEED)speedl=MIN_SPEED;
if (speedr<MIN_SPEED)speedr=MIN_SPEED;

if (speedl>MAX_SPEED)speedl=MAX_SPEED;
if (speedr>MAX_SPEED)speedr=MAX_SPEED;



error=DISTANCE_RIGHT-DISTANCE_LEFT;
derror=error-preerror;
ierror+=error;



preerror=error;


pd=kp*error+kd*derror+ki*ierror;



analogWrite(LEFT_MOTOR_11,speedl);
analogWrite(LEFT_MOTOR_12,0);
analogWrite(RIGHT_MOTOR_11,speedl);
analogWrite(RIGHT_MOTOR_12,speedl);



}
}


long readsensor(int sensor)
{
  int TRIG_PIN,ECHO_PIN;

  if (sensor==1)
  {
    TRIG_PIN=LEFT_TRIG;
    ECHO_PIN=LEFT_ECHO;
    
  }
  else if(sensor==2)
  {
    TRIG_PIN=RIGHT_TRIG;
    ECHO_PIN=RIGHT_ECHO;
  }
  long duration,distance;
  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(10);


  duration=pulseIn(ECHO_PIN,HIGH);

  return (duration/2)/29.1;
}


  
