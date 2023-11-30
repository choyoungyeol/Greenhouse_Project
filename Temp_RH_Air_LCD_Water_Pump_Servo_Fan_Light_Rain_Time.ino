#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <RTClib.h>

#define DHTPIN 22
#define DHTTYPE DHT22
#define WaterPin A8
#define LightPin A9
#define RainPin 25

#define Light 26
#define Pump 32
#define Fan_in 28
#define Fan_out 30

#define Cooling_FanPin 36
#define CoolingPin 38
#define Heating_FanPin 40
#define HeatingPin 42

#define RSWPin 31
#define LSWPin 33
#define RUWPin 35
#define LUWPin 37

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo RSW;    //Right Side Window
Servo LSW;    //Left Side Window
Servo RUW;    //Right Up Window
Servo LUW;    //Left Up Window
RTC_DS3231 rtc;

const int Target_Water_low = 20;
const int Target_Water_high = 23;

const int Target_Temp_low = 20;
const int Target_Temp_high = 30;

const int Target_Light_low = 250;
const int Target_Light_high = 100;

const int Target_Cooling = 40;
const int Target_Heating = 12;



int RSW_value = 0;
int LSW_value = 0;
int RUW_value = 0;
int LUW_value = 0;
int Windows_value = 0;

int Fan_in_value = 0;
int Fan_out_value = 0;
int Light_value = 0;
int Pump_value = 0;

int Cooling_value = 0;
int Cooling_Fan_value = 0;
int Heating_value = 0;
int Heating_Fan_value = 0;

