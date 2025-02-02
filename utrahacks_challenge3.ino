// TCS230 or TCS3200 pins wiring to Arduino
//CHALLENGE 3
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
//define cardboard colour


int ultra_echo = 7;
int ultra_trig = 8;

int motor1pin1 = 9;
int motor1pin2 = 10;
int motor2pin1 = 11;
int motor2pin2 = 12;

int servo = 13;

String sequence[] = {"Red", "Green", "Blue", "Green", "Blue"};
int counter = 0;
// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
String currentColour = "";
int counterRed = 0;
int counterBlue = 0;
int counterGreen = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

unsigned long moveStartTime = 0;
unsigned long totalMoveTime = 0;
bool isMoving = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Set LED pin as output
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

    // ultrasensor
  pinMode(ultra_trig, OUTPUT);
  pinMode(ultra_echo, INPUT);
  

// motor
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);


  // Begins serial communication
  Serial.begin(9600);
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

void angleturn(int angle) {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  float timeturn = angle*(1250/(3.141592/2));
  delay(timeturn); // 1250 = pi/2
  stop();
}
String rgb_count(){
  String detectedColor = "";

  if (redColor > greenColor && redColor > blueColor) {
    detectedColor = "Red";
  } 
  else if (greenColor > redColor && greenColor > blueColor) {
    detectedColor = "Green";
  } 
  else if (blueColor > redColor && blueColor > greenColor) {
    detectedColor = "Blue";
  }

  if (detectedColor != currentColour) {
    if (detectedColor == "Red") counterRed++;
    if (detectedColor == "Green") counterGreen++;
    if (detectedColor == "Blue") counterBlue++;

    if (counterRed >= 3 || counterGreen >= 3 || counterBlue >= 3) {
      currentColour = detectedColor;
      Serial.println("Color change detected: " + currentColour);
      counterRed = counterGreen = counterBlue = 0;
    }
  }
  return currentColour;
}

void check(){
  if (rgb_count() == sequence[counter]){
    digitalWrite(LED_BUILTIN, HIGH); // Turn LED on
    delay(2000);                     // Wait for 1 second
    digitalWrite(LED_BUILTIN, LOW);  // Turn LED off
    delay(1000);                     // Wait for 1 second
    counter += 1;
  }
}

void loop() {
  float duration, distance;
  // put your main code here, to run repeatedly:
  digitalWrite(ultra_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultra_trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(ultra_trig, LOW);

  duration = pulseIn(ultra_echo, HIGH);
  distance = (duration*.0343)/2;
  

  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);

  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // redColor = map(redFrequency, 70, 120, 255,0);
  redColor = map(redFrequency, 180, 600, 255,0);

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
  check();

  // if no wall, move forward; else if wall turn right (or left to optimise depending on where start is)
  // maybe add randomly generated code to turn lol
  if (distance <20){
    stop();
    goBackward();
    delay(350);
    turnRight();
    goForward();

  } else {
    goForward();
  }

}

// TODO: moving.. 

