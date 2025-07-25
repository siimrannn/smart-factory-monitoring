#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TEMP_PIN A0
#define GAS_PIN A1
#define PIR_PIN 3
#define BUZZER_PIN 6
#define LED_PIN 7

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address 0x27, 16 columns, 2 rows

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600); // Start serial for dashboard integration

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Factory");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature from LM35
  int tempValue = analogRead(TEMP_PIN);
  float voltage = tempValue * (5.0 / 1023.0);
  float temperature = voltage * 100.0; // 10mV per °C

  // Read gas value from MQ-2
  int gasValue = analogRead(GAS_PIN);

  // Read motion from PIR sensor
  int motion = digitalRead(PIR_PIN);

  // Display temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print(" C ");

  // Display gas and alert if needed
  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print(" ");

  if (motion == HIGH || gasValue > 400) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    lcd.setCursor(10, 1);
    lcd.print("ALERT");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    lcd.setCursor(10, 1);
    lcd.print("     "); // Clear alert text
  }

  // 🔁 Send data to serial monitor as JSON
  Serial.print("{\"temperature\":");
  Serial.print(temperature, 1);
  Serial.print(",\"gas\":");
  Serial.print(gasValue);
  Serial.print(",\"motion\":");
  Serial.print(motion);
  Serial.println("}");

  delay(2000); // Delay for 2 seconds
}