#include <TFT_eSPI.h> 
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2
#define BUTTON_PIN 5

TFT_eSPI tft = TFT_eSPI(); 
const char* ssid = "Wokwi-GUEST";
const char* password = ""; 
const char* textURL = "https://raw.githubusercontent.com/YeetTheAnson/HackClub-ideation-engine/main/Ideas.txt";

String sentences[50];  
int sentenceCount = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  
  // Initialize the display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  connectToWiFi();

  fetchSentences();
  
  displayRandomSentence();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); 
    displayRandomSentence();
    while (digitalRead(BUTTON_PIN) == LOW); 
  }
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Serial.println("Connected to WiFi");
}

void fetchSentences() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(textURL);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      int start = 0;
      while (start >= 0 && sentenceCount < 50) {
        int startQuote = payload.indexOf('"', start);
        int endQuote = payload.indexOf('"', startQuote + 1);
        
        if (startQuote >= 0 && endQuote > startQuote) {
          sentences[sentenceCount] = payload.substring(startQuote + 1, endQuote);
          sentenceCount++;
          start = endQuote + 1;
        } else {
          break;
        }
      }

      if (sentenceCount == 0) {
        sentences[0] = "No sentences found.";
        sentenceCount = 1;
      }
    } else {
      Serial.println("Error in HTTP request");
      sentences[0] = "Error in fetching data.";
      sentenceCount = 1;
    }
    
    http.end();
  } else {
    Serial.println("Not connected to WiFi");
    sentences[0] = "Not connected to WiFi.";
    sentenceCount = 1;
  }
}

void displayRandomSentence() {
  tft.fillScreen(TFT_BLACK);
  int randomIndex = random(0, sentenceCount);
  tft.drawString(sentences[randomIndex], 10, 10, 2);
  Serial.println(sentences[randomIndex]); 
}
