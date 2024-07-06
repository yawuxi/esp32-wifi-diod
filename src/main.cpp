#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#define WIFI_DIOD 32
#define ERROR_DIOD 5

WebSocketsClient WSClient;
String DIOD_INFO = "http://192.168.0.104:3000/api/diod-state/23";

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_ERROR:
    Serial.println("Error in websocket connection");
    break;
  case WStype_DISCONNECTED:
    Serial.println("Disconected from WS");
    break;
  case WStype_CONNECTED:
    Serial.println("Connected to WS");
    break;
  case WStype_TEXT:
    payload[length] = '\0';

    if (strcmp((char *)payload, "0") == 0)
    {
      Serial.println("LOW");
      digitalWrite(WIFI_DIOD, LOW);
    }
    else if (strcmp((char *)payload, "1") == 0)
    {
      Serial.println("HIGH");
      digitalWrite(WIFI_DIOD, HIGH);
    }

    break;
  }
}

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

  WSClient.begin("192.168.0.104", 3000, "/api/diod-state/23", "wss");
  WSClient.onEvent(webSocketEvent);
}

void loop()
{
  WSClient.loop();
}
