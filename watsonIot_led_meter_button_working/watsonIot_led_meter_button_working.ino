
/* Basic ESP8266 MQTT example

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
#define LED_PIN 13       //physical pin D7
#define pushButton 4     //physical pin D2
#define RED_LED 12        //physical pin D6 
#define analog_pin A0
boolean trigger = false;
boolean previous = false;
#define ORG "yv3pgj"
#define DEVICE_TYPE "ESP8266"
#define DEVICE_ID "68c63ad53c74"
#define TOKEN "abhilash_esp"
//-------- Customise the above values --------

// Update these with values suitable for your network.

const char* ssid = "abhi.sree";
const char* password = "abhilash";
const char* mqtt_server = ORG".messaging.internetofthings.ibmcloud.com";

char topic[] = "iot-2/evt/status/fmt/string";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
long lastMsg2 = 0;
char msg[50];
char msg2[50];
int data = 0;
int current = 0;
//int value = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(RED_LED, OUTPUT);
  pinMode(pushButton, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
    digitalWrite(LED_PIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(LED_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId, authMethod, token)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("iot-2/evt/status/fmt/string", "welcome to abhi.sree IoT Solutions");
  //    delay(500);
      client.publish("iot-2/evt/input/fmt/string", " abhi.sree Iot Solutions");
  //        delay (500);// ... and resubscribe
      client.subscribe("iot-2/cmd/blink/fmt/string");
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
  if (now - lastMsg > 1000) {
    lastMsg = now;
    data = analogRead(analog_pin);// put your main code here, to run repeatedly:
    
Serial.println(data);
Serial.print("\t");
  //  ++value;
    snprintf (msg, 75, "%ld", data);
    Serial.print("Publish Potentia meter Readings: ");
    Serial.println(msg);
    client.publish("iot-2/evt/status/fmt/string", msg);
   current = digitalRead(pushButton);
  if (current && !previous){
    trigger = !trigger;      // put your main code here, to run repeatedly:
    }
  previous = current;
  digitalWrite(RED_LED, trigger);
    Serial.println(trigger);
    Serial.print("\t");
    snprintf (msg2, 50, "%ld", trigger);
    
    Serial.println(msg2);
    
    
    client.publish("iot-2/evt/input/fmt/string", msg2);
  }
}
