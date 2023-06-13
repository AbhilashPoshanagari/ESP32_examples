#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#define LEDPIN 13     //physical pin D7
#define ORG "<yv3pgj>"
#define DEVICE_TYPE "ESP8266"
#define DEVICE_ID "68c63ad53c74"
#define TOKEN "abhilash_esp"
  const char* ssid = "abhi.sree"
  const char* pwd = "abhilash"
  char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
  char authMethod[] = "use-token-auth";
  char token[] = TOKEN;
  char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LEDPIN, OUTPUT);     // put your setup code here, to run once:
  WiFiConnect();
  WiFiClient espClient;
  PubSubClient client(espClient);
  long lastMsg = 0;
  char msg[50];
  int value = 0;
  MQTTConnect();
  client.setServer(server, 1883);
  client.setCallback(callback);
}

void loop() {
 digitalWrite(LEDPIN, HIGH); // put your main code here, to run repeatedly:

}
void WiFiConnet(){
  WiFi.begin(ssid, pwd);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP("ESPsoftAP_01", "abhilash_esp");
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }
}
void MQTTConnet(){
  if (!!!client.connected()) {
   Serial.print("Reconnecting MQTT client to "); Serial.println(server);
   while (!!!client.connect(clientId, authMethod, token)) {
     Serial.print(".");
     delay(500);
   }
   Serial.println();
 }
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LEDPIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LEDPIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
  
}

