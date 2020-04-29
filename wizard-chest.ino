/**************************************************************************
 * Wizard Chest
 * 
 * This code is for a magical chest 
 * that is unlocked with a Magic Wand
 * 
 * Written by The Magic Of Things
 *************************************************************************/


// Arduino pin connected to the lock
#define lockPin            13


int sensor1Pin = 5;             // Arduino pin connected to the Hall effect sensor
int sensor2Pin = 6;             // Arduino pin connected to the Hall effect sensor
int sensor3Pin = 7;             // Arduino pin connected to the Hall effect sensor

int sensor1Reading;             // reading from the sensor pin
int sensor2Reading;             // reading from the sensor pin
int sensor3Reading;             // reading from the sensor pin

bool resetCode = LOW;           // resets the code sequence

void setup()
{
  //Initialise the sensor pins as an input with a pullup resistor
  pinMode(sensor1Pin, INPUT_PULLUP);
  pinMode(sensor2Pin, INPUT_PULLUP);
  pinMode(sensor3Pin, INPUT_PULLUP);
  digitalWrite(lockPin, LOW);
  Serial.begin(9600);
}


//Main program loop
void loop(){

  readSensor1();// only reed the first sensor in the sequence untill a wand is detected
  
}

//Send a 1s pulse to the lock
void unlock(){
  resetCode=HIGH;
  digitalWrite(lockPin, HIGH);
  delay(1000);
  digitalWrite(lockPin, LOW);
  }

//Read the first sensor in the sequence
//if a wand is detected move to the second sensor in the sequence
void readSensor1(){
  sensor1Reading = digitalRead(sensor1Pin); //Get a reading from the sensor
  delay(100);
  resetCode=LOW;
  //If a wand is detected move to the next sensor in the sequence
  if(sensor1Reading == LOW){
    readSensor2();
    }
  }

//Read the second sensor in the sequence
void readSensor2(){
  //keep reading for some time before restarting the sequence
  for(int counter=0; counter <=30; counter++){
    //If the correct sequence was detected 
    //exit the for loop
    if (resetCode == HIGH){
        break;
        } 
  sensor2Reading = digitalRead(sensor2Pin); //Get a reading from the sensor
  delay(100);
  //If a wand is detected move to the next sensor in the sequence
  if(sensor2Reading == LOW){
    readSensor3();
    return;
    }
    //Keep checking the first sensor if the sequence is restarted
    readSensor1();
  }
}

//Read the third sensor in the sequence
void readSensor3(){
  for(int counter=0; counter <=30; counter++){
  sensor3Reading = digitalRead(sensor3Pin); //Get a reading from the sensor
  delay(100);
  if(sensor3Reading == LOW){
    unlock();
    break;
    }
    //Keep checking the first sensor if the sequence is restarted
    readSensor1();
  }
}
