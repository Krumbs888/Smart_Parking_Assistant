int pushButtonPin = 13;
int potReadPin = A0;
int greenPin = 11;

int potReading = 0;
float targetDistance = 0;
int newButtonValue = 0;
int oldButtonValue = 0;

String calibrationState = "untoggled";


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

  targetDistance = ((0.85 / 1023) * potReading) + 0.15;
  Serial.print("Target Distance is now ");
  Serial.print(targetDistance);
  Serial.println("m.");
  Serial.println();
}

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

void setup() {
  // put your setup code here, to run once:
  pinMode(pushButtonPin, OUTPUT);
  pinMode(potReadPin, INPUT);
  pinMode(greenPin, OUTPUT);

  digitalWrite(pushButtonPin, HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  check_calibration_state();

  while (calibrationState == "untoggled") {
    read_pot_value();
    digitalWrite(greenPin, LOW);
    check_calibration_state();
  }
  while(calibrationState == "toggled"){
    digitalWrite(greenPin, HIGH);
    check_calibration_state();
  }
}
