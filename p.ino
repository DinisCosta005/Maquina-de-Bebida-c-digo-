#include <LiquidCrystal.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Definições de pinos dos sensores de moedas
#define trigPin1 13  // Sensor para 0,50€
#define echoPin1 12
#define trigPin2 11  // Sensor para 1,00€
#define echoPin2 10
#define trigPin3 9   // Sensor para 2,00€
#define entradaPin 7 // Sensor de entrada da moeda

// Definições para as bombas de bebidas
int Bebida1 = 1; // Bombeamento para bebida 1
int Bebida2 = 2; // Bombeamento para bebida 2
int Bebida3 = 3; // Bombeamento para bebida 3

// Definição do LCD
LiquidCrystal_I2C lcd(8,6,5,4,3,2);
unsigned long lastUpdate


// Variáveis
float total = 0;
int estado = 0;
int TempoBombas = 500;
SoftwareSerial EEBlue(0, 1); // RX | TX (Bluetooth)
////////////////////////TFT////////////////////////////////////////
byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
///////////////////////////////////////////////////////////////////

void setup() {
///////////////////////////TFT/////////////////////////////////////
    lcd.init();
  lcd.clear();
  lcd.setBacklight(HIGH);

  // Criando o caractere personalizado para o coração na memória do LCD
  lcd.createChar(0, Heart);

  // Exibindo o texto com o coração
  lcd.setCursor(0, 0);
  lcd.print("Escolha ");
  lcd.write(byte(0));  // Exibindo o coração
  
  lcd.setCursor(0, 1);
  lcd.print("a sua bebida");
}
/////////////////////////////////////////////////////////////////
  Serial.begin(9600);

  // Inicialização dos sensores de distância (moedas)
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  pinMode(entradaPin, INPUT);  // Sensor de entrada

  // Inicialização do LCD
  lcd.init(); 
  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("    ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print("Máquina de Moedas");
  delay(2000);
  lcd.clear();

  // Configuração das bombas de bebidas
  pinMode(Bebida1, OUTPUT);
  pinMode(Bebida2, OUTPUT);
  pinMode(Bebida3, OUTPUT);

  digitalWrite(Bebida1, HIGH);
  digitalWrite(Bebida2, HIGH);
  digitalWrite(Bebida3, HIGH);

  EEBlue.begin(9600); // Inicia comunicação Bluetooth
  Serial.println("As portas do bluetooth estão abertas. Conecte-se ao HC-05 com a chave 0000.");
}

void loop() {
  // Verifica se uma moeda foi detectada no sensor de entrada
  int moedaDetectada = digitalRead(entradaPin);  // Detecta a chegada da moeda

  if (moedaDetectada == HIGH) {  // Se a moeda foi detectada
    Serial.println("Moeda detectada, começando a contagem...");

    long distance1 = measureDistance(trigPin1, echoPin1);
    long distance2 = measureDistance(trigPin2, echoPin2);
    long distance3 = measureDistance(trigPin3, echoPin3);

    // Verifica se uma moeda passou e qual o valor baseado no tamanho
    if (distance1 > 0 && distance1 < 30) {
      if (distance1 >= 23 && distance1 <= 24) {  // Moeda de 1€
        total += 1.0;
        Serial.println("Moeda de 1 Euro detectada");
      }
    }

    if (distance2 > 0 && distance2 < 30) {
      if (distance2 >= 24 && distance2 <= 25) {  // Moeda de 0,50€
        total += 0.5;
        Serial.println("Moeda de 0.5 Euro detectada");
      }
    }

    if (distance3 > 0 && distance3 < 30) {
      if (distance3 >= 25 && distance3 <= 27) {  // Moeda de 2€
        total += 2.0;
        Serial.println("Moeda de 2 Euros detectada");
      }
    }

    // Atualiza o LCD com o total
    lcd.setCursor(0, 0);
    lcd.print("Total em Euros:");
    lcd.setCursor(0, 1);
    lcd.print(total);

    // Verifica se o valor total é suficiente para a bebida
    if (total >= 2.0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Valor suficiente!");
      lcd.setCursor(0, 1);
      lcd.print("Entregar bebida.");
      delay(5000);  // Simula a entrega da bebida
      total -= 2.0;  // Deduz o valor
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Insira mais moedas");
    }

    delay(500);  // Delay para evitar leituras duplicadas
  } else {
    // Se a moeda não foi detectada, continua aguardando
    lcd.setCursor(0, 0);
    lcd.print("Esperando moeda...");
    lcd.setCursor(0, 1);
    lcd.print("Valor total: ");
    lcd.print(total);
  }

  // Controle via Bluetooth para escolha da bebida
  if (EEBlue.available()) {
    estado = EEBlue.read();
    Serial.println(estado);

    // Bebida1
    if (estado == 'a') {
      Serial.println("Bebida1 em preparação");
      MixCocktail(5000, 0, 0); // Bebida1, Bebida2, Bebida3
    }

    // Bebida2
    if (estado == 'b') {
      Serial.println("Bebida2 em preparação");
      MixCocktail(0, 5000, 0); // Bebida1, Bebida2, Bebida3
    }

    // Bebida3
    if (estado == 'c') {
      Serial.println("Bebida3 em preparação");
      MixCocktail(0, 0, 5000); // Bebida1, Bebida2, Bebida3
    }

    // Bebida1 + Bebida2
    if (estado == 'd') {
      Serial.println("Bebida1+Bebida2 em preparação");
      MixCocktail(3000, 3000, 0); // Bebida1, Bebida2, Bebida3
    }

    // Bebida1 + Bebida3
    if (estado == 'e') {
      Serial.println("Bebida1+Bebida3 em preparação");
      MixCocktail(3000, 0, 3000); // Bebida1, Bebida2, Bebida3
    }

    // Bebida2 + Bebida3
    if (estado == 'f') {
      Serial.println("Bebida2+Bebida3 em preparação");
      MixCocktail(0, 3000, 3000); // Bebida1, Bebida2, Bebida3
    }

    // Bebida1 + Bebida2 + Bebida3
    if (estado == 'g') {
      Serial.println("Bebida1+Bebida2+Bebida3 em preparação");
      MixCocktail(2000, 2000, 2000); // Bebida1, Bebida2, Bebida3
    }
  }
}

void MixCocktail(int DelayTimeBebida1, int DelayTimeBebida2, int DelayTimeBebida3) {
  estado = 0;
  delay(800);

  // Bebida1
  digitalWrite(Bebida1, LOW);
  delay(DelayTimeBebida1);
  digitalWrite(Bebida1, HIGH);
  delay(TempoBombas);

  // Bebida2
  digitalWrite(Bebida2, LOW);
  delay(DelayTimeBebida2);
  digitalWrite(Bebida2, HIGH);
  delay(TempoBombas);

  // Bebida3
  digitalWrite(Bebida3, LOW);
  delay(DelayTimeBebida3);
  digitalWrite(Bebida3, HIGH);
  delay(TempoBombas);

  Serial.println("A bebida está pronta!");
}

long measureDistance(int trigPin, int echoPin) {
  long duration, distance;

  // Envia o pulso para medir a distância
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Recebe o eco do sinal e calcula a distância
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58;

  return distance;
}
