#define RainPin 8

void setup() {
  Serial.begin(9600);
  pinMode(RainPin, INPUT);
}

void loop() {
  int Rain = digitalRead(RainPin);

  if (Rain == 0) {
    Serial.println("Rain = Raining");
  } else {
    Serial.println("Rain = No Raining");
  }
  delay(1000);
}