void setup() {
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

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jeju Nat'l Univ.");
  lcd.setCursor(0, 1);
  lcd.print("Hortic. Sci. Major");
  lcd.setCursor(0, 2);
  lcd.print("Vegetables Lab.");
  lcd.setCursor(0, 3);
  lcd.print("By Cho Young Yeol");
  Serial.begin(9600);
  dht.begin();
  pinMode(Pump, OUTPUT);
  pinMode(Fan_in, OUTPUT);
  pinMode(Fan_out, OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(Cooling_FanPin, OUTPUT);
  pinMode(CoolingPin, OUTPUT);
  pinMode(Heating_FanPin, OUTPUT);
  pinMode(HeatingPin, OUTPUT);
  delay(5000);

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
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float DP = dewPoint(temp, humi);
  float AH =  ((6.112 * exp((17.67 * temp) / (temp + 245.5)) * humi * 18.02) / ((273.15 + temp) * 100 * 0.08314));
  float Psat =  (6.112 * exp((17.67 * temp) / (temp + 243.5))) / 10;
  float P =  (6.112 * exp((17.67 * temp) / (temp + 243.5)) * (humi / 100)) / 10;
  float VPD = (Psat - P);
  int WaterValue = analogRead(WaterPin);
  int Photo = analogRead(LightPin);

  int Water = map(WaterValue, 596, 308, 0, 100);
  if (Water >= 100) {
    Water = 100;
  }
  if (Water <= 0) {
    Water = 0;
  }

  if (Water < Target_Water_low) {
    digitalWrite(Pump, HIGH);
    delay(5000);
  }

  if (Water > Target_Water_high) {
    digitalWrite(Pump, LOW);
  }

  if (temp < Target_Temp_low) {
    RSW.write(0);
    delay(1000);
    LSW.write(180);
    delay(1000);
    RUW.write(0);
    delay(1000);
    LUW.write(180);
    delay(1000);

    Windows_value = 0;
    digitalWrite(Fan_in, LOW);
    Fan_in_value = 0;
    digitalWrite(Fan_out, LOW);
    Fan_out_value = 0;
  }

  if (temp > Target_Temp_high) {
    RSW.write(60);
    delay(1000);
    LSW.write(120);
    delay(1000);
    RUW.write(60);
    delay(1000);
    LUW.write(120);
    delay(1000);
    Windows_value = 1;
    digitalWrite(Fan_in, HIGH);
    Fan_in_value = 1;
    digitalWrite(Fan_out, HIGH);
    Fan_out_value = 0;
  }

  if (Photo > Target_Light_low) {
    digitalWrite(Light, HIGH);
    Light_value = 1;
    delay(5000);
  }

  if (Photo < Target_Light_high) {
    digitalWrite(Light, LOW);
    Light_value = 0;
  }

  if (temp > Target_Cooling) {
    digitalWrite(Heating_FanPin, LOW);
    Heating_Fan_value = 0;
    digitalWrite(HeatingPin, LOW);
    Heating_value = 0;
    digitalWrite(Cooling_FanPin, HIGH);
    Cooling_Fan_value = 1;
    digitalWrite(CoolingPin, HIGH);
    Cooling_value = 1;
  } else {
    digitalWrite(Heating_FanPin, LOW);
    Heating_Fan_value = 0;
    digitalWrite(HeatingPin, LOW);
    Heating_value = 0;
    digitalWrite(Cooling_FanPin, LOW);
    Cooling_Fan_value = 0;
    digitalWrite(CoolingPin, LOW);
    Cooling_value = 0;
  }

  if (temp < Target_Heating) {
    digitalWrite(Heating_FanPin, HIGH);
    Heating_Fan_value = 1;
    digitalWrite(HeatingPin, HIGH);
    Heating_value = 1;
    digitalWrite(Cooling_FanPin, LOW);
    Cooling_Fan_value = 0;
    digitalWrite(CoolingPin, LOW);
    Cooling_value = 0;
  } else {
    digitalWrite(Heating_FanPin, LOW);
    Heating_Fan_value = 0;
    digitalWrite(HeatingPin, LOW);
    Heating_value = 0;
    digitalWrite(Cooling_FanPin, LOW);
    Cooling_Fan_value = 0;
    digitalWrite(CoolingPin, LOW);
    Cooling_value = 0;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.year());
  lcd.print(" / ");
  lcd.print(now.month());
  lcd.print(" / ");
  lcd.print(now.day());
  lcd.setCursor(0, 1);
  lcd.print(now.hour());
  lcd.print(" : ");
  lcd.print(now.minute());
  lcd.print(" : ");
  lcd.print(now.second());
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp : ");
  lcd.print(temp);
  lcd.print("  oC");
  lcd.setCursor(0, 1);
  lcd.print("R H  : ");
  lcd.print(humi);
  lcd.print("  %");
  lcd.setCursor(0, 2);
  lcd.print("A H  : ");
  lcd.print(AH);
  lcd.print("  g/m3");
  lcd.setCursor(0, 3);
  lcd.print("VPD  : ");
  lcd.print(VPD);
  lcd.print("  kPa");

  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water : ");
  lcd.print(Water);
  lcd.print("  %");

  if (Pump_value == HIGH) {
    lcd.setCursor(0, 1);
    lcd.print("Pump ON");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Pump OFF");
  }

  if (Windows_value = 1) {
    lcd.setCursor(0, 2);
    lcd.print("Windows OPEN");
  } else {
    lcd.setCursor(0, 2);
    lcd.print("Windows CLOSED");
  }

  if (VPD >= 0.5 && VPD <= 1.2) {
    lcd.setCursor(0, 3);
    lcd.print("Environment  GOOD");
  } else {
    lcd.setCursor(0, 3);
    lcd.print("Environment  BAD");
  }
  delay(5000);

  if (Fan_in_value == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fan IN ON");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fan IN OFF");
  }
  if (Fan_out_value == 1) {
    lcd.setCursor(0, 1);
    lcd.print("Fan OUT ON");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Fan OUT OFF");
  }
  lcd.setCursor(0, 2);
  lcd.print("Light : ");
  lcd.print(Photo);

  if (Light_value == 1) {
    lcd.setCursor(0, 3);
    lcd.print("Light ON");
  } else {
    lcd.setCursor(0, 3);
    lcd.print("Light OFF");
  }
  delay(5000);

  Serial.print(temp);
  Serial.print(", ");
  Serial.print(humi);
  Serial.print(", ");
  Serial.print(AH);
  Serial.print(", ");
  Serial.print(VPD);
  Serial.print(", ");
  Serial.print(Water);
  Serial.print(", ");
  Serial.print(Photo);
  Serial.print(", ");
  Serial.print(Pump_value);
  Serial.print(", ");
  Serial.print(Windows_value);
  Serial.print(", ");
  Serial.print(Fan_in_value);
  Serial.print(", ");
  Serial.print(Fan_out_value);
  Serial.print(", ");
  Serial.print(Light_value);
  Serial.println("}");
  
//  Serial.print("{\"temp\":");
//  Serial.print(temp);
//  Serial.print(",\"humidity\":");
//  Serial.print(humi);
//  Serial.print(",\"AH\":");
//  Serial.print(AH);
//  Serial.print(",\"VPD\":");
//  Serial.print(VPD);
//  Serial.print(",\"Water\":");
//  Serial.print(Water);
//  Serial.print(",\"Light\":");
//  Serial.print(Photo);
//  Serial.print(",\"Pump_value\":");
//  Serial.print(Pump_value);
//  Serial.print(",\"Windows_value\":");
//  Serial.print(Windows_value);
//  Serial.print(",\"Fan_in_value\":");
//  Serial.print(Fan_in_value);
//  Serial.print(",\"Fan_out_value\":");
//  Serial.print(Fan_out_value);
//  Serial.print(",\"Light_value\":");
//  Serial.print(Light_value);
//  Serial.println("}");
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
