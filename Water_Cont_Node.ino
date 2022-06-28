#define WaterPin A0
#define MotorPin 26

const int Target_low = 30;
const int Target_upper = 70;
int Auto = 1;
int PumpValue = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
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

  if (Auto == 1) {
    if (Water < Target_low) {
      digitalWrite(MotorPin, HIGH);
      PumpValue = 1;
    }

    if (Water > Target_upper) {
      digitalWrite(MotorPin, LOW);
      PumpValue = 0;
    }
  }

 if (Serial.available()) {
    delay(3);
    char c = Serial.read();
    if (c == 'x') {
      Auto = 1;
    }
    if (c == 'y') {
      Auto = 0;
    }
    if (c == 'c') {
      digitalWrite(MotorPin, HIGH);
      PumpValue = 1;
    }
    if (c == 'd') {
      digitalWrite(MotorPin, LOW);
      PumpValue = 0;
    }
  }
  
  if (Serial1.available()) {
    delay(3);
    char c = Serial1.read();
    if (c == 'x') {
      Auto = 1;
    }
    if (c == 'y') {
      Auto = 0;
    }
    if (c == 'c') {
      digitalWrite(MotorPin, HIGH);
      PumpValue = 1;
    }
    if (c == 'd') {
      digitalWrite(MotorPin, LOW);
      PumpValue = 0;
    }
  }
  Auto = Auto;
  String data = String(WaterValue) + "," + String(Water) + "," + String(Auto) + "," + String(PumpValue);
  Serial.println(data);
  Serial1.println(data);
  delay(5000);
}
