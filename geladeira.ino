#include <IRremote.h>
#define pinSensorPresenca 11
#define ledIR 3
int pinReceptorIR = 8;
void enviarIR(int botao);
void receberIR();

//Habilitando send do IR
IRsend irsend;
//Instanciando receptor IR
IRrecv irrecv(pinReceptorIR);
decode_results results;

void setup() {
  pinMode(pinSensorPresenca, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Inicializa o receptor iR
  irsend.begin(ledIR, ENABLE_LED_FEEDBACK); //Diz qual porta o led IR está (para enviar o IR)
}

void loop() { 
  receberIR();
  if(digitalRead(pinSensorPresenca) == HIGH){ // Quando o Arduino detectar um sinal do sensor de presença, ele executará as seguintes ações
    enviarIR(2);
  }
}

void enviarIR(int botao){
  if(botao == 1){
    //Sinal para avançar
    irsend.sendNEC(0xFD8A75, 32);
    delay(1000);
  }else if (botao == 2){
    //Sinal para retroceder
    irsend.sendNEC(0xFD0AF5, 32);
    delay(1000);
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
