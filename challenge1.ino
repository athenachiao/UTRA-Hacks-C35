/*********
  functionality attributed to:
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  

  logic made by us :>
*********/
//CHALLENGE 1 CODE
#include <Servo.h>
Servo myservo;

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
#define BLACK_THRESHOLD 1000


int motor1pin1 = 9;
int motor1pin2 = 10;
int motor2pin1 = 11;
int motor2pin2 = 12;


int servo = 13;
int pos = 0; 

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
String currentColour = "";
int counter = 0;
int counterRed = 0;
int counterBlue = 0;
int counterGreen = 0;
int initial_triangle = 1;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

void setup() {
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Begins serial communication
  Serial.begin(9600);

  myservo.attach(13);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
}

void crabDrop() {
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

void crabGrab() {
  for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

void stop() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  delay(1000);
}

void angleturn(int angle) {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  timeturn = angle*(1250/(3.141592/2));
  delay(timeturn); // 1250 = pi/2
  stop();
}

void turnRight() {
  // Turns the left wheel
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  delay(1250);
  stop();
}

void turnLeft() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(1250);
  stop();
}

void goBackward() {
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  delay(1000);
}

void goForward() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  delay(1000);
  }

float angle(float t1, float t2) {
  float turnAngle = atan(t1/t2);
  return turnAngle;
}

float middle(float t1, float t2){
  float dist = sqrt(pow(t1,2)+pow(t2,2))/2;
  return dist;
}

void rgb_count(){
  // Checks the current detected color and prints
  // a message in the serial monitor
  if(redColor > greenColor && redColor > blueColor){
    // if different colour than consensus start counting
    if(currentColour != "Red"){
      if(counterRed < 3){
        counterRed += 1;
      }
      else{
        counterRed = 0;
        Serial.print("Colour change to red");
        currentColour = "Red";
        counter += 1;
      }
    }
  }
  else if(greenColor > redColor && greenColor > blueColor){
    // if different colour than consensus start counting
    if(currentColour != "Green"){
      if(counterGreen < 3){
        counterGreen += 1;
      }
      else{
        counterGreen = 0;
        Serial.print("Colour change to green");
        currentColour = "Green";
        counter += 1;
      }
    }
  }
  else if(blueColor > redColor && blueColor > greenColor){
    // if different colour than consensus start counting
    if(currentColour != "Blue"){
      if(counterBlue < 3){
        counterBlue += 1;
      }
      else{
        counterBlue = 0;
        Serial.print("Colour change to blue");
        currentColour = "Blue";
        counter += 1;
      }
    }
  }

  if(counter == 5){
    // stop motor
    stop();
    Serial.println("stop!!!!!");
  }
}

void check_colour(){
  if(redColor > greenColor && redColor > blueColor){
    Serial.println("Red");
  }
  else if(greenColor > redColor && greenColor > blueColor){
    Serial.println("Green");
  }
  else if(blueColor > redColor && blueColor > greenColor){
    Serial.println("Blue");
}

crabGrab();
void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // redColor = map(redFrequency, 70, 120, 255,0);
  redColor = map(redFrequency, 180, 600, 255, 0);
  
  // Printing the RED (R) value
  // Serial.print("R = ");
  // Serial.print(redColor);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
  greenColor = map(greenFrequency, 310, 700, 255, 0);
  
  // Printing the GREEN (G) value  
  // Serial.print(" G = ");
  // Serial.print(greenColor);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
  blueColor = map(blueFrequency, 210, 670, 255, 0);
  
  // Printing the BLUE (B) value 
  // Serial.print(" B = ");
  // Serial.print(blueColor);
  delay(100);
  if(initial_triangle == 1){
    if (redFrequency > 450 && greenColor < BLACK_THRESHOLD && blueColor < BLACK_THRESHOLD) {
      Serial.println("Black color detected!");
      goForward();
      // move forward
    }
    else{
      initial_triangle = 2;
      float startT1 = millis();
    }
  }
  // inside coloured circle
  if(initial_triangle == 2){
    if (redFrequency < 450 || greenColor > BLACK_THRESHOLD || blueColor > BLACK_THRESHOLD) {  
      // move forward 
      goForward();
    } 
    else{
      float t1 = millis() - startT1;
      // move back a little?
      // turn 90 degrees counterclockwise
      goBackward();
      delay(500);
      stop();
      turnLeft();
      initial_triangle = 3;
      float startT2 = millis();
    }
  }
  //NOTE: ensure that rover moves back into a colour detecting area
  // inside coloured circle again
  if(initial_triangle == 3){
    (redFrequency < 450 || greenColor > BLACK_THRESHOLD || blueColor > BLACK_THRESHOLD)  
      // move forward 
      goForward();
    } 
    else{
      float t2 = millis() - startT2;
      // move back a little
      // turn 45 degrees counter clockwise
      goBackward();
      delay(500);
      stop();
      angleturn(angle(t1, t2));
      initial_triangle = 4;
    }
  }
  else{
    rgb_count();
    crabDrop();

  }
}