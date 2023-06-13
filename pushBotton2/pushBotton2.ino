#define LED_PIN 13
#define BUTTON_PIN 12     //physical D6 pin
boolean trigger = false;
boolean previous = false;

void setup() {
 pinMode(LED_PIN, OUTPUT);     // put your setup code here, to run once:
 pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int current = digitalRead(BUTTON_PIN);
  if (current && !previous){
    trigger = !trigger;      // put your main code here, to run repeatedly:
  }
previous = current;
  digitalWrite(LED_PIN, trigger);
}
