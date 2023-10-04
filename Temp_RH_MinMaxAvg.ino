#include <DHT.h> // DHT 라이브러리를 포함

#define DHTPIN 2 // DHT11 센서의 데이터 핀을 2번에 연결
#define DHTTYPE DHT11 // DHT11 센서를 사용

DHT dht(DHTPIN, DHTTYPE); // DHT 객체 생성, 핀 번호와 센서 유형 설정

const int numReadings = 10; // 평균을 계산하기 위한 읽기 횟수
float temperatures[numReadings]; // 온도 값을 저장할 배열
float humidities[numReadings]; // 습도 값을 저장할 배열
int index = 0; // 배열에 값을 저장할 인덱스
float totalTemperature = 0; // 온도 값의 합계
float totalHumidity = 0; // 습도 값의 합계
float minTemperature = 1000; // 초기 최저 온도 값을 큰 값으로 설정
float maxTemperature = -1000; // 초기 최고 온도 값을 작은 값으로 설정
float minHumidity = 1000; // 초기 최저 습도 값을 큰 값으로 설정
float maxHumidity = -1000; // 초기 최고 습도 값을 작은 값으로 설정

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
  dht.begin(); // DHT 센서 초기화
}

void loop() {
  delay(2000); // 2초마다 측정

  float temperature = dht.readTemperature(); // 온도 값을 읽어옴
  float humidity = dht.readHumidity(); // 습도 값을 읽어옴

  if (isnan(temperature) || isnan(humidity)) { // 센서로부터 올바른 값이 읽혔는지 확인
    Serial.println("Failed to read from DHT sensor!"); // 읽기 실패 메시지 출력
    return; // 함수 종료
  }

  totalTemperature = totalTemperature - temperatures[index]; // 이전 값을 합계에서 빼기
  totalHumidity = totalHumidity - humidities[index]; // 이전 값을 합계에서 빼기

  temperatures[index] = temperature; // 새로운 온도 값을 배열에 저장
  humidities[index] = humidity; // 새로운 습도 값을 배열에 저장

  totalTemperature = totalTemperature + temperatures[index]; // 새로운 값을 합계에 더하기
  totalHumidity = totalHumidity + humidities[index]; // 새로운 값을 합계에 더하기

  index = (index + 1) % numReadings; // 다음 값을 저장할 위치 갱신

  float averageTemperature = totalTemperature / numReadings; // 평균 온도 계산
  float averageHumidity = totalHumidity / numReadings; // 평균 습도 계산

  if (temperature < minTemperature) {
    minTemperature = temperature; // 최저 온도 업데이트
  }
  if (temperature > maxTemperature) {
    maxTemperature = temperature; // 최고 온도 업데이트
  }
  if (humidity < minHumidity) {
    minHumidity = humidity; // 최저 습도 업데이트
  }
  if (humidity > maxHumidity) {
    maxHumidity = humidity; // 최고 습도 업데이트
  }

  // 시리얼 모니터에 결과 출력
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
