#define WaterPin A0

void setup() {
  Serial.begin(9600);
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
}
