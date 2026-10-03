#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//================================================================================================================================================================================================
// HARDWWARE PIN CONNECTIONS
//================================================================================================================================================================================================
int echoPin = 3;
int triggerPin = 6;
int buzzPin = 11;

int greenLED = 10;
int yellowLED = 9;
int redLED = 8;

int pushButtonPin = 13;
int potReadPin = A0;

//================================================================================================================================================================================================
// DELAYS
//================================================================================================================================================================================================
int triggerDelay = 10;
int pause = 100;

//================================================================================================================================================================================================
// DISTANCE RELATED VARIABLES
//================================================================================================================================================================================================
float travelTime = 0;
float vehicleDistance = 0;
float vehicleDistanceHistory[3];
float targetDistance = 0.3;
float distanceJumpThreshold = 0.3;
//================================================================================================================================================================================================
// ACTION PARAMETERS/ARGUMENTS
//================================================================================================================================================================================================
String goodBuzz = "unbuzzed";
String objectState = "";

//=====================================================================================================================================================================================
// CALIBRATION VARIABLES
//=====================================================================================================================================================================================
int potReading = 0;

//=====================================================================================================================================================================================
// TOGGLE PARAMETERS/ARGUMENTS (is this the right use of parameters and arguments?)
//=====================================================================================================================================================================================
int newButtonValue = 0;
int oldButtonValue = 0;
String calibrationState = "toggled";
//program says calibrationState starts as "toggled"
//but on my arduino it actually switches to the exact opposite
//of what it is initially set as (i.e in this case its actually "untoggled")

//================================================================================================================================================================================================
// BUZZER LOGIC
//================================================================================================================================================================================================
void buzz_tone(int delayHIGH, int delayLOW ) {
  digitalWrite(buzzPin, HIGH);
  delay(delayHIGH);
  digitalWrite(buzzPin, LOW);
  delay(delayLOW);
}

void buzz(String vehicleState) {
  objectState = vehicleState;

  if (vehicleState == "Correct Spot") {
    for (int j = 0; j < 3; j++ ) {
      buzz_tone(20, 80);
    }
    return 0;
  }

  if (vehicleState == "Far Away") {
    buzz_tone(500, 150);
    return 0;
  }

  if (vehicleState == "Getting Close") {
    buzz_tone(250, 100);
    return 0;
  }

  if (vehicleState == "Very Close") {
    buzz_tone(100, 0);
    return 0;
  }

  if (vehicleState == "SUPER CLOSE") {
    buzz_tone(20, 0);
    return 0;
  }

  if (vehicleState == "Too Close") {
    buzz_tone(500, 150);
    return 0;
  }
  if (vehicleState == "Too Far") {
    buzz_tone(0, 10);
    return 0;
  }
}

//================================================================================================================================================================================================
// LED LOGIC
//================================================================================================================================================================================================
void light_up_LED(String LED) {
  if (LED == "red") {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
  if (LED == "yellow") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  }
  if (LED == "green") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  }
  if (LED == "off") {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }
}

//================================================================================================================================================================================================
// DISTANCE MEASUREMENT
//================================================================================================================================================================================================
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

void measure_vehicle_distance() {
  vehicleDistanceHistory[2] = vehicleDistanceHistory[1];
  vehicleDistanceHistory[1] = vehicleDistanceHistory[0];
  vehicleDistanceHistory[0] = vehicleDistance;
  
  Serial.println(travelTime);
  vehicleDistance = (330. / 2.) * (travelTime / 1000000.);
  Serial.print("VehicleDistance is : ");
  Serial.print(vehicleDistance);
  Serial.println("metres.");
}

void confirm_vehicle_distance() {
  if (vehicleDistance >= (vehicleDistanceHistory[0] * 1.3)
      && vehicleDistance >= (vehicleDistanceHistory[1] * 1.3)
      && vehicleDistance >= (vehicleDistanceHistory[2] * 1.3)) {
    delay(50);
    activate_distance_sensor();
    measure_vehicle_distance();
    display_vehicle_distance();
  }
}

//================================================================================================================================================================================================
// DISTANCE DISPLAY
//================================================================================================================================================================================================
void display_vehicle_distance() {
  if (!(vehicleDistance == vehicleDistance)) {
    lcd.clear();
  }
  if ((objectState == objectState)) {
    lcd.clear();
    //this lcd.clear only works when using the current condition
    //(objectState == objectState), but not with !(objectState == objectState)
    //it is also the cause of the LCD 'choppy-ness'
  }
  lcd.setCursor(0, 0);
  lcd.print("objDist ");
  lcd.print("= ");
  lcd.print(vehicleDistance);
  lcd.setCursor(0, 1);
  lcd.print(objectState);
}

