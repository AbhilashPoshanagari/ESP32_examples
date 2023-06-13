#include <WiFi.h>

const char* ssid = "abhi.sreeJ7";
const char* password = "abhilash";

// const uint16_t port = 4000;
// const char * host = "13.234.227.41";
const char * host = "192.168.43.166";
const uint16_t port = 3000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  // put your main code here, to run repeatedly:
  client.connect(host, port);
 if (!client.connect(host, port)) {
   Serial.println("Connection to host failed");
   delay(1000);
   return;
}
else if(client.connect(host, port)){
  client.print("tcp client Testing from ESP32!");
//  client.print(client.remoteIP());
  Serial.println("Connection to host Success");
    Serial.println(client.remoteIP());
    String line = client.readStringUntil('\r');
    Serial.println(line);
    client.stop();
}
else {
  Serial.println("something went worng");
  }

    delay(10000);
}
