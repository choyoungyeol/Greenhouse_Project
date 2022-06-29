#include "DHT.h"
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define RSWPin 4
#define LSWPin 5
#define RUWPin 6
#define LUWPin 7

int RSW_value = 0;
int LSW_value = 0;
int RUW_value = 0;
int LUW_value = 0;

int LowTemp = 20;
int HighTemp = 28;

Servo RSW;    //Right Side Window
Servo LSW;    //Left Side Window
Servo RUW;    //Right Up Window
Servo LUW;    //Left Up Window
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

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
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" oC, Humidity: ");
  Serial.print(humi);
  Serial.println(" %");
  delay(5000);

  if (temp < LowTemp) {
    RSW.write(0);
    delay(1000);
    LSW.write(180);
    delay(1000);
    RUW.write(0);
    delay(1000);
    LUW.write(180);
    delay(1000);
  }
  if (temp > HighTemp) {
    RSW.write(60);
    delay(1000);
    LSW.write(130);
    delay(1000);
    RUW.write(60);
    delay(1000);
    LUW.write(130);
    delay(1000);
  }
}
