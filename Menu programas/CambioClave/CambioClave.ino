#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
int Down = 25, Right = 23, Left = 24, Up = 22;
int arriba, abajo, derecha, izquierda;
int clave[4] = {2, 6, 7, 3}; //vector que contiene la clave
int ccX;// contador para el desplazamiento del cursor
int cc1, cc2, cc3, cc4;// contadores para almacenar los digitos de la clave
int NuevaClave[4];// vector que almacena la nueva clave
int cambiar, nueva;//variable para acceder al cambio e clave

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);
  ccX = 8;
}

void loop() {
  while (cambiar != 1) {
    izquierda = digitalRead(Left);
    derecha = digitalRead(Right);
    lcd.setCursor(0, 0);
    lcd.print("Ingrese clave Actual");
    lcd.setCursor(8, 1);
    lcd.print(cc1);
    lcd.setCursor(9, 1);
    lcd.print(cc2);
    lcd.setCursor(10, 1);
    lcd.print(cc3);
    lcd.setCursor(11, 1);
    lcd.print(cc4);
    /*  if (izquierda == HIGH && ccX == 5) {
      cambiar = 1;
      ccX = 5;
      c1 = 0; c2 = 0; c3 = 0; c4 = 0;
      lcd.clear();
      }*/
    if (derecha == HIGH) {
      ccX++;
      delay(300);
    }
    if (izquierda == HIGH) {
      ccX--;
      delay(300);
    }
    if (ccX < 8) {
      ccX = 7;
    }

    if (ccX == 8) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc1++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc1--;
        delay(300);
      }
      if (cc1 > 9) {
        cc1 = 9;
      }
      else if (cc1 < 0) {
        cc1 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc1);
      lcd.setCursor(ccX + 4, 2);
      lcd.print("^   ");
    }
    else if (ccX == 9) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc2++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc2--;
        delay(300);
      }
      if (cc2 > 9) {
        cc2 = 9;
      }
      else if (cc2 < 0) {
        cc2 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc2);
      lcd.setCursor(ccX + 3, 2);
      lcd.print(" ^  ");
    }
    else if (ccX == 10) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc3++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc3--;
        delay(300);
      }
      if (cc3 > 9) {
        cc3 = 9;
      }
      else if (cc3 < 0) {
        cc3 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc3);
      lcd.setCursor(ccX + 2, 2);
      lcd.print("  ^ ");

    }
    else if (ccX == 11) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc4++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc4--;
        delay(300);
      }
      if (cc4 > 9) {
        cc4 = 9;
      }
      else if (cc4 < 0) {
        cc4 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc4);
      lcd.setCursor(ccX + 1, 2);
      lcd.print("   ^");
    }
    if (ccX > 11) {
      if (clave[0] == cc1 && clave[1] == cc2 && clave[2] == cc3 && clave[3] == cc4) {
        lcd.setCursor(6, 3);
        lcd.print("ClAVE CORRECTA");
        delay(3000);
        ccX = 8;
        cambiar = 1;
        nueva = 1;
        cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0;
        lcd.clear();
      }
      else {
        lcd.setCursor(4, 3);
        lcd.print("CLAVE INCORRECTA");
        delay(3000);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Ingrese la Clave:");
        ccX = 8;
      }
    }
  }
  while (nueva == 1) {
    izquierda = digitalRead(Left);
    derecha = digitalRead(Right);
    lcd.setCursor(0, 0);
    lcd.print(ccX);
    lcd.setCursor(0, 0);
    lcd.print("Ingrese Clave Nueva");
    lcd.setCursor(8, 1);
    lcd.print(cc1);
    lcd.setCursor(9, 1);
    lcd.print(cc2);
    lcd.setCursor(10, 1);
    lcd.print(cc3);
    lcd.setCursor(11, 1);
    lcd.print(cc4);
    if (derecha == HIGH) {
      ccX++;
      delay(300);
    }
    if (izquierda == HIGH) {
      ccX--;
      delay(300);
    }
    if (ccX < 8) {
      ccX = 7;
    }

    if (ccX == 8) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc1++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc1--;
        delay(300);
      }
      if (cc1 > 9) {
        cc1 = 9;
      }
      else if (cc1 < 0) {
        cc1 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc1);
      lcd.setCursor(ccX + 4, 2);
      lcd.print("^   ");
    }
    else if (ccX == 9) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc2++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc2--;
        delay(300);
      }
      if (cc2 > 9) {
        cc2 = 9;
      }
      else if (cc2 < 0) {
        cc2 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc2);
      lcd.setCursor(ccX + 3, 2);
      lcd.print(" ^  ");
    }
    else if (ccX == 10) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc3++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc3--;
        delay(300);
      }
      if (cc3 > 9) {
        cc3 = 9;
      }
      else if (cc3 < 0) {
        cc3 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc3);
      lcd.setCursor(ccX + 2, 2);
      lcd.print("  ^ ");

    }
    else if (ccX == 11) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc4++;
        delay(300);
      }
      if (abajo == HIGH) {
        cc4--;
        delay(300);
      }
      if (cc4 > 9) {
        cc4 = 9;
      }
      else if (cc4 < 0) {
        cc4 = 0;
      }
      lcd.setCursor(ccX, 1);
      lcd.print(cc4);
      lcd.setCursor(ccX + 1, 2);
      lcd.print("   ^");
    }
    if (ccX > 11) {
      NuevaClave[0] = cc1;
      NuevaClave[1] = cc2;
      NuevaClave[2] = cc3;
      NuevaClave[3] = cc4;
      lcd.setCursor(4, 3);
      lcd.print("CLAVE CAMBIADA");
      ccX = 7;
      cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0;
      cambiar = 1;
      lcd.setCursor(4, 2);
      lcd.print(NuevaClave[0]);
      lcd.setCursor(5, 2);
      lcd.print(NuevaClave[1]);
      lcd.setCursor(6, 2);
      lcd.print(NuevaClave[2]);
      lcd.setCursor(7, 2);
      lcd.print(NuevaClave[3]);
      delay(3000);
      lcd.clear();
    }
  }
}
