/*********
  functionality attributed to:
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  

  logic made by us :>
*********/
#include <Servo.h>
#include <stdbool.h>

int ultra_echo = 7;
int ultra_trig = 8;
float duration, distance;


int motor1pin1 = 9;
int motor1pin2 = 10;
int motor2pin1 = 11;
int motor2pin2 = 12;


int servo = 13;

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6

void goForward();
void goBackward();
void turnLeft();
void turnRight();
void stop();
void checkColour();
int getMedian(int arr[], int size);

int wallThreshold = 28;

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
char currentColour = '\0';

// Stores the red. green and blue colors
int redColour = 0;
int greenColour = 0;
int blueColour = 0;
int blackThreshold = 1000;
bool atStart = true;

void setup() {
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

  // ultrasensor
  pinMode(ultra_trig, OUTPUT);
  pinMode(ultra_echo, INPUT);
  
// motor
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
}

void loop() {
  digitalWrite(ultra_trig, LOW);
  delayMicroseconds(10);
  digitalWrite(ultra_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultra_trig, LOW);

  duration = pulseIn(ultra_echo, HIGH);
  distance = (duration*.0343)/2;
  
  Serial.print("Distance: ");
  Serial.println(distance); //loop it to get rid of noise
  if (distance <= wallThreshold){
    Serial.println("wall found...");
    stop();
    Serial.println("detecting...");
    checkColour();
    atStart = false;
    if (currentColour == 'o'){
      stop();
      Serial.println("reached end!");
      delay(100000);
    }
  }
  else if (currentColour == 'n') {
      delay(100);
  }
  else {
    goForward();
    Serial.println("moving forward!");
    delay(50);
  }
}

void checkColour() {
    const int numSamples = 5;  // Number of readings to reduce noise
    int redSamples[numSamples], greenSamples[numSamples], blueSamples[numSamples];

    // Take multiple samples for each color
    for (int i = 0; i < numSamples; i++) {
        // Read Red
        digitalWrite(S2, LOW);
        digitalWrite(S3, LOW);
        redSamples[i] = pulseIn(sensorOut, LOW);
        delay(50);  // Short delay to stabilize sensor

        // Read Green
        digitalWrite(S2, HIGH);
        digitalWrite(S3, HIGH);
        greenSamples[i] = pulseIn(sensorOut, LOW);
        delay(50);

        // Read Blue
        digitalWrite(S2, LOW);
        digitalWrite(S3, HIGH);
        blueSamples[i] = pulseIn(sensorOut, LOW);
        delay(50);
    }

    // Get the median values to filter out noise
    int redFrequency = getMedian(redSamples, numSamples);
    int greenFrequency = getMedian(greenSamples, numSamples);
    int blueFrequency = getMedian(blueSamples, numSamples);

    // Map the frequency values to color intensity (0-255)
    int redColour = map(redFrequency, 180, 600, 255, 0);
    int greenColour = map(greenFrequency, 310, 700, 255, 0);
    int blueColour = map(blueFrequency, 210, 670, 255, 0);

    // Determine the detected color
    if (redColour > greenColour && redColour > blueColour) {
        currentColour = 'r';  // U-Turn
        digitalWrite(motor2pin1, HIGH);
        digitalWrite(motor2pin2, LOW);
        delay(50);
        digitalWrite(motor1pin1, HIGH);
        digitalWrite(motor1pin2, LOW);
        delay(350);
        stop();
        delay(50);
      
        turnLeft();
        turnLeft();
        Serial.println("u-turn!");
    } 
    else if (blueColour > redColour && blueColour > greenColour) {
        currentColour = 'b';  // Turn Left
        digitalWrite(motor2pin1, HIGH);
        digitalWrite(motor2pin2, LOW);
        delay(50);
        digitalWrite(motor1pin1, HIGH);
        digitalWrite(motor1pin2, LOW);
        delay(350);
        stop();
        delay(50);

        turnLeft();
        
        Serial.println("turn left!");
    } 
    else if (greenColour > redColour && greenColour > blueColour) {
        currentColour = 'g';  // Turn Right
        digitalWrite(motor2pin1, HIGH);
        digitalWrite(motor2pin2, LOW);
        delay(50);
        digitalWrite(motor1pin1, HIGH);
        digitalWrite(motor1pin2, LOW);
        delay(350);
        stop();
        delay(50);

        turnRight();
        Serial.println("turn right!");
    } 
    else if (greenFrequency > blackThreshold && blueFrequency > blackThreshold && redFrequency > 450) {
        currentColour = 'o';  // Black detected
    }
    else {
      currentColour = 'n';
      Serial.println("nahhh bro u trollin");
    }
}
int getMedian(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[i]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return arr[size / 2];  // Return the median value
}

void stop() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  delay(1000);
}

void turnRight() {
  // Turns the left wheel
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  delay(1100);
  stop();
}

void turnLeft() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(1200);
  stop();
}

void goBackward() {
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  delay(50);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  delay(1000);
}

void goForward() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(50);
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  delay(200);
}

void uTurn() {
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  delay(50);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  delay(200);
}
