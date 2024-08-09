#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>
#include <algorithm>
#include <random>

const char* ssid = "Wokwi-GUEST";

const char* url = "https://raw.githubusercontent.com/YeetTheAnson/vineboom/main/thisisasample.txt";

LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

// Button pin
const int buttonPin = 38;
int lastButtonState = HIGH; 

// Global variables
std::vector<String> sentences;
std::vector<int> remainingIndices;
std::mt19937 rng; 

void setup() {
  Serial.begin(115200);

  Wire.begin(8, 9); // Use GPIO 8 for SDA and GPIO 9 for SCL

  lcd.begin(16, 2); // 16 columns and 2 rows
  lcd.backlight();
  lcd.clear();

  pinMode(buttonPin, INPUT_PULLUP); 

  // Connect to WiFi
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Seed the random number generator
  rng.seed(esp_random());

  fetchSentences();
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

void fetchSentences() {
  HTTPClient http;
  http.begin(url); 
  String payload = http.getString(); 

  // Parse the payload and store sentences
  sentences = parseSentences(payload);

  // Initialize remaining indices
  remainingIndices.resize(sentences.size());
  std::iota(remainingIndices.begin(), remainingIndices.end(), 0); 

  // Shuffle indices
  std::shuffle(remainingIndices.begin(), remainingIndices.end(), rng);
  
  http.end(); 
}

void displayRandomSentence() {
  if (remainingIndices.empty()) {
    // Refill the list if all sentences have been used
    fetchSentences();
  }

  int index = remainingIndices.back();
  remainingIndices.pop_back(); 

  String sentence = sentences[index];
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(sentence.substring(0, 16));

  if (sentence.length() > 16) {
    lcd.setCursor(0, 1); 
    lcd.print(sentence.substring(16, 32)); 
  }
}

std::vector<String> parseSentences(String text) {
  std::vector<String> result;
  int startIdx = 0;
  int endIdx = 0;

  while ((startIdx = text.indexOf('"', endIdx)) != -1) {
    endIdx = text.indexOf('"', startIdx + 1);
    if (endIdx != -1) {
      result.push_back(text.substring(startIdx + 1, endIdx));
    }
  }

  return result;
}
