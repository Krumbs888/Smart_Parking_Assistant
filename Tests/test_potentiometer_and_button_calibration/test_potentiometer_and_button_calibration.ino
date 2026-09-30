int potVoltPin = 12;
int pushButtonPin = 13;
int potReadPin = A0;
float potValue = 0;
int potReading = 0;
int buttonValue = 0;
float targetDistance = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(potVoltPin, OUTPUT);
  pinMode(pushButtonPin, OUTPUT);
  pinMode(potReadPin, INPUT);

  digitalWrite(potVoltPin,HIGH);
  digitalWrite(pushButtonPin,HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  potReading = analogRead(potReadPin);
  
  targetDistance = ((0.85/1023)*potReading) + 0.15;
  Serial.print("Target Distance is now ");
  Serial.print(targetDistance);
  Serial.println("m.");

  Serial.print("Potentiometer Reading is ");
  Serial.println(potReading);





  buttonValue = digitalRead(pushButtonPin);
  Serial.print("button value is ");
  Serial.println(buttonValue);
  Serial.println();

  delay(400);
}
