#include <DHT.h>

#define DHTPIN 2 // DHT11 센서의 데이터 핀을 2번에 연결
#define DHTTYPE DHT11 // DHT11 센서를 사용

DHT dht(DHTPIN, DHTTYPE);

const int numReadings = 10;
float temperatures[numReadings];
float humidities[numReadings];
int index = 0;
float totalTemperature = 0;
float totalHumidity = 0;
float minTemperature = 1000;
float maxTemperature = -1000;
float minHumidity = 1000;
float maxHumidity = -1000;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000); // 2초마다 측정

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  totalTemperature = totalTemperature - temperatures[index];
  totalHumidity = totalHumidity - humidities[index];

  temperatures[index] = temperature;
  humidities[index] = humidity;

  totalTemperature = totalTemperature + temperatures[index];
  totalHumidity = totalHumidity + humidities[index];

  index = (index + 1) % numReadings;

  float averageTemperature = totalTemperature / numReadings;
  float averageHumidity = totalHumidity / numReadings;

  if (temperature < minTemperature) {
    minTemperature = temperature;
  }
  if (temperature > maxTemperature) {
    maxTemperature = temperature;
  }
  if (humidity < minHumidity) {
    minHumidity = humidity;
  }
  if (humidity > maxHumidity) {
    maxHumidity = humidity;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Avg. Temperature: ");
  Serial.print(averageTemperature);
  Serial.print(" °C, Avg. Humidity: ");
  Serial.println(averageHumidity);
  Serial.print("Min Temperature: ");
  Serial.print(minTemperature);
  Serial.print(" °C, Max Temperature: ");
  Serial.print(maxTemperature);
  Serial.print(" °C, Min Humidity: ");
  Serial.print(minHumidity);
  Serial.print(" %, Max Humidity: ");
  Serial.print(maxHumidity);
  Serial.println(" %");

}
