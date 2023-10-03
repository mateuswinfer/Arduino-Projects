#include <HCSR04.h>
#include <Servo.h>

Servo myservo;


UltraSonicDistanceSensor distanceSensor(2, 4);  // Initialize sensor that uses digital pins 2 and 4.


#define Vdir myservo.write(10);
#define Vfren myservo.write(85);
#define Vesq myservo.write(170);


// Definitions of Arduino pins connected to the H-Bridge inputs
const int IN1 = 9;   // Left
const int IN2 = 3;   // Right
const int IN3 = 10;  // Backward
const int IN4 = 11;  // Forward
const int SensR = A2;
const int SensL = A3;
const int Sensbluethooth = 7;


//  ____________Variaveis Globais________________

bool sensorRe = true;
int Amedir[16];
int mediu, speed = 0;
int pos = 0;
bool D, F, E = false;
bool R, L, U, M = false;  //Right - Direita, Left - Esquerda, U - Frente, M -  Mediu
bool acel = false;
bool thooth = false;

// ___________Diretivas do copilador _______________




void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Sensbluethooth, INPUT);
  pinMode(A1, OUTPUT);  // Pino que acende os Farois
  pinMode(SensR, INPUT);   // Pino sensor traseiro
  pinMode(A3, INPUT);  // Pino que sensor de luminosidade
  Serial.begin(9600);
  myservo.attach(5);
  Vfren;

  bool thooth = digitalRead(Sensbluethooth);
  if (thooth) {
    bluethooth();
  }

 Scanner();
}
//---------------------------------------------------------------
void loop() {
   medir();
  if(mediu <=50 && acel == true){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    Scanner(); }
  farol();
  bool thooth = digitalRead(Sensbluethooth);
  if (thooth) {
    bluethooth();
  } 
  
}
//-------------------------------------------------------------------

void Scanner() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN1, LOW);
  acel = false;
  int esquerda, frente, direita = 0;
  Vdir;
  medir();
  direita = mediu;
  Vfren;
  medir();
  frente = mediu;
  Vesq;
  medir();
  esquerda = mediu;

  if (direita > frente && direita > esquerda && !D) {  //  Vira para direita
    Serial.println("Virei direita");
    turnRight();
    acelera();
    D = true;
    E = false;
    F = false;
  }

  else if (frente > esquerda && frente > direita) {  // Virar frente
    Serial.println("Virei pra frente");
    center();
    acelera();
    D = false;
    E = false;
    F = true;
  } else if (esquerda > direita && esquerda > frente) {
    Serial.println("Virei pra esquerda");
    turnLeft();
    acelera();
    D = false;
    E = true;
    F = false;
  }
}

void farol() {
  int led = analogRead(SensL);
  if (led >= 900) {
    digitalWrite(A1, HIGH);
  } else {
    digitalWrite(A1, LOW);
  }
}


int medir() {

  int s = 0;
  int x, Stotal = 0;
  mediu = 0;
  for (x = 0; x < 15; x++) {
    s = distanceSensor.measureDistanceCm();
    Amedir[x] = s;
    delay(100);
  }

  for (x = 0; x < 15; x++) {
    Stotal = Stotal + Amedir[x];
  }
  mediu = (Stotal / 15);
  Serial.println(mediu);
  Vfren;
  return mediu;
}

void turnLeft() {
  if (!L && !E) {
    for (int x = 0; x < 7; x++) {  // Ajuste de angulo
      digitalWrite(IN1, HIGH);     // Ajusta mais força
      delay(100);
      digitalWrite(IN1, LOW);
      delay(50);  // Ajusta menos força
      L = true;
      R = false;
    }
  }
}

void turnRight() {
  if (!R && !D) {
    digitalWrite(IN2, HIGH);  // Ajusta mais força
    delay(300);
    digitalWrite(IN2, LOW);
    delay(50);
    neutro();
    delay(100);
    L = false;
    R = true;
  }
}

void center() {
  if (!F) {
    if (R) {
      digitalWrite(IN1, HIGH);  // Ajusta mais força
      delay(200);
      digitalWrite(IN1, LOW);
      delay(50);
    } else if (L) {
      digitalWrite(IN2, HIGH);  // Ajusta mais força
      delay(200);
      digitalWrite(IN2, LOW);
      delay(50);
    }
  }
}



void neutro() {
  medir();
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  delay(10);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void acelera() {
  analogWrite(IN4, 150);
  acel = true;
}
void bluethooth() {

  bool move, r, led = false;
  neutro();

  bool thooth = digitalRead(Sensbluethooth);

  while (thooth) {

    bool thooth = digitalRead(Sensbluethooth);

    if (Serial.available()) {
      char input = Serial.read();
      Serial.println(input);
      if (input == 'A')  // ACELERAR
      {
        speed = speed + 25;
        analogWrite(IN4, speed);
        if(speed >=255)speed=255;
      } else if (input == 'C') {  // FREIO
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        speed = 50;
      }  else if (input == 'D' && !led) {
        led = true;
        digitalWrite(A1, HIGH);
      } else if (led) {
        led = false;
        digitalWrite(A1, LOW);
      } else if (input == '>') {
        digitalWrite(IN2, HIGH);
        delay(100);
        digitalWrite(IN2, LOW);
      } else if(input == 'B'){
        digitalWrite(IN3, HIGH);
        delay(100);
        digitalWrite(IN3, LOW);
      } else if (input == '<') {
        digitalWrite(IN1, HIGH);
        delay(100);
        digitalWrite(IN1, LOW);}
    }
  }
}


  

