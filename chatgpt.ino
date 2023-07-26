#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "a";
const char* password = "a";
const char* apiKey = "sk-a";

void performTextCompletion() {
  String inputText = "Where is the capital of Japan?";
  String url = "https://api.openai.com/v1/completions";
  String payload = "{\"prompt\":\"" + inputText + "\",\"max_tokens\":10, \"model\": \"text-davinci-003\"}";

  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect("api.openai.com", 443)) {
    Serial.println("connection failed");
    return;
  }

  client.println("POST " + String(url) + " HTTP/1.1");
  client.println("Host: api.openai.com");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + String(payload.length()));
  client.println("Authorization: Bearer " + String(apiKey));
  client.println("Connection: close");
  client.println();
  client.println(payload);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  String response = "";
  while (client.available()) {
    char c = client.read();
    response += c;
  }

  client.stop();

  int startIndex = response.indexOf("\"text\": \"") + 9;
  int endIndex = response.indexOf("\"", startIndex);
  String textOutput = response.substring(startIndex, endIndex);

  // Replace the literal string "\n\n" with an empty string
  textOutput.replace("\\n\\n", "");

  // Trim leading and trailing whitespace
  textOutput.trim();

  Serial.println(textOutput);
}

void setup() {
  Serial.begin(9600);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    Serial.flush();
  }

  Serial.println("Connected to WiFi");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.flush();

  performTextCompletion();  // Perform text completion once during setup
}

void loop() {
  // Empty loop
}
