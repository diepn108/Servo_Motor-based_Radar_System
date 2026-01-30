#include <Servo.h>
#include <LiquidCrystal.h>

Servo myservo;
const int rs = 10, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int trigPin = 7;
const int echoPin = 6; 

int scanRange;
int readings;

void setup() {
  myservo.attach(11);
  myservo.write(0);
  lcd.begin(16, 2);
  lcd.print("See monitor");

  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
  Serial.begin(9600);

  Serial.println(" ");
  Serial.println("Enter degrees (1-180):");
  while (Serial.available() == 0) {}
  scanRange = Serial.parseInt();
  while (Serial.available()) Serial.read();  

  Serial.println("Enter number of readings (2-100):");
  while (Serial.available() == 0) {}
  readings = Serial.parseInt();
  while (Serial.available()) Serial.read();  

  Serial.print("Degrees entered: ");
  Serial.println(scanRange);
  Serial.print("Readings entered: ");
  Serial.println(readings);
  
  lcd.clear();
  lcd.print("Starting scan...");
  delay(1500);
}

void loop() {
  float stepSize = scanRange / float(readings - 1);
  float duration, distance;
  float angle;

  float farthest = 0;
  float closest = 10000;
  float farangle = 0, closeangle = 0;

  for (int i = 0; i < readings; i++) {
    angle = i * stepSize;
    myservo.write(angle);
    delay(10000 / readings);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH); 
    distance = (duration * 0.0343) / 2;  
    
    // Ignore far distances
    if (distance > 150) {
      distance = 0;
    }

    // Live LCD update
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Angle: ");
    lcd.print(angle, 0);

    lcd.setCursor(0, 1);
    if (distance == 0 || distance > 200) {
      lcd.print("Out of range");
    } else {
      lcd.print("Dist: ");
      lcd.print(distance, 1);
      lcd.print(" cm");
    }

    // Keep track of closest and farthest
    if (distance > farthest) {
      farthest = distance;
      farangle = angle;
    }
    if (distance != 0 && distance < closest) {
      closest = distance;
      closeangle = angle;
    }

    Serial.print(distance);
    Serial.print(" cm | ");
    Serial.print(angle);
    Serial.println(" degrees");

    delay(250); // Short delay for display readability
  }

  // Show summary on LCD
  lcd.clear();
  lcd.print("Closest: ");
  lcd.setCursor(0, 1);
  lcd.print(closest);
  lcd.print(" cm");
  delay(3000);

  lcd.clear();
  lcd.print("Close angle:");
  lcd.setCursor(0, 1);
  lcd.print(closeangle);
  lcd.print(" deg");
  delay(3000);

  lcd.clear();
  lcd.print("Farthest:");
  lcd.setCursor(0, 1);
  lcd.print(farthest);
  lcd.print(" cm");
  delay(3000);

  lcd.clear();
  lcd.print("Far angle:");
  lcd.setCursor(0, 1);
  lcd.print(farangle);
  lcd.print(" deg");
  delay(3000);

  myservo.write(0);
}