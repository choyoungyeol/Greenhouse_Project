#define PhotoPin A2

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int Photo = analogRead(PhotoPin);

  Serial.print("{\"Photo\":");
  Serial.print(Photo);
  Serial.println("}");
  delay(5000);
}
