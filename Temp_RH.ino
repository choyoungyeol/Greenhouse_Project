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
  
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" oC, Humidity: ");
  Serial.print(humi);
  Serial.println(" %");
  delay(5000);
}
