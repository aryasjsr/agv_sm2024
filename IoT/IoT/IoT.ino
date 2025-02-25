
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

const int outputPin0 = 33;
const int outputPin1 = 12;
const int outputPin2 = 14;
const int outputPin3 = 27;
const int outputPin4 = 26;
const int outputPin5 = 25;

const char* ssid = "AMR_IISMA24";     // CHANGE IT
const char* password = "12345678";  // CHANGE IT

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket server on port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
       digitalWrite(outputPin0, LOW);
       Serial.println("Ay");
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        digitalWrite(outputPin0, HIGH);
         Serial.println("Ayo");
      }
      break;
    case WStype_TEXT:
      //Serial.printf("[%u] Received text: %s\n", num, payload);
      String angle = String((char*)payload);
      int command = angle.toInt();
      Serial.print("command: ");
      Serial.println(command);

      switch (command) {
        case CMD_STOP:
          // Serial.println("Stop");
          CAR_stop();
          break;
        case CMD_FORWARD:
          // Serial.println("Move Forward");
          CAR_moveForward();
          break;
        case CMD_BACKWARD:
          // Serial.println("Move Backward");
          CAR_moveBackward();
          break;
        case CMD_LEFT:
          // Serial.println("Turn Left");
          CAR_turnLeft();
          break;
        case CMD_RIGHT:
          // Serial.println("Turn Right");
          CAR_turnRight();
          break;
        default:
          Serial.println("Unknown command");
      }
  
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1,16,17);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Serve a basic HTML page with JavaScript to create the WebSocket connection
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Web Server: received a web page request");
    String html = HTML_CONTENT;  // Use the HTML content 
    request->send(200, "text/html", html);
  });

  server.begin();
  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());

  // Set the pins as outputs
  pinMode(outputPin0, OUTPUT);
  pinMode(outputPin1, OUTPUT);
  pinMode(outputPin2, OUTPUT);
  pinMode(outputPin3, OUTPUT);
  pinMode(outputPin4, OUTPUT);
  pinMode(outputPin5, OUTPUT);


}

void loop() {
  webSocket.loop();
  // TO DO: Your code here
}

void CAR_moveForward() {
  Serial2.println('1');
  digitalWrite(outputPin1, LOW);
  digitalWrite(outputPin2, HIGH);
  digitalWrite(outputPin3, LOW);
  digitalWrite(outputPin4, LOW);
  digitalWrite(outputPin5, LOW);
}

void CAR_moveBackward() {
 Serial2.println('2');
  digitalWrite(outputPin1, LOW);
  digitalWrite(outputPin2, LOW);
  digitalWrite(outputPin3, HIGH);
  digitalWrite(outputPin4, LOW);
  digitalWrite(outputPin5, LOW);
}

void CAR_turnLeft() {
 Serial2.println('3');
  digitalWrite(outputPin1, LOW);
  digitalWrite(outputPin2, LOW);
  digitalWrite(outputPin3, LOW);
  digitalWrite(outputPin4, HIGH);
  digitalWrite(outputPin5, LOW);
}

void CAR_turnRight() {
  digitalWrite(outputPin1, LOW);
  digitalWrite(outputPin2, LOW);
  digitalWrite(outputPin3, LOW);
  digitalWrite(outputPin4, LOW);
  digitalWrite(outputPin5, HIGH);
}

void CAR_stop() {
  digitalWrite(outputPin1, HIGH);
  digitalWrite(outputPin2, LOW);
  digitalWrite(outputPin3, LOW);
  digitalWrite(outputPin4, LOW);
  digitalWrite(outputPin5, LOW);
}
