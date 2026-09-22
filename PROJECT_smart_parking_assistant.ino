int echoPin = 3;
int triggerPin = 6;
int buzzPin = 11;

int greenLED = 10;
int yellowLED = 9;
int redLED = 8;

int triggerDelay = 10;
float travelTime = 0;
float vehicleDistance = 0;
int pause = 100;
String goodBuzz = "";

void setup() {
  // put your setup code here, to run once:
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  Serial.begin(57600);
}

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


void loop() {
  // put your main code here, to run repeatedly:
  activate_distance_sensor();

  Serial.println(travelTime);
  vehicleDistance = (330. / 2.) * (travelTime / 1000000.);
  Serial.print("VehicleDistance is : ");
  Serial.print(vehicleDistance);
  Serial.println("metres.");

  if (vehicleDistance <= 0.30  ) {
    light_up_LED("green");
    if (goodBuzz == "unbuzzed") {
      buzz("Corrrect Spot");
      goodBuzz = "buzzed";
    }
  }
  // extra if statement that checks 'goodBuzz' stops continuous buzzing
  // goodBuzz = "buzzed" can be anything, it acts as a simple signifier
  else {
    digitalWrite(greenLED, LOW);
    goodBuzz = "unbuzzed";
  }

  if (vehicleDistance > 0.7 && vehicleDistance < 1.0) {
    light_up_LED("red");
    buzz("Far Away");
  }

  if (vehicleDistance >= 0.54 && vehicleDistance <= 0.7) {
    light_up_LED("red");
    buzz("Getting Close");
  }

  if (vehicleDistance >= 0.39 && vehicleDistance <= 0.55) {
    light_up_LED("yellow");
    buzz("Very Close");
  }

  if (vehicleDistance >= 0.29 && vehicleDistance <= 0.4) {
    light_up_LED("yellow");
    buzz("SUPER CLOSE");
  }

  if (vehicleDistance < 0.05) {
    light_up_LED("red");
    buzz("Too Close");
  }
}
