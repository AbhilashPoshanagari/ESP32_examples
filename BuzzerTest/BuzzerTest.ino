#define BUZZER_PIN 12 //physical D4
#define PHOTO_SENS A0 //physical D7

void setup() {
 digitalWrite(BUZZER_PIN, OUTPUT); // put your setup code here, to run once:

}

void loop() {
  tone(BUZZER_PIN, analogRead(PHOTO_SENS) * 4, 10);// put your main code here, to run repeatedly:

}
