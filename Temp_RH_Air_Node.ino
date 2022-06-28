#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(300);
}

void loop() {
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float DP = dewPoint(temp, humi);
  float AH =  ((6.112 * exp((17.67 * temp) / (temp + 245.5)) * humi * 18.02) / ((273.15 + temp) * 100 * 0.08314));
  float Psat =  (6.112 * exp((17.67 * temp) / (temp + 243.5))) / 10;
  float P =  (6.112 * exp((17.67 * temp) / (temp + 243.5)) * (humi / 100)) / 10;
  float VPD = (Psat - P);

  Serial.print(temp);
  Serial.print(", ");
  Serial.print(humi);
  Serial.print(", ");
  Serial.print(DP);
  Serial.print(", ");
  Serial.print(AH);
  Serial.print(", ");
  Serial.print(P);
  Serial.print(", ");
  Serial.println(VPD);
  delay(5000);
}
