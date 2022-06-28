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

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" oC, Humidity: ");
  Serial.print(humi);
  Serial.println(" %");
  Serial.print("Dew-point Temperature: ");
  Serial.print(DP);
  Serial.print(" oC, Absolute Humidity: ");
  Serial.print(AH);
  Serial.println(" g/m3");
  Serial.print("Vapor Pressure: ");
  Serial.print(P);
  Serial.print(" kPa, Vapor Pressure Deficit: ");
  Serial.print(VPD);
  Serial.println(" kPa");
  delay(5000);
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
