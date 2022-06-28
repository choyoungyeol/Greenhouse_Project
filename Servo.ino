#include <Servo.h>

#define RSWPin 4
#define LSWPin 5
#define RUWPin 6
#define LUWPin 7

int RSW_value = 0;
int LSW_value = 0;
int RUW_value = 0;
int LUW_value = 0;

Servo RSW;    //Right Side Window
Servo LSW;    //Left Side Window
Servo RUW;    //Right Up Window
Servo LUW;    //Left Up Window

void setup() {
  Serial.begin(9600);

  RSW.attach(RSWPin);
  LSW.attach(LSWPin);
  RUW.attach(RUWPin);
  LUW.attach(LUWPin);

  RSW.write(0);
  delay(1000);
  LSW.write(180);
  delay(1000);
  RUW.write(0);
  delay(1000);
  LUW.write(180);
  delay(1000);
}

void loop() {
  RSW.write(0);
  delay(1000);
  LSW.write(180);
  delay(1000);
  RUW.write(0);
  delay(1000);
  LUW.write(180);
  delay(1000);
  RSW.write(60);
  delay(1000);
  LSW.write(130);
  delay(1000);
  RUW.write(60);
  delay(1000);
  LUW.write(130);
  delay(1000);
}
