#define analog_pin A0

void setup() {
Serial.begin(115200);
// pinMode(analog_pin, OUTPUT); // put your setup code here, to run once:

}

void loop() {
 int data = analogRead(analog_pin);// put your main code here, to run repeatedly:
Serial.println(data);
Serial.print("\t");
delay(500);
}
