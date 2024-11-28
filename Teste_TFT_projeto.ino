#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 6, 5, 4, 3, 2);

byte Gota[] = {
  B00100,  //  .   
  B01110,  //  . .
  B11111,  //  . . .
  B11111,  //  . . .
  B01110,  //  . . .
  B00100,  //  . .
  B00010,  //   .
  B00010   //   .
};


void setup() {
  lcd.begin(16, 2);  // Inicializa o LCD
  lcd.createChar(0, Gota);  // Cria o caractere copo
  lcd.clear();

  // Exibe o texto com os emojis do copo no início e no final
  lcd.setCursor(0, 0);  // Define o cursor na primeira linha
  lcd.setCursor(8 - (strlen("Escolha a sua") / 2), 0);   
  lcd.print("Escolha a sua");  // Primeira linha

 
  // Centraliza a palavra "bebida" na segunda linha
  lcd.setCursor(8 - (strlen("0bebida0") / 2), 1);  // Calcula a posição central para "bebida"
  lcd.write(byte(0));
  lcd.print(" bebida ");  // Segunda linha
  lcd.write(byte(0)); 
}

void loop() {
  // Não é necessário código no loop, pois o texto é fixo.
}
