/**************************************************************************
 * Wizard Chest
 * 
 * This code is for a magical chest 
 * that is unlocked with a Magic Wand
 * 
 * Written by The Magic Of Things
 *************************************************************************/
#include <EEPROM.h>

// Arduino pin connected to the lock
#define lockPin            3

// Arduino pin connected to the keyhole led
#define keyholeLED            10

int sensorPin[3] = {5, 6, 7};           // Arduino pin connected to the Hall effect sensors
int sensorReading[3];                   // reading from the sensor pin
int currentState[3] = {1, 1, 1};        // the current state of the output pin
int previousSate[3] = {0, 0, 0};        // the previousSate reading from the input pin

int input[4] = {0, 0, 0, 0};
int pass[4] = {0, 0, 0, 0};

bool defaultPass = 1;

// Variables used for debouncing
long time = 0;         // the last time the output pin was toggled
long debounce = 100;   // delay between readings


void setup() {
  //Initialise the sensor pins as an input with a pullup resistor
  pinMode(sensorPin[0], INPUT_PULLUP);
  pinMode(sensorPin[1], INPUT_PULLUP);
  pinMode(sensorPin[2], INPUT_PULLUP);
  //Initialise the lock and led pins as an output
  pinMode(lockPin, OUTPUT);
  pinMode(keyholeLED, OUTPUT);
  digitalWrite(lockPin, LOW);
  digitalWrite(keyholeLED, LOW);
  Serial.begin(9600);

  //hold the wand at the first touch point during start up to enter menu mode
  sensorReading[0] = digitalRead(sensorPin[0]); //Get a reading from the sensor
  if(sensorReading[0]==0){
    menuMode();
    
    }

    //Read the password stored in EEPROM 
    pass[0] = EEPROM.read(0);
    pass[1] = EEPROM.read(1);
    pass[2] = EEPROM.read(2);
    pass[3] = EEPROM.read(3);
    

}

void loop() {

  checkState();
  checkCombination();
  
}


//Update the rray that stores the password and shift all values left
void shiftArray(){
  int tmp;
  int *p = &input[0];
  int *q = &input[1];
  tmp = *p;  // Copy input[0] to tmp
  for (int i=0; i<3; i++)
  {
    *(p++) = *(q++);      // Shift input[1-4] down to input[0-3]
  }
  *p = tmp;  // copy old input[0] to input[3]
  
}

void checkState(){

  for(int i=0; i<3; i++){
  sensorReading[i] = digitalRead(sensorPin[i]); //Get a reading from the sensor
  
  // if the input changed between HIGH and LOW and we've waited long enough
  // for debouncing, toggle the output pin and record the current time
  if (sensorReading[i] == LOW && previousSate[i] == HIGH && millis() - time > debounce) {
    if(sensorReading[i] == LOW){
    shiftArray();
    input[3] = i+1;
    }
   
    time = millis();    
  }
  
  previousSate[i] = sensorReading[i];  
  
  }
  
}

void checkCombination(){
  
  //check if the input array matches the unlock code  
  if(input[0] == pass[0] && input[1] == pass[1] && input[2] == pass[2] && input[3] == pass[3]){
    
    digitalWrite(lockPin, HIGH);
    digitalWrite(keyholeLED, HIGH);
    delay(1000);                  //it takes 0.8s for the lock to trigger
    digitalWrite(lockPin, LOW);   //turn the lock back off
    delay(1000);                  // keep the led on for a bit longer after unlock
    digitalWrite(keyholeLED, LOW);//turn the led off
    //reset the input array after successful code entry
    input[0] = 0;
    input[1] = 0;
    input[2] = 0;
    input[3] = 0;
   }
    else{
      digitalWrite(lockPin, LOW);
    }
}

void menuMode(){
  flashLED();
  //run the check state function untill all four entries are filled
  while(input[0]== 0){
  checkState();
  }
  for(int j=0; j<4; j++){
  pass[j] = input[j];
  }
  
  //Store the new password in EEPROM
  EEPROM.write(0, pass[0]);
  EEPROM.write(1, pass[1]);
  EEPROM.write(2, pass[2]);
  EEPROM.write(3, pass[3]);
}


//Flash the LED few time to indicate menue mode
void flashLED(){
    for(int i=0; i<5; i++){
      digitalWrite(keyholeLED, HIGH);
      delay(400);
      digitalWrite(keyholeLED, LOW);
      delay(200);
    }
}
