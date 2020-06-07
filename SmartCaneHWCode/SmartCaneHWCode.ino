#include "BluetoothSerial.h"
#include <Ultrasonic.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int received;// received value will be stored in this variable
char receivedChar;// received value will be stored as CHAR in this variable

int count = 0;
int count2 = 0;
int countHeight = 0;
int userHeight = 170;
int alertUser = 100;

//#define PIN_BUZZER 21
#define PIN_BTN 18
#define PIN_BTN2 21
//conexão dos pinos para o sensor ultrasonico
#define PIN_TRIGGER   22
#define PIN_ECHO      23

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);

void setup() {
  Serial.begin(9600);
  SerialBT.begin("SmartCane"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  configurarPinos();
}

void configurarPinos()
{
//  pinMode(PIN_BUZZER,    OUTPUT);
  pinMode(PIN_BTN,     INPUT);
  pinMode(PIN_BTN2,     INPUT);
}

void loop() {
  if (countHeight > 10)
  {
    getDistance();
    countHeight = 0;
  }


  byte btnStatus = digitalRead(PIN_BTN);
  byte btn2Status = digitalRead(PIN_BTN2);

  if (btnStatus == LOW) {
    count = 0;
  }
  if (btnStatus == HIGH && count == 0)
  {
    Serial.println("a");
    SerialBT.println("a");// write on BT app
    count++;
  }
  if (btn2Status == LOW) {
    count2 = 0;
  }
  if (btn2Status == HIGH && count2 == 0)
  {
    Serial.println("b");
    SerialBT.println("b");// write on BT app
    count2++;
  }

  if (SerialBT.available()) {
    String receivedHeight = "";
    for (int i = 0; i < 3; i++) {
      receivedHeight += (char)SerialBT.read();
    }
    int newHeight = receivedHeight.toInt();
    if (newHeight > 100) {
      userHeight = newHeight;
      alertUser = userHeight * 0.58;
      Serial.println(alertUser);

    }
  }
  countHeight++;
  delay(100);
}

void getDistance()
{
  digitalWrite(PIN_TRIGGER, LOW); 
 delayMicroseconds(2); 
  //faz a leitura das informacoes do sensor (em cm)
  digitalWrite(PIN_TRIGGER, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
  delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
  digitalWrite(PIN_TRIGGER, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
  //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
  //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
  //NA VARIAVEL "distancia"
  long distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
  Serial.println(distancia);// write on BT app

  if (distancia <= alertUser)
  {
//    digitalWrite(PIN_BUZZER, HIGH);// turn the buzzer on
    SerialBT.println(distancia);// write on BT app
  }
  else if (distancia > alertUser) {
//    digitalWrite(PIN_BUZZER, LOW);// turn the buzzer of
  }
}
