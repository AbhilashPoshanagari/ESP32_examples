#define led6 12 //Physical D6
#define led10 16 //Physical D0
void setup() {
  int i;
 for( i=led6;i<led10;i++){
 pinMode(i, OUTPUT);   // put your setup code here, to run once:
 }
 }

void loop() {
  int i;
 for(i=led6;i<led10;i+=1){
  digitalWrite(i, HIGH);// put your main code here, to run repeatedly:
 delay(200);
 digitalWrite(i, LOW);
 }

}
