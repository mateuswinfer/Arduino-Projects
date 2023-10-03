//Programa : Controle 2 motores DC usando Ponte H L298N
//Autor : Marcos
//Data: 19/09/2021
//Definicoes pinos Arduino ligados a entrada da Ponte H

#include <HCSR04.h>
#include <Servo.h>
#include <EEPROM.h>

Servo myservo;  // create servo object to control a servo
UltraSonicDistanceSensor distanceSensor(2, 4);  // Initialize sensor that uses digital pins 2 and 4.


int IN1 = 3;       // direcao      Pino 3 do nano
int IN2 = 9;       // direcao      Pino 9 do nano
int IN3 = 10;      // frente-re    Pino 10 do nano
int IN4 = 11;      // frente-re    Pino 11 do nano

//*******************Prototipo de funçoes ******************************************
void AndaFrente();
void ViraDireita();
void ViraEsquerda();
void AndaTras();
void Freio();
void scan();
void viraDireita45();
void viraEsquerda45();
void virafrente();
void initdirecao();

//***************** Variaveis Globais *******************************************

short andar = LOW; // variavel para guardar se o carro estiver andando ou nao
int pos = 0;    // variable to store the servo position
bool dir, dir45, frent, esq45, esq = LOW; // variavel que vai guardar se o carro ja estiver virado a alguma posicao


void setup()
{
  myservo.attach(5);  // attaches the servo on pin 5 to the servo object
  Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
  //Define os pinos como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

}
// initdirecao();           // Garante que a direcao sempre seja em linha reta
// delay(1000);            // Aguarda o Sistema Inicializar
//    while (1) {
//     // scan();             // Faz a leitura do local (180 graus nos dois lados)
//       AndaFrente();
//       delay(1000);
//       Freio();
//      delay(200);
//       AndaTras();
//       delay(1000);
//        Freio();
//     //  ViraDireita();
//     //   delay(500);
//     //   ViraEsquerda();
//     //   delay(500);
//     //   viraDireita45();
//     //    delay(500);
//     //    viraEsquerda45();
//     //    delay(500);
//    }
//  }
//


void loop() {
  short var1 = distanceSensor.measureDistanceCm();
  if ( var1 > 30) {
    AndaFrente();
  }
  else {
    Freio();
    // delay(50);
    AndaTras();
    while (var1 < 30) {
      AndaTras();
      var1 = distanceSensor.measureDistanceCm();
    }
    scan();
  }

  var1 = 0;
  // delay(200);
  // AndaTras();
  //Freio();


}



//****************** O carro anda para tras ******************************
void AndaTras() {
  //Gira o Motor B no sentido horario

  if (esq)ViraDireita();
  else
    ViraEsquerda();
  delay(250);
  int x, y = 0;
  y = distanceSensor.measureDistanceCm();
  while (x < 10) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);     //Esse valor *Deve* ser Low
    delay(10);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);     //Esse valor *Deve* ser Low
    delay(18);                 // ajusta a força para andar para tras
    ++ x;

  }
  y = distanceSensor.measureDistanceCm();
  while (y < 30) {
    y = distanceSensor.measureDistanceCm();
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);     //Esse valor *Deve* ser Low
    delay(10);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);     //Esse valor *Deve* ser Low
    delay(18);                 // ajusta a força para andar para tras
  }
}
//******************** Vira o carro para a Direita 180 **************************
void ViraDireita() {
  //Gira o Motor A no sentido horario
  int x, y = 0;
  if (dir)y = 0;
  else
    y = 90;
  for (x = 0; x <= 90; ++x) {
    Serial.print(x);
    analogWrite(IN1, x);
    digitalWrite(IN2, LOW);           // Sendo Low, ele vira para o sentido Horario
    delay(5);
  }
  dir = HIGH;
  esq = LOW;                      // APAGO MEMORIA DE RODA JA VIRADA PARA ESQUERADA
  delay(250);
  //Parar o motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  EEPROM.write(0, 1);            // Gravo na posicao 0, o valor 1 na EEPROM
  delay(30);
}

//************************VIRA A 45 GRAUS A DIREITA ***************************
void viraDireita45() {
  //Gira o Motor A no sentido horario
  int x = 0;
  for (x = 0; x <= 50; ++x) {
    analogWrite(IN1, x);
    digitalWrite(IN2, LOW);           // Sendo Low, ele vira para o sentido Horario
    delay(10);
  }

  //Parar o motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  EEPROM.write(0, 2);            // Gravo na posicao 0, o valor 2 na EEPROM
  delay(30);

}


//********************** Vira o carro para a Esquerda 180 *************************
void ViraEsquerda() {
  //Gira o Motor A no sentido anti-horario
  int y, x = 0;
  if (esq == HIGH)x = 0;
  else
    x = 18;
  for (y = 0; y <= x; ++y) {
    Serial.println(y);
    analogWrite(IN1, 1);
    digitalWrite(IN2, HIGH);         // Sendo High, ele vira para o sentido Anti-Horario
    delay(10);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);         // Sendo High, ele vira para o sentido Anti-Horario
    delay(10);
  }
  esq = HIGH;
  dir = LOW;
  //Para o motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  EEPROM.write(0, 3);            // Gravo na posicao 0, o valor 3 na EEPROM
  delay(30);
}

