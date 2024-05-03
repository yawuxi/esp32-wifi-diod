#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define WIFI_DIOD 23
#define ERROR_DIOD 5

String DIOD_INFO = "http://192.168.0.104:3000/api/diod-state/23";

void getDiodInfo();

void setup()
{
  Serial.begin(115200);
  pinMode(WIFI_DIOD, OUTPUT);
  pinMode(ERROR_DIOD, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin("HOME_2.0", "49298080");
  Serial.println("\nConnecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  delay(5000);
  getDiodInfo();
}

void getDiodInfo()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    Serial.println("Requesting: " + DIOD_INFO);

    http.begin(DIOD_INFO.c_str());
    int requestStatus = http.GET();

    Serial.println(requestStatus);

    if (requestStatus == 200)
    {
      Serial.println("Response recevied!");

      if (http.getString().toInt() == 1)
      {
        digitalWrite(WIFI_DIOD, HIGH);
      }
      else
      {
        digitalWrite(WIFI_DIOD, LOW);
      }
    }
    else
    {
      Serial.println("Request error, error code: " + requestStatus);

      for (size_t i = 0; i < 3; i++)
      {
        digitalWrite(ERROR_DIOD, HIGH);
        delay(1000);
        digitalWrite(ERROR_DIOD, LOW);
        delay(1000);
      }
    }

    http.end();
  }
}
