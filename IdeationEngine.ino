#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Wokwi-GUEST";

// URL containing raw text
const char* url = "http://example.com/sentences.txt";

LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

// Button pin
const int buttonPin = 38;
int lastButtonState = HIGH; 

void setup() {
  Serial.begin(115200);

  Wire.begin(8, 9); // Use GPIO 8 for SDA and GPIO 9 for SCL

  lcd.begin(16, 2); // 16 columns and 2 rows
  lcd.backlight();
  lcd.clear();

  pinMode(buttonPin, INPUT_PULLUP); // Button connected to GND, use internal pull-up

  // Connect to WiFi
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Fetch and display the first sentence
  displayRandomSentence();
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); 
    if (digitalRead(buttonPin) == LOW) {
      displayRandomSentence();
    }
  }

  lastButtonState = buttonState; 
}

void displayRandomSentence() {
  HTTPClient http;
  http.begin(url);
  String payload = http.getString();

  // Parse the payload and get a random sentence
  String sentence = getRandomSentence(payload);

  // Display the sentence on the LCD
  lcd.clear();
  lcd.setCursor(0, 0); // Start at the first column, first row
  lcd.print(sentence.substring(0, 16));

  if (sentence.length() > 16) {
    lcd.setCursor(0, 1); // Move to the second row
    lcd.print(sentence.substring(16, 32)); 
  }

  http.end(); // Free the resources
}

String getRandomSentence(String text) {
  // Split the text by double quotes and store sentences in an array
  int startIdx = 0;
  int endIdx = 0;
  int sentenceCount = 0;
  String sentences[100];

  while ((startIdx = text.indexOf('"', endIdx)) != -1) {
    endIdx = text.indexOf('"', startIdx + 1);
    if (endIdx != -1) {
      sentences[sentenceCount] = text.substring(startIdx + 1, endIdx);
      sentenceCount++;
    }
  }

  // Pick a random sentence
  int randomIndex = random(0, sentenceCount);
  return sentences[randomIndex];
}
