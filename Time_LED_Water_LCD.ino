#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

#define Light 26
#define Pump 32
#define WaterPin A8

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;

const int Target_Water_low = 20;
const int Target_Water_high = 23;

int Light_value = 0;
int Pump_value = 0;
int n = 0;

void setup () {
  pinMode(Light, OUTPUT);
  pinMode(Pump, OUTPUT);
  Serial.begin(9600);

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

  int Water = map(WaterValue, 596, 308, 0, 100);
  if (Water >= 100) {
    Water = 100;
  }
  if (Water <= 0) {
    Water = 0;
  }
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
    }
  } else {
    digitalWrite(Pump, LOW);
    delay(10000);
    n = 0;
  }

  if (Water > Target_Water_high) {
    digitalWrite(Pump, LOW);
    n = 0;
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
}
