#include <PubSubClient.h>


#include <WiFi.h>

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

/* #include <ESP8266WiFi.h> */
#include <PubSubClient.h>
#include <HardwareSerial.h>
#define LED_PIN 33       //physical pin 9
#define analog_pin 36   // physical anlog pin
#define UART_TX 17
#define UART_RX 16
#define ORG "w5xjt5"
#define DEVICE_TYPE "esp32_Testing"
#define DEVICE_ID "esp32_testing2"
#define TOKEN "testing_dev"
//-------- Customise the above values --------

// Update these with values suitable for your network.

const char* ssid = "abhi.sreeM32";
const char* password = "abhilash";
const char* mqtt_server = ORG".messaging.internetofthings.ibmcloud.com";

char topic[] = "iot-2/evt/status/fmt/string";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

HardwareSerial SerialPort(2); // use UART2
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int data = 0;
//int value = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  SerialPort.begin(15200, SERIAL_8N1, UART_RX, UART_TX);
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
          delay (5000);// ... and resubscribe
      client.subscribe("iot-2/cmd/blink/fmt/string");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}

char change(){
  
  }

void uartCheck(){
  char check = Serial.read();
  if(check == 1){
    Serial.write("-");
    SerialPort.write(check);
  }
  else{
    Serial.write("-");
    SerialPort.write(check);
  }
  delay(1000);
  }
  
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  uartCheck();
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
  }
}
