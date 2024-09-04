#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <HTTPClient.h>

  #define TFT_CS     15
  #define TFT_RST    4
  #define TFT_DC     2
  #define BUTTON_PIN 5
  #define TIMER_BUTTON_PIN 22

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "Wokwi-GUEST";
const char* password = ""; 
const char* textURL = "https://raw.githubusercontent.com/YeetTheAnson/HackClub-ideation-engine/main/Ideas.txt";

String sentences[50];  // Array to store sentences
int sentenceCount = 0; 
unsigned long countdownStartTime = 0; 
bool timerRunning = false; 
unsigned long countdownDuration = 59 * 60 * 1000 + 59 * 1000; // 59 minutes and 59 seconds in milliseconds
unsigned long lastRefreshTime = 0;
const unsigned long refreshInterval = 1000; 

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TIMER_BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  
  // Initialize display
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);

  connectToWiFi();
  fetchSentences();
  displayRandomSentence();
}

void loop() {
  // Start generation
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); 
    displayRandomSentence();
    while (digitalRead(BUTTON_PIN) == LOW);  
  }

  // Start countdown timer
  if (digitalRead(TIMER_BUTTON_PIN) == LOW) {
    delay(200); 
    startCountdown();
    while (digitalRead(TIMER_BUTTON_PIN) == LOW);  
  }

  // Update the countdown timer only when running
  if (timerRunning) {
    unsigned long currentTime = millis();
    unsigned long elapsed = currentTime - countdownStartTime;
    unsigned long remaining = countdownDuration - elapsed;

    if (remaining <= 0) {
      remaining = 0;
      timerRunning = false;
      displayCongratsMessage();
    } else if (currentTime - lastRefreshTime >= refreshInterval) {
      displayCountdown(remaining);
      lastRefreshTime = currentTime;
    }
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

      // WARNING: THIS PART OF THE CODE WAS WRITTEN BY WITH WITH ASSIST FROM CHATGPT
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
      // END OF CHATGPT CODE
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
  tft.fillScreen(ILI9341_BLACK);
  int randomIndex = random(0, sentenceCount);
  tft.setCursor(10, 10);
  tft.print(sentences[randomIndex]);
  Serial.println(sentences[randomIndex]);  
  lastRefreshTime = millis(); 
}

void startCountdown() {
  countdownStartTime = millis();
  timerRunning = true;
  displayRandomSentence(); 
}

// Convert ms time to minutes and second
void displayCountdown(unsigned long remaining) {
  tft.fillRect(0, 220, 320, 40, ILI9341_BLACK); 
  unsigned long minutes = remaining / 60000;
  unsigned long seconds = (remaining % 60000) / 1000;
  tft.setCursor(10, 220);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED);
  tft.printf("%02lu:%02lu", minutes, seconds);
}

void displayCongratsMessage() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 120);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.print("Congrats!");
}
