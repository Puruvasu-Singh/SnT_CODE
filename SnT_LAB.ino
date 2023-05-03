//INCLUDING DISPLAY
#include <TM1637.h>
#define CLK 13
#define DIO 12
TM1637 tm(CLK, DIO);

// DEFINING SERVO
#include <Servo.h>
Servo barrier;

// DEFINING ULTRASONIC PREIFREALS
#define ECHO1 6
#define TRIG1 7
#define ECHO2 4
#define TRIG2 5

unsigned long refrence;
int measurement_delay = 100;

//DEFINING ALARM PHERIPHERALS
#define BUZZER 8

// DEFINING INDICATING PERIPHERALS
#define RED 11
#define GREEN 10

//defining count of CARS
int count;
int dis1;
int dis2;
int thresh = 10;

//defining interrupt 
#define INCR 3
#define DECR 2
int entering = false;
int leaving = false;

void incrementCount();
void decrementCount();
void displayCount(int count);


// ULTRASONIC TRIGGERS
void trigger1(){
  digitalWrite(TRIG1 , LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1 , HIGH );
  delayMicroseconds(10);
  digitalWrite(TRIG1 , LOW);
  
}

void trigger2(){
  digitalWrite(TRIG2 , LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG2 , HIGH );
  delayMicroseconds(10);
  digitalWrite(TRIG2 , LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ECHO1 , INPUT);
  pinMode(ECHO2 , INPUT);
  pinMode(TRIG1 , OUTPUT);
  pinMode(TRIG2 , OUTPUT);

  pinMode(BUZZER , OUTPUT);

  pinMode(RED , OUTPUT);
  pinMode(GREEN , OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(INCR), increment, RISING);
  attachInterrupt(digitalPinToInterrupt(DECR), decrement, RISING);

  tm.init();
  tm.set(BRIGHT_TYPICAL);

  barrier.attach(9);
  barrier.write(90);
}

void loop() {
 unsigned long timeNow = millis();
 if(timeNow - refrence > measurement_delay){
    refrence+=measurement_delay;
     trigger1();
    dis1 = (int)pulseIn(ECHO1 , HIGH) / 58;
    delayMicroseconds(10);
    trigger2();
    dis2 = (int)pulseIn(ECHO2 , HIGH) / 58;
 }

  if(dis1 < thresh && dis2 > thresh){
      entering = true;
    }
  else if(dis2 < thresh && dis1 > thresh){
      leaving = true;
    }
  else if(dis1 < thresh && dis2 < thresh){
    tone(BUZZER , 500 , 1000);
    digitalWrite(RED , HIGH);
    digitalWrite(GREEN , HIGH);
    barrier.write(15);
    delay(3000);
    } 
  else{
    digitalWrite(BUZZER , LOW);
    digitalWrite(RED , LOW);
    digitalWrite(GREEN , LOW);
    barrier.write(90);
    }

  if(entering){
    incrementCount();
    entering = false;
  }
  if(leaving){
    decrementCount();
    leaving = false;
  }
       
  displayCount(count);
}

void incrementCount(){
  tone(BUZZER , 260 , 1000);
  digitalWrite(GREEN , HIGH);
  barrier.write(15);
  delay(3000);
  count++;
}

void decrementCount(){
  tone(BUZZER , 400 , 1000);
  digitalWrite(RED , HIGH);
  barrier.write(15);
  delay(3000);
  count--;
}

void displayCount(int count){
  tm.display(0, (count / 1000) % 10);
  tm.display(1, (count / 100) % 10);
  tm.display(2, (count / 10) % 10);
  tm.display(3, count % 10); 
}

void increment(){
  entering = true;
}
void decrement(){
  leaving = true;
}
