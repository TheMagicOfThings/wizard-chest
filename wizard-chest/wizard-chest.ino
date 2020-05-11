/**************************************************************************
 * Wizard Chest
 * 
 * This code is for a magical chest 
 * that is unlocked with a Magic Wand
 * 
 * Written by The Magic Of Things
 *************************************************************************/


// Arduino pin connected to the lock
#define lockPin            3


int sensorPin[3] = {5, 6, 7};           // Arduino pin connected to the Hall effect sensors
int sensorReading[3];                   // reading from the sensor pin
int currentState[3] = {1, 1, 1};        // the current state of the output pin
int previousSate[3] = {0, 0, 0};        // the previousSate reading from the input pin

int input[4] = {0, 0, 0, 0};
int pass[4] = {1, 2, 3, 1};

// Variables used for debouncing
long time = 0;         // the last time the output pin was toggled
long debounce = 100;   // delay between readings

void setup() {
  //Initialise the sensor pins as an input with a pullup resistor
  pinMode(sensorPin[0], INPUT_PULLUP);
  pinMode(sensorPin[1], INPUT_PULLUP);
  pinMode(sensorPin[2], INPUT_PULLUP);
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, LOW);
  Serial.begin(9600);

}

void loop() {

for(int j=0; j<4; j++)
{
  Serial.println(input[j]);
}
Serial.println(" ");
delay(200);

  checkState();
  checkCombination();
  
}

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
    delay(300);
    digitalWrite(lockPin, LOW);
    
    //reset the input array
    input[0] = 0;
    input[1] = 0;
    input[2] = 0;
    input[3] = 0;
   }
    else{
      digitalWrite(lockPin, LOW);
    }
}