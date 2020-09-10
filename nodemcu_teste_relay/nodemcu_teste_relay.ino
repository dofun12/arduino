#define RELAY D0

void setup() {
  pinMode(RELAY,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  
  digitalWrite(RELAY, HIGH);
  delay(3000);
  digitalWrite(RELAY, LOW);
  delay(3000);
}
