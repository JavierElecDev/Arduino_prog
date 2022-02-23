#include  <Wire.h>//libreria necesaria para el funcionamiento del modulo
#include  <LiquidCrystal_I2C.h>//libreria para el modulo IC2 de La pantalla LCD
int Down = 25, Right = 23, Left = 24, Up = 22;//Pines entrada para lectura de pulsadores
int arTPP, abTPP, drTPP, izTPP;//Variables para la lectura de las Flehas panel frontal
int cXTpp;// contadores para el desplazamiento del cursor tiempo post-purga
int cTpp1, cTpp2, cTpp3, cTpp4;// contadores para almacenar los valores del tiempo de post-purga
int TiempoPostPurga[4];
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);
  cXTpp = 5;

}

void loop() {

  izTPP = digitalRead(Left);
  drTPP = digitalRead(Right);
  lcd.setCursor(0, 0);
  lcd.print(cXTpp);
  //cTp1 = 0; cTp2 = 0; cTp3 = 0; cTp3 = 0;
  lcd.setCursor(4, 0);
  lcd.print("Establezca");
  lcd.setCursor(1, 1);
  lcd.print("Tiempo post-purga:");
  lcd.setCursor(10, 2);
  lcd.print(cTpp1);
  lcd.setCursor(11, 2);
  lcd.print(cTpp2);
  lcd.setCursor(12, 2);
  lcd.print(":");
  lcd.setCursor(13, 2);
  lcd.print(cTpp3);
  lcd.setCursor(14, 2);
  lcd.print(cTpp4);
  if (drTPP == HIGH) {
    cXTpp++;
    delay(300);
  }
  if (izTPP == HIGH) {
    cXTpp--;
    delay(300);
  }

  if (cXTpp < 6) {
    cXTpp = 6;
  }
  if (cXTpp == 6) {
    abTPP = digitalRead(Down);
    arTPP = digitalRead(Up);
    if (arTPP == HIGH) {
      cTpp1++;
      delay(300);
    }
    if (abTPP == HIGH) {
      cTpp1--;
      delay(300);
    }
    if (cTpp1 > 9) {
      cTpp1 = 9;
    }
    else if (cTpp1 < 1) {
      cTpp1 = 0;
    }
    TiempoPostPurga[0] = cTpp1;
    lcd.setCursor(cXTpp + 4, 2);
    lcd.print(cTpp1);
    lcd.setCursor(cXTpp + 4, 3);
    lcd.print("^    ");
  }
  else if (cXTpp == 7) {
    abTPP = digitalRead(Down);
    arTPP = digitalRead(Up);
    if (arTPP == HIGH) {
      cTpp2++;
      delay(300);
    }
    if (abTPP == HIGH) {
      cTpp2--;
      delay(300);
    }
    if (cTpp2 > 9) {
      cTpp2 = 9;
    }
    else if (cTpp2 < 1) {
      cTpp2 = 0;
    }
    TiempoPostPurga[1] = cTpp2;
    lcd.setCursor(cXTpp + 4, 2);
    lcd.print(cTpp2);
    lcd.setCursor(cXTpp + 3, 3);
    lcd.print(" ^   ");
  }
  else if (cXTpp == 8) {
    abTPP = digitalRead(Down);
    arTPP = digitalRead(Up);
    if (arTPP == HIGH) {
      cTpp3++;
      delay(300);
    }
    if (abTPP == HIGH) {
      cTpp3--;
      delay(300);
    }
    if (cTpp3 > 9) {
      cTpp3 = 9;
    }
    else if (cTpp3 < 1) {
      cTpp3 = 0;
    }
    TiempoPostPurga[2] = cTpp3;
    lcd.setCursor(cXTpp + 5, 2);
    lcd.print(cTpp3);
    lcd.setCursor(cXTpp + 2, 3);
    lcd.print("   ^ ");
  }
  else if (cXTpp == 9) {
    abTPP = digitalRead(Down);
    arTPP = digitalRead(Up);
    if (arTPP == HIGH) {
      cTpp4++;
      delay(300);
    }
    if (abTPP == HIGH) {
      cTpp4--;
      delay(300);
    }
    if (cTpp4 > 9) {
      cTpp4 = 9;
    }
    else if (cTpp4 < 1) {
      cTpp4 = 0;
    }
    TiempoPostPurga[3] = cTpp4;
    lcd.setCursor(cXTpp + 5, 2);
    lcd.print(cTpp4);
    lcd.setCursor(cXTpp + 1, 3);
    lcd.print("    ^");
  }
  if (cXTpp > 9) {
    cXTpp = 9;
    lcd.clear();
    lcd.setCursor(4, 2);
    lcd.print(TiempoPostPurga[0]);
    lcd.setCursor(5, 2);
    lcd.print(TiempoPostPurga[1]);
    lcd.setCursor(6, 2);
    lcd.print(TiempoPostPurga[2]);
    lcd.setCursor(7, 2);
    lcd.print(TiempoPostPurga[3]);
  }
}
