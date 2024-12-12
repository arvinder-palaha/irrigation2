#define LED_BUILTIN 13
#define PUMP_PIN 2
#define BUTTON_PIN 3
#define MOISTURE_PIN A0
#define READING_INTERVAL_SECONDS 10
#define READING_COUNT 10
#define MOISTURE_THRESHOLD 500
#define PUMP_DURATION_MILLISECONDS 1000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);  // npn transistor, E,B,C is left to right when facing flat side.
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button is active low

  digitalWrite(PUMP_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long lastReadTime = 0;
  static int readingCount = 0;
  static int moistureSum = 0;

  int moisture = analogRead(MOISTURE_PIN);

  Serial.print("moisture reading:");
  Serial.println(moisture);

  // activate pump if button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    activatePump(100);
    return;
  }

  // handle moisture readings every interval
  unsigned long currentTime = millis();
  if (currentTime - lastReadTime >= READING_INTERVAL_SECONDS * 1000) {
    int moisture = analogRead(MOISTURE_PIN);
    moistureSum += moisture;
    readingCount++;

    if (readingCount >= READING_COUNT){
      int averageMoisture = moistureSum / READING_COUNT;
      Serial.print("average moisture:");
      Serial.println(averageMoisture);
      readingCount = 0;
      moistureSum = 0;

      if (averageMoisture < MOISTURE_THRESHOLD) {
        activatePump(PUMP_DURATION_MILLISECONDS);
      }
    }
    lastReadTime = currentTime;
  }

  // digitalWrite(LED_BUILTIN, HIGH);
  // digitalWrite(PUMP_PIN, HIGH);
  // delay(READING_INTERVAL_SECONDS * 1000);

  // digitalWrite(LED_BUILTIN, LOW);
  // digitalWrite(PUMP_PIN, LOW);
  // delay(READING_INTERVAL_SECONDS * 1000);
}

void activatePump(int durationMilliSecond) {
  digitalWrite(PUMP_PIN, HIGH);
  delay(durationMilliSecond);
  digitalWrite(PUMP_PIN, LOW);
}
