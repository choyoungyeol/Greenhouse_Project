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

int LowVPD = 0.4;
int HighVPD = 1.4;

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
  float DP = dewPoint(temp, humi);
  float AH =  ((6.112 * exp((17.67 * temp) / (temp + 245.5)) * humi * 18.02) / ((273.15 + temp) * 100 * 0.08314));
  float Psat =  (6.112 * exp((17.67 * temp) / (temp + 243.5))) / 10;
  float P =  (6.112 * exp((17.67 * temp) / (temp + 243.5)) * (humi / 100)) / 10;
  float VPD = (Psat - P);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" oC, Humidity: ");
  Serial.print(humi);
  Serial.println(" %");
  delay(5000);

  if (VPD < LowVPD) {
    RSW.write(0);
    delay(1000);
    LSW.write(180);
    delay(1000);
    RUW.write(0);
    delay(1000);
    LUW.write(180);
    delay(1000);
  }
  if (VPD > HighVPD) {
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

float dewPoint(float temp, float humi) {
  float RATIO = 373.15 / (273.15 + temp);
  float RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);
  float VP = pow(10, RHS - 3) * humi;
  float T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}
