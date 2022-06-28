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
  
  Serial.print(temp);
  Serial.print(" ,");
  Serial.println(humi);
  delay(5000);
}
