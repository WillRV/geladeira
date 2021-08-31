#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define pinSensorPresenca 4
#define ledIR 3
#define pinLDR A0
//LCD
#define endereco 0x27
#define linhas 2
#define colunas 16

//Variáveis e Funções
int pinReceptorIR = 8;
int pessoasQueAbriram = 0;
void enviarIR(int botao);
void receberIR();
void iniciarPendrive();

//Instanciações
IRsend irsend; //Send do IR
IRrecv irrecv(pinReceptorIR); //Receptor IR
decode_results results; //Faz parte do receptor do IR
LiquidCrystal_I2C lcd(endereco, colunas, linhas);
//Começo do código
void setup() {  
  Serial.begin(9600);
  pinMode(pinSensorPresenca, INPUT); //Declara o pino do Sensor de Presença como Input
  irrecv.enableIRIn(); // Inicializa o receptor iR
  irsend.begin(ledIR, ENABLE_LED_FEEDBACK); //Diz qual porta o led IR está (para enviar o IR)
  lcd.init();
  lcd.backlight();
  lcd.clear();

  //lcd.setCursor(0, 1);
  //lcd.print("Willian");
  lcd.print(pessoasQueAbriram);
  delay(3000);
  iniciarPendrive(); //Inicia os videos na TV
}

void loop() { 
  //receberIR();
  if(digitalRead(pinSensorPresenca) == HIGH){ // Quando o Arduino detectar um sinal do sensor de presença, ele executará as seguintes ações
    enviarIR(1);
    delay(6000);
  }
  
  if (LDR() > 240){
    //A porta da geladeira abriu
    while(1){ //Entra em um loop infinito, e só contará quando fizer o movimento completo da porta (abrir e fechar)
      if(LDR() < 240){
        pessoasQueAbriram = pessoasQueAbriram + 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String("Doacoes: ") + String(pessoasQueAbriram));
        enviarIR(3);
        break;
      }
    }
  }
}

void enviarIR(int botao){
  if(botao == 1){ //Video da chamada
    
    long tempoAnterior = millis();
    //Sinal para avançar
    irsend.sendNEC(0xFD8A75, 32);
    
    //Mostrando no LCD que está sendo reproduzido o video 2
    lcd.clear();
    lcd.print("Reproduzindo 2");
    lcd.setCursor(0, 1);
    lcd.print("Chamada");

    while(1){
      
    //delay(16000); //Segundos do vídeo da chamada
    if((millis() - tempoAnterior) > 16000){
      irsend.sendNEC(0xFD0AF5, 32); //Voltar video para o institucional
      lcd.clear();
      lcd.print(String("Doacoes: ") + String(pessoasQueAbriram));
      break;
    }

    if (LDR() > 240){
      //A porta da geladeira abriu
      irsend.sendNEC(0xFD0AF5, 32); //Voltar video para o institucional
      while(1){ //Entra em um loop infinito, e só contará quando fizer o movimento completo da porta (abrir e fechar)
        if(LDR() < 240){
          pessoasQueAbriram = pessoasQueAbriram + 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(String("Doacoes: ") + String(pessoasQueAbriram));
          
          //Vai para o agradecimento
          //Avançar 2 vezes para o agradecimento
            irsend.sendNEC(0xFD8A75, 32);
            lcd.clear();
            lcd.print("Reproduzindo 3");
            lcd.setCursor(0, 1);
            lcd.print("Agradecimento");
            delay(250);
            irsend.sendNEC(0xFD8A75, 32);
            delay (11000); //Tempo do video de agradecimento
            irsend.sendNEC(0xFD0AF5, 32);
            delay(250);
            irsend.sendNEC(0xFD0AF5, 32);
            lcd.clear();
            lcd.print(String("Doacoes: ") + String(pessoasQueAbriram));
          break;
        }
      }
    }
  }
  }else if (botao == 2){
    //Sinal para retroceder
    irsend.sendNEC(0xFD0AF5, 32);
    delay(1000);
  }else if (botao == 3){
    //Avançar 2 vezes para o agradecimento
    irsend.sendNEC(0xFD8A75, 32);
    lcd.clear();
    lcd.print("Reproduzindo 3");
    lcd.setCursor(0, 1);
    lcd.print("Agradecimento");
    delay(250);
    irsend.sendNEC(0xFD8A75, 32);
    delay (11000); //Tempo do video de agradecimento
    irsend.sendNEC(0xFD0AF5, 32);
    delay(250);
    irsend.sendNEC(0xFD0AF5, 32);
    lcd.clear();
    lcd.print(String("Doacoes: ") + String(pessoasQueAbriram));
  }
}

void receberIR(){
  if (irrecv.decode(&results)) {
    Serial.print("Valor recebido pelo IR: ");
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}

int LDR(){
  int valorLDR = analogRead(pinLDR);
  Serial.print("Valor LDR: ");
  Serial.println(valorLDR);
  return valorLDR;
}

void iniciarPendrive(){
  irsend.sendNEC(0xFDC03F, 32); // LIGAR
  delay(15000);
  irsend.sendNEC(0xFD42BD, 32);//DIREITA
  delay(400);
  irsend.sendNEC(0xFD42BD, 32);//DIREITA
  delay(400);
  irsend.sendNEC(0xFD827D, 32); //ENTER
  delay(400);
  irsend.sendNEC(0xFD827D, 32); //ENTER
  delay(400);
  irsend.sendNEC(0xFD42BD, 32);//DIREITA
  delay(400);
  irsend.sendNEC(0xFD827D, 32); //ENTER
  delay(400);
  irsend.sendNEC(0xFD42BD, 32);//DIREITA
  delay(400);
  irsend.sendNEC(0xFD827D, 32); //ENTER
  delay(400);
  irsend.sendNEC(0xFD42BD, 32);//DIREITA
  delay(400);
  irsend.sendNEC(0xFD827D, 32); //ENTER
  delay(400);
  irsend.sendNEC(0xFD02FD, 32); //ESQUERDA
  delay(400);
  irsend.sendNEC(0xFD02FD, 32); //ESQUERDA
  delay(400);
  irsend.sendNEC(0xFD4AB5, 32); //PLAY  
}
