#include <Servo.h>
#include <LiquidCrystal.h>
Servo myservo;
const int rs = 10, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  int scanRange;
  int readings;
  const int trigPin = 7;
  const int echoPin = 6; 
  float duration;
  float distance;
  myservo.attach(11);
  myservo.write(0);

  lcd.begin(16, 2);
  lcd.print("See monitor");

  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  

  Serial.begin(9600);

  Serial.println(" ");
  Serial.println("Enter degrees (1-180)");
  while (Serial.available() == 0) {}
  scanRange = Serial.parseInt();

  while (Serial.available()) Serial.read();  

  Serial.println("Enter number of readings (2-100)");
  while (Serial.available() == 0) {}
  readings = Serial.parseInt();

  Serial.print("Degrees entered: ");
  Serial.println(scanRange);
  Serial.print("Readings entered: ");
  Serial.println(readings);

  float *distanceArray = (float*)malloc(sizeof(float) * readings);
  float stepSize = scanRange / float(readings - 1);

  lcd.clear();
  delay(10);
  lcd.print("Scanning...");

  for(int i = 0; i < readings; i++) {
    
    myservo.write(i * stepSize);
    delay(10000/readings);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); 
    distance = (duration * .0343)/2;  
    distanceArray[i] = distance;
    delay(250);
  }


  float farthest = 0;
  float closest = 10000;
  float farangle, closeangle;

  

  for(int i = 0; i < readings; i++) {
    if(distanceArray[i] > farthest) {
      farthest = distanceArray[i];
      farangle = i * stepSize;
    }
    if(distanceArray[i] < closest) {
      closest = distanceArray[i];
      closeangle = i * stepSize;
    }

  }

  Serial.println("List of distances (in cm) and angles:");

  for(int i = 0; i < readings; i++) {
    float angle = i * stepSize;
    Serial.print(distanceArray[i]);
    Serial.print(" cm | ");
    Serial.print(angle);
    Serial.println(" degrees");
  }

  lcd.clear();
  delay(10);
  lcd.print("Closest: ");
  lcd.setCursor(0, 1);
  lcd.print(closest);
  lcd.print(" cm");

  delay(4000);
  lcd.clear();

  lcd.print("Close angle: ");
  lcd.setCursor(0, 1);
  lcd.print(closeangle);
  lcd.print(" degrees");
  delay(4000);

  delay(4000);
  lcd.clear();

  lcd.print("Farthest: ");
  lcd.setCursor(0, 1);
  lcd.print(farthest);
  lcd.print(" cm");
  
  delay(4000);
  lcd.clear();

  lcd.print("Far angle: ");
  lcd.setCursor(0, 1);
  lcd.print(farangle);
  lcd.print(" degrees");
  

  myservo.write(0);
  free(distanceArray);
}

void loop() {
  // this section just repeats forever, which we don't need to do
}