#define WaterPin A0
#define MotorPin 26

const int Target_low = 30;
const int Target_upper = 70;

void setup() {
  Serial.begin(9600);
  pinMode(MotorPin, OUTPUT);
}

void loop() {
  int WaterValue = analogRead(WaterPin);
  int Water = map(WaterValue, 1023, 317, 0, 100);
  if (Water >= 100) {
    Water = 100;
  }
  if (Water <= 0) {
    Water = 0;
  }
  Serial.print("Water value = ");
  Serial.print(WaterValue);
  Serial.print(", Water = ");
  Serial.print(Water);
  Serial.println(" %");
  delay(5000);

  if (Water < Target_low) {
    digitalWrite(MotorPin, HIGH);
    delay(10000);
  }

  if (Water > Target_upper) {
    digitalWrite(MotorPin, LOW);
  }
}
