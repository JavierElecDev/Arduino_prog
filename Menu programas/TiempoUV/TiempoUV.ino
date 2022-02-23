#include  <Wire.h>//libreria necesaria para el funcionamiento del modulo
#include  <LiquidCrystal_I2C.h>//libreria para el modulo IC2 de La pantalla LCD
int Down = 25, Right = 23, Left = 24, Up = 22;//Pines entrada para lectura de pulsadores
int arTUV, abTUV, drTUV, izTUV;//Variables para la lectura de las Flehas panel frontal
int cXTUV;// contadores para el desplazamiento del cursor tiempo lampara UV
int cTUV1, cTUV2, cTUV3, cTUV4;// contadores para almacenar los valores del tiempo de uso de la lampara UV
int TiempoUV[4];// vector para establecer el tiempo de la lampara UV
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);
  cXTUV = 7;
}

void loop() {
  izTUV = digitalRead(Left);
  drTUV = digitalRead(Right);
  lcd.setCursor(0, 0);
  lcd.print(cXTUV);
  lcd.setCursor(5, 0);
  lcd.print("Establezca");
  lcd.setCursor(1, 1);
  lcd.print("Tiempo Lampara UV:");
  lcd.setCursor(12, 2);
  lcd.print(cTUV1);
  lcd.setCursor(12, 2);
  lcd.print(cTUV2);
  lcd.setCursor(13, 2);
  lcd.print(":");
  lcd.setCursor(14, 2);
  lcd.print(cTUV3);
  lcd.setCursor(15, 2);
  lcd.print(cTUV4);
  if (drTUV == HIGH) {
    cXTUV++;
    delay(300);
  }
  if (izTUV == HIGH) {
    cXTUV--;
    delay(300);
  }

  if (cXTUV < 7) {
    cXTUV = 7;
  }
  if (cXTUV == 7) {
    abTUV = digitalRead(Down);
    arTUV = digitalRead(Up);
    if (arTUV == HIGH) {
      cTUV1++;
      delay(300);
    }
    if (abTUV == HIGH) {
      cTUV1--;
      delay(300);
    }
    if (cTUV1 > 9) {
      cTUV1 = 9;
    }
    else if (cTUV1 < 1) {
      cTUV1 = 0;
    }
    TiempoUV[0] = cTUV1;
    lcd.setCursor(cXTUV + 4, 2);
    lcd.print(cTUV1);
    lcd.setCursor(cXTUV + 4, 3);
    lcd.print("^    ");
  }
  else if (cXTUV == 8) {
    abTUV = digitalRead(Down);
    arTUV = digitalRead(Up);
    if (arTUV == HIGH) {
      cTUV2++;
      delay(300);
    }
    if (abTUV == HIGH) {
      cTUV2--;
      delay(300);
    }
    if (cTUV2 > 9) {
      cTUV2 = 9;
    }
    else if (cTUV2 < 1) {
      cTUV2 = 0;
    }
    TiempoUV[1] = cTUV2;
    lcd.setCursor(cXTUV + 4, 2);
    lcd.print(cTUV2);
    lcd.setCursor(cXTUV + 3, 3);
    lcd.print(" ^   ");
  }
  else if (cXTUV == 9) {
    abTUV = digitalRead(Down);
    arTUV = digitalRead(Up);
    if (arTUV == HIGH) {
      cTUV3++;
      delay(300);
    }
    if (abTUV == HIGH) {
      cTUV3--;
      delay(300);
    }
    if (cTUV3 > 9) {
      cTUV3 = 9;
    }
    else if (cTUV3 < 1) {
      cTUV3 = 0;
    }
    TiempoUV[2] = cTUV3;
    lcd.setCursor(cXTUV + 5, 2);
    lcd.print(cTUV3);
    lcd.setCursor(cXTUV + 2, 3);
    lcd.print("   ^ ");
  }
  else if (cXTUV == 10) {
    abTUV = digitalRead(Down);
    arTUV = digitalRead(Up);
    if (arTUV == HIGH) {
      cTUV4++;
      delay(300);
    }
    if (abTUV == HIGH) {
      cTUV4--;
      delay(300);
    }
    if (cTUV4 > 9) {
      cTUV4 = 9;
    }
    else if (cTUV4 < 1) {
      cTUV4 = 0;
    }
    TiempoUV[3] = cTUV4;
    lcd.setCursor(cXTUV + 5, 2);
    lcd.print(cTUV4);
    lcd.setCursor(cXTUV + 1, 3);
    lcd.print("    ^");
  }
  if (cXTUV > 10) {
    cXTUV = 9;
    lcd.clear();
    lcd.setCursor(4, 2);
    lcd.print(TiempoUV[0]);
    lcd.setCursor(5, 2);
    lcd.print(TiempoUV[1]);
    lcd.setCursor(6, 2);
    lcd.print(TiempoUV[2]);
    lcd.setCursor(7, 2);
    lcd.print(TiempoUV[3]);
  }

}
