#include <SDI12.h>

#define SERIAL_BAUD 9600
#define DATA_PIN 7
#define POWER_PIN 22

SDI12 mySDI12(DATA_PIN);

bool isActive[64] = {0};
uint8_t numSensors = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial);

  mySDI12.begin();
  delay(500);

  if (POWER_PIN > 0) {
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH);
    delay(200);
  }

  for (byte i = 0; i < 62; i++) {
    char addr = decToChar(i);
    if (checkActive(addr)) {
      numSensors++;
      isActive[i] = true;
    }
  }
}

void loop() {
  int n = 0;
  for (byte i = 0; i < 62; i++) {
    char addr = decToChar(i);
    if (isActive[i]) {
      Serial.print(n);
      Serial.print(", ");
      if (getContinuousResults(addr, 2)) {
        float result1 = data1.toFloat();
        float result2 = data2.toFloat();
        // Perform calculation using the results
        float calculatedResult = result1 * result2;  // Replace this with your calculation

        Serial.print(calculatedResult, 2);  // Print the calculated result
      } else {
        Serial.print("Error: Failed to get results");
      }
      Serial.println();
    }
    n++;
  }
  delay(50000);
}

boolean checkActive(char i) {
  String myCommand = "";
  myCommand += (char)i;
  myCommand += "!";

  for (int j = 0; j < 3; j++) {
    mySDI12.sendCommand(myCommand);
    delay(100);
    if (mySDI12.available()) {
      mySDI12.clearBuffer();
      return true;
    }
  }
  mySDI12.clearBuffer();
  return false;
}

void getContinuousResults(char i, int resultsExpected) {
  uint8_t resultsReceived = 0;
  uint8_t cmd_number = 0;
  while (resultsReceived < resultsExpected && cmd_number <= 9) {
    String command = "";
    command = "";
    command += i;
    command += "R";
    command += cmd_number;
    command += "!";

    mySDI12.sendCommand(command);

    uint32_t start = millis();
    while (mySDI12.available() < 3 && (millis() - start) < 1500) {}
    mySDI12.read();

    while (mySDI12.available()) {
      char c = mySDI12.peek();
      if (c == '-' || (c >= '0' && c <= '9') || c == '.') {
        float result = mySDI12.parseFloat(SKIP_NONE);
        Serial.print(String(result, 2));
        if (result != -9999) {
          resultsReceived++;
        }
      }
      else if (c == '+') {
        mySDI12.read();
        if (resultsReceived > 0) {
          Serial.print(", ");
        }
      }
      else {
        mySDI12.read();
      }
      delay(10);
    }
    if (resultsReceived < resultsExpected) {
      Serial.print(", ");
    }
    cmd_number++;
  }
  mySDI12.clearBuffer();
}

char decToChar(byte i) {
  if (i < 10) return i + '0';
  if ((i >= 10) && (i < 36)) return i + 'a' - 10;
  if ((i >= 36) && (i <= 62)) return i + 'A' - 36;
  return i;
}