//************************VIRA A 45 GRAUS A ESQUERDA ***************************
void viraEsquerda45() {
  int y = 0;
  int z = 0;
  //  // Caso 100% a esquerda viro 45% a direita
  if (esq == HIGH) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);           // Sendo Low, ele vira para o sentido Horario
    delay(35);
    //Parar o motor A
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
  }
  //      else if (frent == HIGH) {
  //        digitalWrite(IN1, LOW);
  //        digitalWrite(IN2, HIGH);         // Sendo High, ele vira para o sentido Anti-Horario
  //        delay(10);
  //  }

}
//*********************** VIRA PARA FRENTE *****************************
void virafrente() {
  int x = 0;
  for (x = 0; x <= 70; ++x) {    // Viro 45 graus para a direita
    analogWrite(IN1, x);
    digitalWrite(IN2, LOW);           // Sendo Low, ele vira para o sentido Horario
    delay(10);
  }
  delay(25);
  //Parar o motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  delay(50);
}
//********************** Aceleraçao do carro ****************************
void AndaFrente() {
  //Gira o Motor B no sentido anti-horario
  analogWrite(IN4, 100);                            // Ajuste de potencia para o carro andar para frente ( Sendo 255 o Max)
  digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);
}

//********************** Freia o carro ************************************
void Freio() {
  if (andar == HIGH) {                              // Se o carro estiver andando para frente, inverte a rotacao por 100 miliseconds
    analogWrite(IN3, 100);
    digitalWrite(IN4, LOW);
    delay(500);                                      // Define quanto tempo de freio o carro tem
    digitalWrite(IN3, LOW);                          // Para de Acelerar
    digitalWrite(IN4, LOW);
  }
  if (andar == LOW) {                              // Se o carro estiver andando para Tras, inverte a rotacao por 100 miliseconds
    analogWrite(IN4, 100);
    digitalWrite(IN3, LOW);
    delay(500);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

//********************* Funçao de Escaneamento *************************
void scan() {
  int x1, x2, x3, x4, x5, x6 = 0;
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
    if (pos == 10) {
      delay(100);
      x6 = distanceSensor.measureDistanceCm();
      x1 = x6;
      Serial.print("X1 = ");
      Serial.println(x6);
    }
    if (pos == 45) {
      delay(100);
      x6 = distanceSensor.measureDistanceCm();
      x2 = x6;
      Serial.print("X2 = ");
      Serial.println(x6);
    }
    if (pos == 90) {
      delay(100);
      x6 = distanceSensor.measureDistanceCm();
      x3 = x6;
      Serial.print("X3 = ");
      Serial.println(x6);
    }
    if (pos == 135) {
      delay(100);
      x6 = distanceSensor.measureDistanceCm();
      x4 = x6;
      Serial.print("X4 = ");
      Serial.println(x6);
    }
    if (pos == 160) {
      delay(100);
      x6 = distanceSensor.measureDistanceCm();
      x5 = x6;
      Serial.print("X5 = ");
      Serial.println(x6);
    }

  }

  //  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //    myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //    delay(15);                       // waits 15 ms for the servo to reach the position
  //  }
  myservo.write(85);                 // move o sensor para o centro


  //***************** Testa X1 ******************************************
  if (x1 > x2 && x1 > x3 && x1 > x4 && x1 > x5 && dir == LOW) {
    Serial.println("Viro para a direita");
    delay(500);
    ViraDireita();
    esq = LOW;
    dir = HIGH;
    dir45 = LOW;
    esq45 = LOW;
    frent = LOW;

  }
  else if (x2 > x1 && x2 > x3 && x2 > x4 && x2 > x5 && dir45 == LOW) {
    Serial.println("Viro 45 graus para a direita");
    delay(500);
    // viraDireita45();
    ViraDireita();
    esq45 = LOW;
    esq = LOW;
    dir = LOW;
    dir45 = HIGH;
    frent = LOW;
    // myservo.write(180);
  }
  else if (x3 > x1 && x3 > x2 && x3 > x4 && x3 > x5 && frent == LOW) {
    Serial.println("Viro 90 graus");
    delay(500);
    virafrente();
    frent = HIGH;
    esq45 = LOW;
    dir = LOW;
    dir45 = LOW;
    esq = LOW;
  }
  else if (x4 > x1 && x4 > x2 && x4 > x3 && x4 > x5 && esq45 == LOW) {
    Serial.println("Viro a 45 graus a esquerda");
    delay(500);
    // viraEsquerda45();
    ViraEsquerda();
    dir45 = LOW;
    dir = LOW;
    esq = LOW;
    esq45 = HIGH;
    frent = LOW;
  }
  else if (x5 > x1 && x5 > x2 && x5 > x3 && x5 > x4 && esq == LOW) {
    Serial.println("Viro para a esquerda");
    delay(500);
    ViraEsquerda();
    dir = LOW;
    dir45 = LOW;
    esq = HIGH;
    esq45 = LOW;
    frent = LOW;
  }

  delay(1000);
  Serial.println();
  Serial.println("****************************************************************************************");

}

void initdirecao() {
  int value = 0;
  value = EEPROM.read(0);
  delay(30);
  switch (value) {
    case 1:
      // ViraEsquerda();
      //Serial.println("a Direita!!!");
      break;
    case 2:
      //Serial.println("a Direita45!!!");
      //viraEsquerda45();
      break;
    case 3:
      //ViraDireita();
      //Serial.println("Esquerda!!!");
      break;
    case 4:
      //viraEsquerda45();
      //Serial.println("Esquerda45!!!");
      break;
    default:
      // statements
      break;
  }

}





void teste() {
  //ESQUERDA
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);         // Sendo High, ele vira para o sentido Anti-Horario
  delay(11);
  delay(3000);
  //DIREITA
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);           // Sendo Low, ele vira para o sentido Horario
  delay(10);

  //Parar o motor A
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);

  delay(30);

}
