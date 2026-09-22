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
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
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
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    buzz("Far Away");
  }

  if (vehicleDistance >= 0.54 && vehicleDistance <= 0.7) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    buzz("Getting Close");
  }

  if (vehicleDistance >= 0.39 && vehicleDistance <= 0.55) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    buzz("Very Close");
  }

  if (vehicleDistance >= 0.29 && vehicleDistance <= 0.4) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    buzz("SUPER CLOSE");
  }

  if (vehicleDistance < 0.05) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    buzz("Too Close");
  }
}
