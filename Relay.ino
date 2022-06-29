#define Relay1 22
#define Relay2 24
#define Relay3 26
#define Relay4 28

int Relay1Value = 0;
int Relay2Value = 0;
int Relay3Value = 0;
int Relay4Value = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
}

void loop()
{
  if (Serial.available()) {
    delay(3);
    char c = Serial.read();
    if (c == 'a') {
      digitalWrite(Relay1, HIGH);
      Relay1Value = 1;
    }
    if (c == 'b') {
      digitalWrite(Relay1, LOW);
      Relay1Value = 0;
    }
    if (c == 'c') {
      digitalWrite(Relay2, HIGH);
      Relay2Value = 1;
    }
    if (c == 'd') {
      digitalWrite(Relay2, LOW);
      Relay2Value = 0;
    }
    if (c == 'e') {
      digitalWrite(Relay3, HIGH);
      Relay3Value = 1;
    }
    if (c == 'f') {
      digitalWrite(Relay3, LOW);
      Relay3Value = 0;
    }
    if (c == 'g') {
      digitalWrite(Relay4, HIGH);
      Relay4Value = 1;
    }
    if (c == 'h') {
      digitalWrite(Relay4, LOW);
      Relay4Value = 0;
    }
  }
  String data = String(Relay1Value) + "," + String(Relay2Value) + "," + String(Relay3Value) + "," + String(Relay4Value);
  Serial.println(data);
  delay(2000);
}
