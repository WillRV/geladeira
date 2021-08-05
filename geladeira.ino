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
    //Sinal para avançar
    irsend.sendNEC(0xFD8A75, 32);
    
    lcd.clear();
    lcd.print("Reproduzindo 2");
    lcd.setCursor(0, 1);
    lcd.print("Chamada");
    
    delay(16000); //Segundos do vídeo da chamada
    irsend.sendNEC(0xFD0AF5, 32); //Voltar video para o estático
    lcd.clear();
    lcd.print(String("Doacoes: ") + String(pessoasQueAbriram));
    
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
