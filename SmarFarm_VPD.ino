#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <BH1750.h>
#include <Wire.h>
#include <RTClib.h>

#define Fan_in 28
#define Fan_out 30

#define RSWPin 31
#define LSWPin 33
#define RUWPin 35
#define LUWPin 37

#define DHTPIN 22
#define DHTTYPE DHT22
#define WaterPin A8
#define RainPin 25
#define Light 26

Servo RSW;    //Right Side Window
Servo LSW;    //Left Side Window
Servo RUW;    //Right Up Window
Servo LUW;    //Left Up Window

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BH1750 lightMeter;

RTC_DS3231 rtc;

int RSW_value = 0;
int LSW_value = 0;
int RUW_value = 0;
int LUW_value = 0;

int RainPin_value = 0;
int Fan_in_value = 0;
int Fan_out_value = 0;
int Light_value = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jeju Nat'l Univ.");
  lcd.setCursor(0, 1);
  lcd.print("Hortic. Sci.");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vegetables Lab.");
  lcd.setCursor(0, 1);
  lcd.print("By YoungYeol Cho");
  Serial.begin(9600);
  Wire.begin();
  dht.begin();
  lightMeter.begin();

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
  delay(5000);
  pinMode(RainPin, INPUT);
  pinMode(Fan_in, OUTPUT);
  pinMode(Fan_out, OUTPUT);
  pinMode(Light, OUTPUT);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  float lux = lightMeter.readLightLevel();

  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float DP = dewPoint(temp, humi);
  float AH =  ((6.112 * exp((17.67 * temp) / (temp + 245.5)) * humi * 18.02) / ((273.15 + temp) * 100 * 0.08314));
  float Psat =  (6.112 * exp((17.67 * temp) / (temp + 243.5))) / 10;
  float P =  (6.112 * exp((17.67 * temp) / (temp + 243.5)) * (humi / 100)) / 10;
  float VPD = (Psat - P);
  int WaterValue = analogRead(WaterPin);
  int Water = map(WaterValue, 522, 275, 0, 100);
  Water = constrain(Water, 0, 100);
  RainPin_value = digitalRead(RainPin);

  if ((now.hour() > 7) && (now.hour() < 19 )) {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Temp : ");
    lcd.print(temp);
    lcd.print("  oC");
    lcd.setCursor(0, 1);
    lcd.print("R H  : ");
    lcd.print(humi);
    lcd.print("  %");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A H  : ");
    lcd.print(AH);
    lcd.print("  g/m3");
    lcd.setCursor(0, 1);
    lcd.print("VPD  : ");
    lcd.print(VPD);
    lcd.print("  kPa");
    delay(5000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water : ");
    lcd.print(Water);
    lcd.print("  %");
    if (VPD >= 0.5 && VPD <= 1.2) {
      lcd.setCursor(0, 1);
      lcd.print("Environ. GOOD");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Environ. BAD");
    }

    lcd.clear();
    if (RainPin_value == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Raining");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("No Rain");
    }
    lcd.setCursor(0, 1);
    lcd.print("Lux : ");
    lcd.print(lux);
    lcd.print(" lx");

    delay(5000);

    if (RainPin_value == 1) {
      if (temp <= 10) {
        RSW.write(0);
        delay(1000);
        LSW.write(180);
        delay(1000);
        RUW.write(0);
        delay(1000);
        LUW.write(180);
        delay(1000);
        digitalWrite(Fan_in, LOW);
        Fan_in_value = 0;
        digitalWrite(Fan_out, LOW);
        Fan_out_value = 0;
      }

      if (temp >= 45)  {
        RSW.write(60);
        delay(1000);
        LSW.write(120);
        delay(1000);
        RUW.write(60);
        delay(1000);
        LUW.write(120);
        delay(1000);
        digitalWrite(Fan_in, HIGH);
        Fan_in_value = 1;
        digitalWrite(Fan_out, HIGH);
        Fan_out_value = 1;
      }

      if ((VPD > 0.4) && (VPD <= 1.4)) {
        RSW.write(0);
        delay(1000);
        LSW.write(180);
        delay(1000);
        RUW.write(0);
        delay(1000);
        LUW.write(180);
        delay(1000);
        digitalWrite(Fan_in, LOW);
        Fan_in_value = 0;
        digitalWrite(Fan_out, LOW);
        Fan_out_value = 0;
      } else {
        RSW.write(60);
        delay(1000);
        LSW.write(120);
        delay(1000);
        RUW.write(60);
        delay(1000);
        LUW.write(120);
        delay(1000);
        digitalWrite(Fan_in, HIGH);
        Fan_in_value = 1;
        digitalWrite(Fan_out, HIGH);
        Fan_out_value = 1;
      }
    } else {   //rain
      if (temp <= 10) {
        RSW.write(0);
        delay(1000);
        LSW.write(180);
        delay(1000);
        RUW.write(0);
        delay(1000);
        LUW.write(180);
        delay(1000);
        digitalWrite(Fan_in, LOW);
        Fan_in_value = 0;
        digitalWrite(Fan_out, LOW);
        Fan_out_value = 0;
      }

      if (temp >= 45)  {
        RSW.write(60);
        delay(1000);
        LSW.write(120);
        delay(1000);
        RUW.write(0);
        delay(1000);
        LUW.write(180);
        delay(1000);
        digitalWrite(Fan_in, HIGH);
        Fan_in_value = 1;
        digitalWrite(Fan_out, HIGH);
        Fan_out_value = 1;
      }

      if ((VPD > 0.4) && (VPD <= 1.4)) {
        RSW.write(0);
        delay(1000);
        LSW.write(180);
        delay(1000);
        RUW.write(0);
        delay(1000);
        LUW.write(180);
        delay(1000);
        digitalWrite(Fan_in, LOW);
        Fan_in_value = 0;
        digitalWrite(Fan_out, LOW);
        Fan_out_value = 0;
      } else {
        RSW.write(60);
        delay(1000);
        LSW.write(120);
        delay(1000);
        RUW.write(0);
        delay(1000);
        LUW.write(180);
        delay(1000);
        digitalWrite(Fan_in, HIGH);
        Fan_in_value = 1;
        digitalWrite(Fan_out, HIGH);
        Fan_out_value = 1;
      }
    }

    if (lux < 100) {
      digitalWrite(Light, HIGH);
    }
    if (lux >= 300) {
      digitalWrite(Light, LOW);
    }
    delay(5000);

  } else {
    lcd.noBacklight();
  }
  String data = String(lux) + ", " + String(temp) + ", " + String(humi) + ", " + String(VPD) + ", " + String(Water) + ", " + String(RainPin_value) + ", " + String(Fan_out_value);
  Serial.println(data);
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
