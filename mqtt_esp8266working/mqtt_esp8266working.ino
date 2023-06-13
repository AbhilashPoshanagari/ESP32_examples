/*
  Basic ESP8266 MQTT example

  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.

  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define LED_PIN1 13       //physical pin D7
#define LED_PIN2 12       //physical pin D6
#define analog_pin A0
// Update these with values suitable for your network.

const char* ssid = "abhi.sreeJ7";
const char* password = "abhilash";
const char* mqtt_server = "192.168.43.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int data = 0;
// int value = 0;

void setup() {
  pinMode(LED_PIN1, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED_PIN2, OUTPUT);
  pinMode(analog_pin, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback1);
  client.setCallback(callback2);
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
void callback1(char* topic1, byte* payload1, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic1);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload1[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload1[0] == '1') {
    digitalWrite(LED_PIN1, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LED_PIN1, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void callback2(char* topic2, byte* payload2, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic2);
  Serial.print("] ");
  for (int j = 0; j < length; j++) {
    Serial.print((char)payload2[j]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload2[0] == '1') {
    digitalWrite(LED_PIN2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LED_PIN2, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "welcome to abhi.sree IoT Solutions");
      // ... and resubscribe
      client.subscribe("inTopic1");
      client.subscribe("inTopic2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    data = analogRead(analog_pin); //put your main code here, to run repeatedly:
    
   // ++value;
    snprintf (msg, 75, "%ld", data);
    Serial.print("Publish Potentio meter Readings : ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
