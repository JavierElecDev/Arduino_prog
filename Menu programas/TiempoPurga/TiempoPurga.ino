#include  <Wire.h>//libreria necesaria para el funcionamiento del modulo
#include  <LiquidCrystal_I2C.h>//libreria para el modulo IC2 de La pantalla LCD
int Down = 25, Right = 23, Left = 24, Up = 22;//Pines entrada para lectura de pulsadores
int arTP, abTP, drTP, izTP;//Variables para la lectura de las Flehas panel frontal
int cXTp, regresoTp;// contadores para el desplazamiento del cursor tiempo purga
int cTp1, cTp2, cTp3, cTp4;// contadores para almacenar los valores del tiempo de purga
int TiempoPurga[4];
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {
  lcd.begin(16, 4);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);
  cXTp = 6;

}

void loop() {

  izTP = digitalRead(Left);
  drTP = digitalRead(Right);
  lcd.setCursor(0, 0);
  lcd.print(cXTp);
  lcd.setCursor(4, 0);
  lcd.print("Establezca");
  lcd.setCursor(2, 1);
  lcd.print("Tiempo de purga:");
  lcd.setCursor(10, 2);
  lcd.print(cTp1);
  lcd.setCursor(11, 2);
  lcd.print(cTp2);
  lcd.setCursor(12, 2);
  lcd.print(":");
  lcd.setCursor(13, 2);
  lcd.print(cTp3);
  lcd.setCursor(14, 2);
  lcd.print(cTp4);
  if (drTP == HIGH) {
    cXTp++;
    delay(300);
  }
  if (izTP == HIGH) {
    cXTp--;
    delay(300);
  }

  if (cXTp < 6) {
    cXTp = 6;
  }
  if (cXTp == 6) {
    abTP = digitalRead(Down);
    arTP = digitalRead(Up);
    if (arTP == HIGH) {
      cTp1++;
      delay(300);
    }
    if (abTP == HIGH) {
      cTp1--;
      delay(300);
    }
    if (cTp1 > 9) {
      cTp1 = 9;
    }
    else if (cTp1 < 1) {
      cTp1 = 0;
    }
    TiempoPurga[0] = cTp1;
    lcd.setCursor(cXTp + 4, 2);
    lcd.print(cTp1);
    lcd.setCursor(cXTp + 4, 3);
    lcd.print("^    ");
  }
  else if (cXTp == 7) {
    abTP = digitalRead(Down);
    arTP = digitalRead(Up);
    if (arTP == HIGH) {
      cTp2++;
      delay(300);
    }
    if (abTP == HIGH) {
      cTp2--;
      delay(300);
    }
    if (cTp2 > 9) {
      cTp2 = 9;
    }
    else if (cTp2 < 1) {
      cTp2 = 0;
    }
    TiempoPurga[1] = cTp2;
    lcd.setCursor(cXTp + 4, 2);
    lcd.print(cTp2);
    lcd.setCursor(cXTp + 3, 3);
    lcd.print(" ^   ");
  }
  else if (cXTp == 8) {
    abTP = digitalRead(Down);
    arTP = digitalRead(Up);
    if (arTP == HIGH) {
      cTp3++;
      delay(300);
    }
    if (abTP == HIGH) {
      cTp3--;
      delay(300);
    }
    if (cTp3 > 9) {
      cTp3 = 9;
    }
    else if (cTp3 < 1) {
      cTp3 = 0;
    }
    TiempoPurga[2] = cTp3;
    lcd.setCursor(cXTp + 5, 2);
    lcd.print(cTp3);
    lcd.setCursor(cXTp + 2, 3);
    lcd.print("   ^ ");
  }
  else if (cXTp == 9) {
    abTP = digitalRead(Down);
    arTP = digitalRead(Up);
    if (arTP == HIGH) {
      cTp4++;
      delay(300);
    }
    if (abTP == HIGH) {
      cTp4--;
      delay(300);
    }
    if (cTp4 > 9) {
      cTp4 = 9;
    }
    else if (cTp4 < 1) {
      cTp4 = 0;
    }
    TiempoPurga[3] = cTp4;
    lcd.setCursor(cXTp + 5, 2);
    lcd.print(cTp4);
    lcd.setCursor(cXTp + 1, 3);
    lcd.print("    ^");
  }
  if (cXTp > 9) {
    cXTp = 9;
    lcd.clear();
    lcd.setCursor(4, 2);
    lcd.print(TiempoPurga[0]);
    lcd.setCursor(5, 2);
    lcd.print(TiempoPurga[1]);
    lcd.setCursor(6, 2);
    lcd.print(TiempoPurga[2]);
    lcd.setCursor(7, 2);
    lcd.print(TiempoPurga[3]);
  }
}
