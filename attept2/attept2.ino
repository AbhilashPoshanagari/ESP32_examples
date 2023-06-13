#include <PubSubClient.h>
//#include<WiFi.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEsp.h>

#include <WiFiEspUdp.h>
#include <ArduinoJson.h>

#include “SoftwareSerial.h”

#define ORG “***”
#define DEVICE_TYPE “****”
#define DEVICE_ID “****”
#define TOKEN “**************”

#define WIFI_AP “*******”
#define WIFI_PASSWORD “*******”
WiFiEspClient espClient;

//PubSubClient client(espClient);

SoftwareSerial soft(2, 3); // RX, TX
int status = WL_IDLE_STATUS;

char server[] = ORG “.messaging.internetofthings.ibmcloud.com”;
char authMethod[] = “use-token-auth”;
char token[] = TOKEN;
char clientId[] = “d:” ORG “:” DEVICE_TYPE “:” DEVICE_ID;

const char publishTopic[] = “iot-2/evt/status/fmt/json”;
const char responseTopic[] = “iotdm-1/response”;
const char manageTopic[] = “iotdevice-1/mgmt/manage”;
const char updateTopic[] = “iotdm-1/device/update”;
const char rebootTopic[] = “iotdm-1/mgmt/initiate/device/reboot”;

void callback(char* publishTopic, char* payload, unsigned int payloadLength);

//WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, espClient);
//PubSubClient client(espClient);

int publishInterval = 30000; // 30 seconds
long lastPublishMillis;

void setup() {
// initialize serial for debugging
// client.setServer( server, 1883 );
Serial.begin(115200);
InitWiFi();
Serial.print(WiFi.localIP());
if (!!!client.connected()) {
Serial.print(“Reconnecting client to “);
Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
Serial.print(“.”);
//delay(500);
}
Serial.println();
}

}
int counter = 0;
void loop() {

// if (millis() – lastPublishMillis > publishInterval) {
String payload = “{\”d\”:”;
payload += counter++;
payload += “}”;
/*const size_t bufferSize = 2*JSON_OBJECT_SIZE(1) + 20;
DynamicJsonBuffer jsonBuffer(bufferSize);

const char* payload = “a”;*/

Serial.print(“Sending payload: “);
Serial.println(payload);
// client.publish(publishTopic, payload);
//if(client.connected())
//{
// client.publish(publishTopic, (char *)payload.c_str());
if (client.publish(publishTopic, (char *)payload.c_str())) {
Serial.println(“Publish ok”);
if (!!!client.connected()) {
Serial.print(“Reconnecting client to “);
Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
Serial.print(“.”);
//delay(500);
}
Serial.println();
}

} else {
Serial.println(“Publish failed”);
if (!!!client.connected()) {
Serial.print(“Reconnecting client to “);
Serial.println(server);
while (!!!client.connect(clientId, authMethod, token)) {
Serial.print(“.”);
//delay(500);
}
Serial.println();
}

}
}

void InitWiFi()
{
// initialize serial for ESP module
soft.begin(115200);
// initialize ESP module
WiFi.init(&soft);

Serial.println(“Connecting to AP …”);
// attempt to connect to WiFi network
while ( status != WL_CONNECTED) {
Serial.print(“Attempting to connect to WPA SSID: “);
Serial.println(WIFI_AP);
// Connect to WPA/WPA2 network
status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
delay(500);
}
Serial.println(“Connected to AP”);
}

 

void callback(char* publishTopic, char* payload, unsigned int length) {
Serial.println(“callback invoked”);
}
