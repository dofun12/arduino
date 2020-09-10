
int delayTime= 100;
int cooldown = 10*delayTime;
int cycle = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10, INPUT);
  pinMode(8, OUTPUT);
  pinMode(6, OUTPUT);
}

void apita(){
   if(cycle == 0){
     digitalWrite(6, HIGH);
     delay(100);
     digitalWrite(6, LOW);
     delay(100);
     digitalWrite(6, HIGH);
     delay(100);
     digitalWrite(6, LOW);
     cycle = cooldown;
   }else{
     cycle--;
   }
   
}

void loop() {
  int acionamento = digitalRead(10);
  Serial.println(acionamento);
  if(acionamento){
    digitalWrite(8, HIGH);
    apita();
  }else{
    cycle = 0;
    digitalWrite(8, LOW);
  }
  delay(delayTime);
  // put your main code here, to run repeatedly:

}
