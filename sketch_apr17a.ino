#include <Servo.h>
#include <LiquidCrystal.h>
Servo myservo;



void setup() {
  int degrees;
  int readings;
  const int trigPin = 7;  
  const int echoPin = 6; 
  float duration;
  float distance;
  myservo.attach(11);
  const int rs = 10, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  

  Serial.begin(9600);
  while (!Serial);

  Serial.print("Enter degrees (1-180):");
  while (Serial.available() == 0) {}
  degrees = Serial.parseInt();

  while (Serial.available()) Serial.read();  

  Serial.print("Enter number of readings (2-100):");
  while (Serial.available() == 0) {}
  readings = Serial.parseInt();

  Serial.print("Degrees entered: ");
  Serial.println(degrees);
  Serial.print("Readings entered: ");
  Serial.println(readings);

  float *distanceArray = (float*)malloc(sizeof(float) * readings);

  for(int i = 0; i <= readings; i++) {
    
    myservo.write(i * (degrees / readings));
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
  int farangle, closeangle;

  for(int i = 0; i <= readings; i++) {
    if(distanceArray[i] > farthest) {
      farthest = distanceArray[i];
      farangle = i * (degrees / readings);
    }
    if(distanceArray[i] < closest) {
      closest = distanceArray[i];
      closeangle = i * (degrees / readings);
    }

  }

  Serial.print("List of distances (in cm) and angles:\n");

  for(int i = 0; i <= readings; i++) {
    int angle = i * (degrees / readings);
    Serial.print(distanceArray[i]);
    Serial.print(" cm | ");
    Serial.print(angle);
    Serial.println(" degrees");
  }

  lcd.begin(16, 2);
  lcd.print("Closest: ");
  lcd.setCursor(0, 1);
  lcd.print(closest);
  lcd.print("cm");

  lcd.setCursor(0,0);
  delay(4000);

  lcd.print("Close angle: ");
  lcd.setCursor(0, 1);
  lcd.print(closeangle);
  lcd.print("degrees");
  delay(4000);

  lcd.setCursor(0,0);
  delay(4000);

  lcd.print("Farthest: ");
  lcd.setCursor(0, 1);
  lcd.print(farthest);
  lcd.print("cm");
  
  lcd.setCursor(0,0);
  delay(4000);

  lcd.print("Far angle: ");
  lcd.setCursor(0, 1);
  lcd.print(farangle);
  lcd.print(" degrees");
  

  myservo.write(0);
  free(distanceArray);
}

void loop() {
  // nothing
}