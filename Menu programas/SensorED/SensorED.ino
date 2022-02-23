#include  <Wire.h>//libreria necesaria para el funcionamiento del modulo
#include  <LiquidCrystal_I2C.h>//libreria para el modulo IC2 de La pantalla LCD
int Down = 25,Left = 24, Up = 22;//Pines entrada para lectura de pulsadores
int arSED, abSED,izSED;//Variables para ajuste del sensor E/D
float cSED;// contadores para el desplazamiento del cursor tiempo post-purga
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Down, INPUT);

}

void loop() {
  arSED = digitalRead(Up);
  izSED = digitalRead(Left);
  abSED = digitalRead(Down);
  lcd.setCursor(0,0);
  lcd.print("Sensor Flujo E/D");
    lcd.setCursor(0,1);
  lcd.print("Fabrica:");
    lcd.setCursor(4,2);
  lcd.print("Actual:");
    lcd.setCursor(4,3);
  lcd.print("Ajuste: ");
   if (arSED == HIGH) {
    cSED += 0.01;
    delay(30);
  }
  if (abSED == HIGH) {
    cSED -= 0.01;
    delay(30);
  }
    lcd.setCursor(13,3);
  lcd.print(cSED,2);
}
