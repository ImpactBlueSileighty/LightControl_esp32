// Подключаем библиотеку для работы с HTTP-протоколом
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Вводим имя и пароль точки доступа
const char* ssid = "SSID_NETWORK";
const char* password = "PASSWORD";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  // делаем небольшую задержку на открытие монитора порта
  delay(5000);
  // подключаемся к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
}

void loop() {
  // выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) {
    // создаем объект для работы с HTTP
    HTTPClient http;
    // подключаемся к веб-странице
    http.begin("http://192.168.1.67:8080/api/light");
    // делаем GET запрос
    int httpCode = http.GET();
    // проверяем успешность запроса
    if (httpCode > 0) {
      // выводим ответ сервера
      String payload = http.getString();
      
      display.clearDisplay();
      display.setTextSize(1);             
      display.setTextColor(WHITE);        
      display.setCursor(0,20);             
      display.println(payload);
      display.display();
    }
    else {
      Serial.println("Ошибка HTTP-запроса");
    }
    // освобождаем ресурсы микроконтроллера
    http.end();
  }
  delay(10000);
}
