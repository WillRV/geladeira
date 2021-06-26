#include <IRremote.h>
#define pinSensorPresenca 11
void enviarIR(int botao);

//Habilitando send do IR
IRsend irsend;

void setup() {
  pinMode(pinSensorPresenca, INPUT);
  Serial.begin(9600);
  irsend.begin(3, ENABLE_LED_FEEDBACK);
}

void loop() {
  // Quando o Arduino detectar um sinal do sensor de presença, ele executará as seguintes ações
  if(digitalRead(pinSensorPresenca) == HIGH){
    Serial.println("Botao apertado");
    enviarIR(1);
  }
  //Fim sensor de presença
}

void enviarIR(int botao){
  if(botao == 1){
    irsend.sendNEC(0xBD30CF, 32);
    delay(50);
  }
}
