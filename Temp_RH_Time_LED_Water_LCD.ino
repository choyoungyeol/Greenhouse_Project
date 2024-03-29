#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define Light 26
#define Pump 32
#define WaterPin A8
#define DHTPIN 22
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;
DHT dht(DHTPIN, DHTTYPE);

const int Target_Water_low = 20;
const int Target_Water_high = 23;

int Light_value = 0;
int Pump_value = 0;
int n = 0;
int A = 1;

void setup () {
  pinMode(Light, OUTPUT);
  pinMode(Pump, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  dht.begin();

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
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jeju Nat'l Univ.");
  lcd.setCursor(0, 1);
  lcd.print("Hortic. Sci. Major");
  lcd.setCursor(0, 2);
  lcd.print("Vegetables Lab.");
  lcd.setCursor(0, 3);
  lcd.print("By Young-Yeol CHO");
  delay(5000);
}

void loop () {
  DateTime now = rtc.now();
  int WaterValue = analogRead(WaterPin);
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  int Water = map(WaterValue, 596, 308, 0, 100);
  if (Water >= 100) {
    Water = 100;
  }
  if (Water <= 0) {
    Water = 0;
  }

  if (A == 1) {
    if ((now.hour() > 8) && (now.hour() < 19)) {
      digitalWrite(Light, HIGH);
      Light_value = 1;
    } else {
      digitalWrite(Light, LOW);
      Light_value = 0;
    }

    if (n <= 3) {
      if (Water < Target_Water_low) {
        digitalWrite(Pump, HIGH);
        delay(2000);
        n = n + 1;
        Pump_value = 1;
      }
    } else {
      digitalWrite(Pump, LOW);
      delay(10000);
      n = 0;
      Pump_value = 0;
    }

    if (Water > Target_Water_high) {
      digitalWrite(Pump, LOW);
      n = 0;
      Pump_value = 0;
    }
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
  lcd.setCursor(0, 2);
  lcd.print("Temp : ");
  lcd.print(temp);
  lcd.print("  oC");
  lcd.setCursor(0, 3);
  lcd.print("R H  : ");
  lcd.print(humi);
  lcd.print("  %");

  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water : ");
  lcd.print(Water);
  lcd.print("  %");

  if (Pump_value == HIGH) {
    lcd.setCursor(0, 2);
    lcd.print("Pump ON");
  } else {
    lcd.setCursor(0, 2);
    lcd.print("Pump OFF");
  }

  if (Light_value == 1) {
    lcd.setCursor(0, 3);
    lcd.print("Light ON");
  } else {
    lcd.setCursor(0, 3);
    lcd.print("Light OFF");
  }
  delay(5000);

  if (Serial.available()) {
    delay(3);
    char c = Serial.read();
    if (c == 'x') {
      A = 1;
    }
    if (c == 'y') {
      A = 0;
    }
    if (c == 'a') {
      digitalWrite(Pump, HIGH);
      delay(5000);
      Pump_value = 1;
    }
    if (c == 'b') {
      digitalWrite(Pump, LOW);
      Pump_value = 0;
    }
    if (c == 'c') {
      digitalWrite(Light, HIGH);
      delay(5000);
      Light_value = 1;
    }
    if (c == 'd') {
      digitalWrite(Light, LOW);
      Light_value = 0;
    }
  }

  Serial.print(temp);
  Serial.print(", ");
  Serial.print(humi);
  Serial.print(", ");
  Serial.print(Water);
  Serial.print(", ");
  Serial.print(Pump_value);
  Serial.print(", ");
  Serial.print(Light_value);
  Serial.print(", ");
  Serial.println(A);
}
