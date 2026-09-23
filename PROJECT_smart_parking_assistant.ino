#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int echoPin = 3;
int triggerPin = 6;
int buzzPin = 11;

int greenLED = 10;
int yellowLED = 9;
int redLED = 8;

int triggerDelay = 10;
float travelTime = 0;
float vehicleDistance = 0.3;
float targetDistance = 0.3;
int pause = 100;
String goodBuzz = "unbuzzed";

void buzz_tone(int delayHIGH, int delayLOW ){
    digitalWrite(buzzPin, HIGH);
    delay(delayHIGH);
    digitalWrite(buzzPin, LOW);
    delay(delayLOW);
}

void buzz(String vehicleState) {
  if (vehicleState == "Corrrect Spot") {
    for (int j = 0; j < 3; j++ ) {
      buzz_tone(20,80);
    }
  }
  
  if (vehicleState == "Far Away") {
    buzz_tone(500,150);
  }

  if (vehicleState == "Getting Close") {
    buzz_tone(250,100);
  }

  if (vehicleState == "Very Close") {
    buzz_tone(100,0);
  }

  if (vehicleState == "SUPER CLOSE") {
    buzz_tone(20,0);
  }

  if (vehicleState == "Too Close") {
    buzz_tone(500,150);
  }
  if(vehicleState == "Too Far"){
    buzz_tone(0,10);
  }
}

void light_up_LED(String LED){
  if(LED == "red"){
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  if(LED == "yellow"){
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  if(LED == "green"){
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
  if(LED == "off"){
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
}

void activate_distance_sensor() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(triggerDelay);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(triggerDelay);
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(triggerDelay);

    travelTime = pulseIn(echoPin, HIGH);
    delay(25);
  }

void setup() {
  // put your setup code here, to run once:
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(57600);

  lcd.begin(16,2);
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  lcd.print("Testing");
  lcd.setCursor(0,1);
  lcd.print("Test Success");
  
  activate_distance_sensor();

  Serial.println(travelTime);
  vehicleDistance = (330. / 2.) * (travelTime / 1000000.);
  Serial.print("VehicleDistance is : ");
  Serial.print(vehicleDistance);
  Serial.println("metres.");

  if(vehicleDistance >= (targetDistance*3.3)){
    goodBuzz = "unbuzzed";
    light_up_LED("off");
    buzz("Too Far");
  }
  else if(vehicleDistance < (targetDistance*0.17)){
    goodBuzz = "unbuzzed";
    light_up_LED("red");
    buzz("Too Close");
  }
  else if(vehicleDistance < (targetDistance*1.0)){
    light_up_LED("green");
    if (goodBuzz == "unbuzzed") {
      buzz("Corrrect Spot");
      goodBuzz = "buzzed";
    }
  }
  else if(vehicleDistance < (targetDistance*1.3)){
    goodBuzz = "unbuzzed";
    light_up_LED("yellow");
    buzz("SUPER CLOSE");
  }
  else if(vehicleDistance < (targetDistance*1.8)){
    goodBuzz = "unbuzzed";
    light_up_LED("yellow");
    buzz("Very Close");
  }
  else if(vehicleDistance < (targetDistance*2.3)){
    goodBuzz = "unbuzzed";
    light_up_LED("red");
    buzz("Getting Close");
  }
  else if(vehicleDistance < (targetDistance*3.3)){
    goodBuzz = "unbuzzed";
    light_up_LED("red");
    buzz("Far Away");
  }

  lcd.clear();
}
