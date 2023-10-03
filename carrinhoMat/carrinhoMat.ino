const int IN1 = 2; //DADO acelera
const int IN2 = 3; //DADO ré
const int IN3 = 4; //FORWARD
const int IN4 = 5; //FORWARD2










void setup() {

  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


Serial.begin(9600);
}

void loop() {
 int var = digitalRead(IN1);
 if(var == HIGH){
   digitalWrite(IN3,HIGH);
   digitalWrite(IN4,LOW);
 }

}

