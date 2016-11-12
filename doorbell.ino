#include <ESP8266WiFi.h>

const char* ssid     = "NerdNetwork";
const char* password = "networkpassword";
const char* host = "api.pushingbox.com";

#define LONG_DELAY_US 583
#define SHORT_DELAY_US 230
#define REPEAT_DELAY_US 5105 // 5105 US according to the signal
#define REPEAT_NUM 98 // 98
#define BUTTON_PIN 2
#define RF433MHZ_PIN 0

int buttonState = 0;

byte waves[] = {
  0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0
};

byte delays[] = {
  0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0
};

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RF433MHZ_PIN, OUTPUT);
  delay(100);
  wifiStart();
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if(buttonState == LOW) {
    Serial.println("Button pressed");
    sendFMsignal();
    triggerNotification();
    delay(7000);
  }

  delay(30);
}

void wifiStart() {
  // Connecting to WiFi network

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  int maxTries = 40;
  int tries = 0;
  
  while (WiFi.status() != WL_CONNECTED) {
    ++tries;
    delay(500);
    Serial.print(".");

    if(tries >= maxTries) {
      Serial.print("Max tries reached (");
      Serial.print(maxTries);
      Serial.println(")");
      break;
    }
  }

  if(tries < maxTries) {
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void sendBit(byte w, byte d) {
  digitalWrite(RF433MHZ_PIN, HIGH);

  if(w == 0) {
    delayMicroseconds(SHORT_DELAY_US);
  } else {
    delayMicroseconds(LONG_DELAY_US);
  }
  
  digitalWrite(RF433MHZ_PIN, LOW);

  if(d == 0) {
    delayMicroseconds(SHORT_DELAY_US);
  } else {
    delayMicroseconds(LONG_DELAY_US);
  }
}

void sendSequence() {
  for (byte i = 0; i < sizeof(waves); i++) {
    sendBit(waves[i], delays[i]);
  }
}

void sendFMsignal() {
  Serial.println("Sending FM signal");
  for (byte i = 0; i < REPEAT_NUM; i++) {
    sendSequence();
    delayMicroseconds(REPEAT_DELAY_US);
  }
}

void triggerNotification() {
  Serial.println("Triggering pushingbox notification");
  if(WiFi.status() != WL_CONNECTED) wifiStart();
  
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/pushingbox?devid=YOUR_DEVID";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

