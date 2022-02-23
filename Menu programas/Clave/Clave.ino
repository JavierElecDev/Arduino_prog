#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
int Down = 25, Right = 23, Left = 24, Up = 22;
int arriba, abajo, derecha, izquierda;
int clave[4] = {2, 6, 7, 3}; //vector que contiene la clave
int cX;// contador para el desplazamiento del cursor
int c1, c2, c3, c4;// contadores para almacenar los digitos de la clave
int ingreso[4];

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);
  cX = 6;

}

void loop() {
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  lcd.setCursor(0, 0);
  lcd.print(cX);
  if (derecha == HIGH) {
    cX++;
    delay(300);
  }
  if (izquierda == HIGH) {
    cX--;
    delay(300);
  }
  if (cX < 6) {
    cX = 6;
  }

  if (cX == 6) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      c1++;
      delay(300);
    }
    if (abajo == HIGH) {
      c1--;
      delay(300);
    }
    if (c1 > 9) {
      c1 = 9;
    }
    else if (c1 < 0) {
      c1 = 0;
    }
    ingreso[0] = c1;
    lcd.setCursor(cX, 1);
    lcd.print(c1);
    lcd.setCursor(cX+4, 2);
    lcd.print("^   ");
  }
  else if (cX == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      c2++;
      delay(300);
    }
    if (abajo == HIGH) {
      c2--;
      delay(300);
    }
    if (c2 > 9) {
      c2 = 9;
    }
    else if (c2 < 0) {
      c2 = 0;
    }
    ingreso[1] = c2;
    lcd.setCursor(cX, 1);
    lcd.print(c2);
    lcd.setCursor(cX+3, 2);
    lcd.print(" ^  ");
  }
  else if (cX == 8) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      c3++;
      delay(300);
    }
    if (abajo == HIGH) {
      c3--;
      delay(300);
    }
    if (c3 > 9) {
      c3 = 9;
    }
    else if (c3 < 0) {
      c3 = 0;
    }
    ingreso[2] = c3;
    lcd.setCursor(cX, 1);
    lcd.print(c3);
    lcd.setCursor(cX+2, 2);
    lcd.print("  ^ ");

  }
  else if (cX == 9) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      c4++;
      delay(300);
    }
    if (abajo == HIGH) {
      c4--;
      delay(300);
    }
    if (c4 > 9) {
      c4 = 9;
    }
    else if (c4 < 0) {
      c4 = 0;
    }
    ingreso[3] = c4;
    lcd.setCursor(cX, 1);
    lcd.print(c4);
    lcd.setCursor(cX+1, 2);
    lcd.print("   ^");
  }
  if (cX > 9) {
    lcd.setCursor(4, 2);
    lcd.print(ingreso[0]);
    lcd.setCursor(5, 2);
    lcd.print(ingreso[1]);
    lcd.setCursor(6, 2);
    lcd.print(ingreso[2]);
    lcd.setCursor(7, 2);
    lcd.print(ingreso[3]);
    if (clave[0] == ingreso[0] && clave[1] == ingreso[1] && clave[2] == ingreso[2] && clave[3] == ingreso[3]) {
      lcd.setCursor(4, 3);
      lcd.print("clave Correcta");
    }
    else {
      lcd.setCursor(4, 3);
      lcd.print("clave incorrecta");
    }
  }
}
