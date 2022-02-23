#include  <Wire.h>//libreria necesaria para el funcionamiento del modulo
#include  <LiquidCrystal_I2C.h>//libreria para el modulo IC2 de La pantalla LCD
int Down = 25, Left = 24, Up = 22; //Pines entrada para lectura de pulsadores
int arSE, abSE, izSE; //Variables para ajuste del parametro de ExFlow
float cSE;// variable calibracion ExFlow
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Down, INPUT);

}

void loop() {
  arSE = digitalRead(Up);
  izSE = digitalRead(Left);
  abSE = digitalRead(Down);
  lcd.setCursor(0, 0);
  lcd.print("Ajuste Exaust");
  lcd.setCursor(0, 1);
  lcd.print("Fabrica:");
  lcd.setCursor(4, 2);
  lcd.print("Actual:");
  lcd.setCursor(4, 3);
  lcd.print("Ajuste: ");
  if (arSE == HIGH) {
    cSE += 0.01;
    delay(30);
  }
  if (abSE == HIGH) {
    cSE -= 0.01;
    delay(30);
  }
  lcd.setCursor(13, 3);
  lcd.print(cSE, 2);
}
