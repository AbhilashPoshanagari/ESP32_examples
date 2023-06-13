#include <ESP8266WiFi.h>
#define ledpin 2
#define sensor_input 5
void setup()
{
  Serial.begin(115200);
  Serial.println();
 // uint8_t ledpin=13;
 // uint8_t sensor_input=5;
  pinMode(ledpin, OUTPUT);     // PHY D7 pin
  pinMode(sensor_input, INPUT);       // PHY D1 pin

  WiFi.begin("abhi.sree", "abhilash");

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
  Serial.println(WiFi.softAPIP());
}

void loop() {
//  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
//  delay(1000);
  digitalRead(HIGH);
  if(digitalRead(sensor_input)==1){
     // Turn the LED on (Note that LOW is the voltage level
 // Serial.print("Object Detected\n");
  
  digitalWrite(ledpin, LOW);  // Turn the LED off by making the voltage HIGH
//  Serial.print("led ON");
  //delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  }
  else
  digitalWrite(ledpin, HIGH);
//  Serial.print("led OFF");
  
  }
