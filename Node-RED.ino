#include <LoRa.h>
#include <Sensirion.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const uint8_t dataPin  =  3;
const uint8_t clockPin =  4;

float temperature;
float humidity;
float dewpoint;

Sensirion tempSensor = Sensirion(dataPin, clockPin);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

String outString = "";

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(868E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Jeju Nat'l Univ.");
  lcd.setCursor(0, 1);
  lcd.print("Vegetables Lab.");
}

void loop()
{
  tempSensor.measure(&temperature, &humidity, &dewpoint);

  //float DP = dewpoint(temperature, humidity);  //노점온도
  float AH =  ((6.112 * exp((17.67 * temperature) / (temperature + 245.5)) * humidity * 18.02) / ((273.15 + temperature) * 100 * 0.08314));   //절대습도
  float Psat =  (6.112 * exp((17.67 * temperature) / (temperature + 243.5))) / 10;  //포화수증기압 (단위 : kPa)
  float P =  (6.112 * exp((17.67 * temperature) / (temperature + 243.5)) * (humidity / 100)) / 10;  //수증기압 (단위 : kPa)
  float VPD = (Psat - P); //수증기압차 (단위 : kPa)

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Dewpoint: ");
  Serial.print(dewpoint);
  Serial.println(" C");
  Serial.print("Absolute Humidity: ");
  Serial.print(AH);
  Serial.print(" g/m3, VPD: ");
  Serial.print(VPD);
  Serial.println(" kPa");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(temperature);
  lcd.print(" oC");
  lcd.setCursor(0, 1);
  lcd.print("Humi = ");
  lcd.print(humidity);
  lcd.print(" %");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A H = ");
  lcd.print(AH);
  lcd.print(" g/m3");
  lcd.setCursor(0, 1);
  lcd.print("VPD = ");
  lcd.print(VPD);
  lcd.print(" kPa");
  delay(2000);
  outString = "{\"Temperature\":" + String(temperature) + ", \"Humidity\":" + String(humidity) + ", \"Dewpoint temp\":" + String(dewpoint) + "}";

  LoRa.beginPacket();
  LoRa.print(outString);
  LoRa.endPacket();
  delay(50);
}
