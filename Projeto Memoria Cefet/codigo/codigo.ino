#include <LiquidCrystal.h>

// Definição de variáveis
#define buz 7
#define ledG 8
#define ledR 9
#define ledY 10
#define ledB 13
#define pushG A0
#define pushR A1
#define pushY A2
#define pushB A3

// Declaração das variáveis do Display LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Declaração dos pinos usados
  pinMode(pushG, INPUT);
  pinMode(pushR, INPUT);
  pinMode(pushY, INPUT);
  pinMode(pushB, INPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(buz, OUTPUT);

  randomSeed(analogRead(0)); // Garante que os números serão aleatórios quando o jogo é reiniciado
}

void loop() {
  lcd.begin(16, 2);
  int inicio = 1; // Botão de iniciar
  lcd.print("Vamos jogar?"); // Escreve no LCD

  // Inicia o jogo quando o botão do led verde for pressionado
  while(inicio!=1023){ // Como os botão estão ligados em portas analógicas, quando eles são pressionados, o valor de retorno é 1023
    inicio = analogRead(pushG); // Lê o botão
    digitalWrite(ledG, HIGH); // Liga leds
    digitalWrite(ledR, HIGH);
    digitalWrite(ledY, HIGH);
    digitalWrite(ledB, HIGH);
  }

  lcd.clear(); // Apaga LCD
  digitalWrite(ledG, LOW); // Apaga leds
  digitalWrite(ledR, LOW);
  digitalWrite(ledY, LOW);
  digitalWrite(ledB, LOW);
  delay(1000); // Pausa de 1 segundo

  int acertou = 1; // Variável usada para determinar se o jogador acertou a sequência de cores ou não
  int luzes[200]; // Variável para guardar a sequência de cores
  int quant = 0; // Quantidade de cores na sequência
  int pont = 0; // Pontuação do jogador
  
  while(acertou == 1){ // Enquanto o jogador continuar acertando...
    
    // Escreve no LCD a pontuação do jogador
    lcd.print("Taca-lhe pau!!");
    lcd.setCursor(0,1);
    lcd.print("Pontuacao: ");
    lcd.print(pont);
    lcd.setCursor(0,0);
    
    luzes[quant] = random(1, 5); // Escolhe aleatoriamente qual luz será a próxima a acender
    quant = quant + 1; // Adiciona 1 a quantidade de cores na sequência
    
    for(int i = 0; i < quant; i++){ // Loop para acender a sequência de luzes uma a uma
      if(luzes[i] == 1){ // Verifica se é esta a luz que deve acender
        digitalWrite(ledG, HIGH); // Se sim, liga o led correspondente
        tone(buz, 262, 500); // Toca o buzzer na frequência pré-determinada para essa cor
        delay(500);
        digitalWrite(ledG, LOW); // Depois de um tempo, apaga a luz
        delay(200);
      } // A mesma lógica é implementada para as demais luzes
      if(luzes[i] == 2){
        digitalWrite(ledR, HIGH);
        tone(buz, 294, 500);
        delay(500);
        digitalWrite(ledR, LOW);
        delay(200);
      }
      if(luzes[i] == 3){
        digitalWrite(ledY, HIGH);
        tone(buz, 330, 500);
        delay(500);
        digitalWrite(ledY, LOW);
        delay(200);
      }
      if(luzes[i] == 4){
        digitalWrite(ledB, HIGH);
        tone(buz, 349, 500);
        delay(500);
        digitalWrite(ledB, LOW);
        delay(200);
      }
    }

    int aux = 0; // Variável para apontar qual luz deve acender dentro do vetor
    int terminou = 0; // Variável para verificar se o jogador já terminou a sequência de luzes

    while(terminou == 0 && acertou == 1){ // Enquanto ainda faltarem luzes para serem pressionadas e o jogador ainda não tiver perdido...
      int pg = analogRead(pushG); // Lê o botões de cada luz
      int pr = analogRead(pushR);
      int py = analogRead(pushY);
      int pb = analogRead(pushB);

      if(pg == 1023){ // Se o botão dessa luz for pressionado...
        digitalWrite(ledG, HIGH); // O led acende
        tone(buz, 262, 500); // O buzzer toca
        if(luzes[aux] == 1){ // Verifica se o jogador acertou a luz correta
          aux++; // Se sim, a próxima luz é apontada
        }
        else{
          acertou = 0; // Se não, o jogador errou
        }
        while(pg == 1023){pg = analogRead(pushG);} // Enquanto o jogador estiver pressionando o botão da luz, o programa estaciona nesse ponto
        delay(500);
        digitalWrite(ledG, LOW); // A luz se apaga
        delay(500);
      } // A mesma lógica é implementada para todas as luzes
      if(pr == 1023){
        digitalWrite(ledR, HIGH);
        tone(buz, 294, 500);
        if(luzes[aux] == 2){
          aux++;
        }
        else{
          acertou = 0;
        }
        while(pr == 1023){pr = analogRead(pushR);}
        delay(500);
        digitalWrite(ledR, LOW);
        delay(500);
      }
      if(py == 1023){
        digitalWrite(ledY, HIGH);
        tone(buz, 330, 500);
        if(luzes[aux] == 3){
          aux++;
        }
        else{
          acertou = 0;
        }
        while(py == 1023){py = analogRead(pushY);}
        delay(500);
        digitalWrite(ledY, LOW);
        delay(500);
      }
      if(pb == 1023){
        digitalWrite(ledB, HIGH);
        tone(buz, 349, 500);
        if(luzes[aux] == 4){
          aux++;
        }
        else{
          acertou = 0;
        }
        while(pb == 1023){pb = analogRead(pushB);}
        delay(500);
        digitalWrite(ledB, LOW);
        delay(500);
      }
      if(aux >= quant){ // Se o jogador já terminou a sequência...
        terminou = 1;
        pont++; // Sua pontuação sobe
      }
    }
  }
  // Se o jogador errou...
  tone(buz, 415, 3000); // O buzzer toca numa frequência diferente das luzes

  // Escreve-se no LCD que o jogador errou e sua pontuação final
  lcd.clear();
  lcd.print("ERROU!!!");
  lcd.setCursor(0,1);
  lcd.print("Resultado: ");
  lcd.print(pont);
  delay(3000);
}