//아두이노 코드
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "RTClib.h"
#include <Servo.h>

#define PHOTO A0
#define Water_Pin A1
#define DHTPIN 12
#define DHTTYPE DHT11
#define servoPin 9
#define Fan 32
#define Light 4
#define Pump 31

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
Servo myservo;

int Photo_Value = 0;
int pos = 0;
int Auto = 1;
int LightValue = 0;
int FanValue = 0;
int PumpValue = 0;
int ServoValue = 0;

char c;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("JNU SmartFarm");
  lcd.setCursor(0, 1);
  lcd.print("Jeju Nat'l Univ.");
  delay(5000);
  myservo.attach(servoPin);
  myservo.write(0);
  pinMode(Fan, OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(Pump, OUTPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  DateTime now = rtc.now();
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();
  Photo_Value = analogRead(PHOTO);
  float pv_value = float(Photo_Value * 5) / 1023;
  float Rp = (10 * pv_value) / (5 - pv_value);
  float y = (log10(200 / Rp)) / 0.7;
  float Ea = pow(10, y);

  int Water = analogRead(Water_Pin);
  int Water_Content = map(Water, 518, 272, 0, 100);

  if (Water_Content >= 100) {
    Water_Content = 100;
  }
  if (Water_Content <= 0) {
    Water_Content = 0;
  }

  if (Auto == 1) {
    if ((now.hour() >= 6) && (now.hour() <= 18)) {
      if (Ea <= 5000) {
        digitalWrite(Light, HIGH);
        LightValue = 1;
      }
      if (Ea > 6000) {
        digitalWrite(Light, LOW);
        LightValue = 0;
      }

      if (temperature >= 30) {
        digitalWrite(Fan, HIGH);
        myservo.write(90);
        FanValue = 1;
        ServoValue = 1;
      }

      if ((temperature < 30) && (temperature >= 20))  {
        digitalWrite(Fan, LOW);
        myservo.write(0);
        FanValue = 0;
        ServoValue = 0;
      }
      
      if (temperature < 20) {
        digitalWrite(Fan, LOW);
        myservo.write(0);
        FanValue = 0;
        ServoValue = 0;
      }
    } else {
      digitalWrite(Light, LOW);
      LightValue = 0;
      if (temperature >= 30) {
        digitalWrite(Fan, HIGH);
        myservo.write(90);
        FanValue = 1;
        ServoValue = 1;
      }
      if ((temperature < 30) && (temperature >= 20))  {
        digitalWrite(Fan, LOW);
        myservo.write(0);
        FanValue = 0;
        ServoValue = 0;
      }
      if (temperature < 20) {
        digitalWrite(Fan, LOW);
        myservo.write(0);
        FanValue = 0;
        ServoValue = 0;
      }
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
    if (c == 'a') {
      digitalWrite(Light, HIGH);
      LightValue = 1;
    }
    if (c == 'b') {
      digitalWrite(Light, LOW);
      LightValue = 0;
    }
    if (c == 'c') {
      digitalWrite(Pump, HIGH);
      PumpValue = 1;
    }
    if (c == 'd') {
      digitalWrite(Pump, LOW);
      PumpValue = 0;
    }
    if (c == 'e') {
      myservo.write(90);
      ServoValue = 1;
    }
    if (c == 'f') {
      myservo.write(0);
      ServoValue = 0;
    }
    if (c == 'g') {
      digitalWrite(Fan, HIGH);
      FanValue = 1;
    }
    if (c == 'h') {
      digitalWrite(Fan, LOW);
      FanValue = 0;
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
    if (c == 'a') {
      digitalWrite(Light, HIGH);
      LightValue = 1;
    }
    if (c == 'b') {
      digitalWrite(Light, LOW);
      LightValue = 0;
    }
    if (c == 'c') {
      digitalWrite(Pump, HIGH);
      PumpValue = 1;
    }
    if (c == 'd') {
      digitalWrite(Pump, LOW);
      PumpValue = 0;
    }
    if (c == 'e') {
      myservo.write(90);
      ServoValue = 1;
    }
    if (c == 'f') {
      myservo.write(0);
      ServoValue = 0;
    }
    if (c == 'g') {
      digitalWrite(Fan, HIGH);
      FanValue = 1;
    }
    if (c == 'h') {
      digitalWrite(Fan, LOW);
      FanValue = 0;
    }
  }

  if ((now.hour() % 2 == 0))  {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(now.year());
    lcd.print("/");
    lcd.print(now.month());
    lcd.print("/");
    lcd.print(now.day());
    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(" hour: ");
    lcd.print(now.minute());
    lcd.print(" min");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lux= ");
    lcd.print(Ea);
    lcd.setCursor(0, 1);
    lcd.print("Water= ");
    lcd.print(Water_Content);
    lcd.print(" %");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp= ");
    lcd.print(temperature);
    lcd.print(" oC");
    lcd.setCursor(0, 1);
    lcd.print("R  H= ");
    lcd.print(humidity);
    lcd.print(" %");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode = ");
    lcd.print(Auto);
    delay(2000);
  } else {
    lcd.noBacklight();
  }

  Auto = Auto;
  String data = String(Ea) + "," + String(Water_Content) + "," + String(temperature) + "," + String(humidity) + "," + String(Auto) + "," + String(LightValue) + "," + String(FanValue) + "," + String(PumpValue) + "," + String(ServoValue);
  Serial.println(data);
  Serial1.println(data);
  delay(1000);
}