//================================================================================================================================================================================================
// MISC/DECISION MAKING
//================================================================================================================================================================================================
bool sudden_jump_detected() {
  float average = (vehicleDistanceHistory[0]
                   + vehicleDistanceHistory[1]
                   + vehicleDistanceHistory[2]) / 3.;
  float PercentageChange = fabs((vehicleDistance - average)/average);
  if (PercentageChange >= distanceJumpThreshold) {
    return 1;
  }
  else {
    return 0;
  }
}

void indicate_vehicle_distance() {
  if (sudden_jump_detected()== 1) {
    delay(10);
    activate_distance_sensor();
    measure_vehicle_distance();
    display_vehicle_distance();
    if(sudden_jump_detected()==1){
    activate_distance_sensor();
    measure_vehicle_distance();
    display_vehicle_distance();
    }
  }
  // made the above delay 10 millisecs because it takes the sensor about 30millisecs
  // to register a new reading and anything longer might have significant effects on
  // the buzzer delays.
  //even 10 millisecs is doing a lot
  else {
    if (vehicleDistance >= (targetDistance * 3.3)) {
      goodBuzz = "unbuzzed";
      light_up_LED("off");
      buzz("Too Far");
    }
    else if (vehicleDistance < (targetDistance * 0.17)) {
      goodBuzz = "unbuzzed";
      light_up_LED("red");
      buzz("Too Close");
    }
    else if (vehicleDistance < (targetDistance * 1.0)) {
      light_up_LED("green");
      if (goodBuzz == "unbuzzed") {
        buzz("Correct Spot");
        goodBuzz = "buzzed";
      }
    }
    else if (vehicleDistance < (targetDistance * 1.3)) {
      goodBuzz = "unbuzzed";
      light_up_LED("yellow");
      buzz("SUPER CLOSE");
    }
    else if (vehicleDistance < (targetDistance * 1.8)) {
      goodBuzz = "unbuzzed";
      light_up_LED("yellow");
      buzz("Very Close");
    }
    else if (vehicleDistance < (targetDistance * 2.3)) {
      goodBuzz = "unbuzzed";
      light_up_LED("red");
      buzz("Getting Close");
    }
    else if (vehicleDistance < (targetDistance * 3.3)) {
      goodBuzz = "unbuzzed";
      light_up_LED("red");
      buzz("Far Away");
    }
  }
  //an ascending chain of else if statements seems to be the best way to
  //measure a range of overlapping values
}

//=====================================================================================================================================================================================
// CALIBRATION VALUE READING/MEASUREMENT 
//=====================================================================================================================================================================================
void read_button_value() {
  oldButtonValue = newButtonValue;
  newButtonValue = digitalRead(pushButtonPin);
  Serial.print("button value is ");
  Serial.println(newButtonValue);
}
void read_pot_value() {
  potReading = analogRead(potReadPin);
  Serial.print("Potentiometer Reading is ");
  Serial.println(potReading);
}

void calculate_targetDistance(){
  targetDistance = ((0.85 / 1023) * potReading) + 0.15;
  Serial.print("Target Distance is now ");
  Serial.print(targetDistance);
  Serial.println("m.");
  Serial.println();
  //lower limit is 0.15m, any lower and sensor might bug out
  //upper limit is 1m, any higher and sensor WILL bug out
}

//=====================================================================================================================================================================================
// TOGGLE DECISION MAKING
//=====================================================================================================================================================================================
bool button_is_toggled() {
  read_button_value();
  if (oldButtonValue == 0 && newButtonValue == 1) {
    delay(30);
    return 1;
  }
  else {
    return 0;
  }
}

void check_calibration_state(){
  if (button_is_toggled() == 1) {
    if(calibrationState ==  "toggled"){
      calibrationState = "untoggled";
    }
    else{
      calibrationState = "toggled";
    }
  }
}

//================================================================================================================================================================================================
// SETUP
//================================================================================================================================================================================================
void setup() {
  // put your setup code here, to run once:
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);

  pinMode(buzzPin, OUTPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(pushButtonPin, INPUT);
  pinMode(potReadPin, INPUT);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();
}

//================================================================================================================================================================================================
// LOOP
//================================================================================================================================================================================================
void loop() {
  // put your main code here, to run repeatedly:
  check_calibration_state();

  if(calibrationState == "untoggled"){
  activate_distance_sensor();
  measure_vehicle_distance();
  display_vehicle_distance();
  indicate_vehicle_distance();

  check_calibration_state();
  }
  if(calibrationState == "toggled"){
    for(int i = 0; i < 1; i++){
      while(i == 0){


        read_pot_value();
        if(potReading == 0){
          i++;
        }
        if(potReading == 1023){
          calibrationState == "untoggled";
        }
      }
    }

    while(calibrationState == "toggled"){
      read_pot_value();
      calculate_targetDistance();
      check_calibration_state();
    }
  }

}
