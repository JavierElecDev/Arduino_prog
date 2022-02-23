#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
int Down = 25, Right = 23, Left = 24, Up = 22;//Pines entrada para lectura de pulsadores
int arriba, abajo, derecha, izquierda;//Flehas panel frontal
int arSA, drSA, izSA , abSA, cSA;// Felchas y contador menu Ajustes
int arSC, drSC, izSC, abSC , cSC;// Felchas y contador menu Clave
int arSM, drSM, izSM, abSM , cSM;// Felchas y contador menu Mantenimineto
int c1 = 1, c2 = 0, opcion = 0;// Contadores menu general
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  //Configuracion inical LCD 20x4
  lcd.begin(16, 4);
  lcd.setCursor(4, 0);
  lcd.print("JP InGlobal");
  lcd.setCursor(0, 1);
  lcd.print("Cabina Seguridad Bio");
  lcd.setCursor(9, 2);
  lcd.print("Clase 2");
  lcd.setCursor(9, 3);
  lcd.print("Tipo A2");
  delay(3000);
  lcd.clear();
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);
  menu();
}

void loop() {
  arriba = digitalRead(Up);
  derecha = digitalRead(Right);
  abajo = digitalRead(Down);
  if (arriba == HIGH)
  {
    c1++;
    menu();
    delay(500);
  }
  if (abajo == HIGH)
  {
    c1--;
    menu();
    delay(500);
  }
  if (derecha == HIGH)
  {
    c2++;
    delay(500);
  }
  if (c1 > 3) {
    c1 = 3;
  }
  if (c1 == 0) {
    c1 = 1;
  }
  if (c2 > 1) {
    c2 = 0;
  }
  if (c1 == 1 && c2 == 1) {
    opcion = 1;
  }
  if (c1 == 2 && c2 == 1) {
    opcion = 2;
    lcd.clear();
  }
  if (c1 == 3 && c2 == 1) {
    opcion = 3;
  }
  while (opcion == 1) {
    c1 = 0;
    c2 = 0;
    arSA = digitalRead(Up);
    izSA = digitalRead(Left);
    abSA = digitalRead(Down);
    if (arSA == HIGH) {
      cSA++;
      submenuA();
      delay(500);
    }
    if (abSA == HIGH) {
      cSA--;
      submenuA();
      delay(500);
    }
    if (cSA > 3) {
      cSA = 3;
    }
    if (cSA == 0) {
      cSA = 1;
    }
    if (izSA == HIGH) {
      opcion = 0;
      cSA = 0;
      delay(500);
    }
  }
  while (opcion == 2) {
    c1 = 0;
    c2 = 0;
    submenuC();
    izSC = digitalRead(Left);
    if (izSC == HIGH) {
      opcion = 0;
      cSM = 0;
      delay(500);
    }
  }
  while (opcion == 3) {
    c1 = 0;
    c2 = 0;
    arSM = digitalRead(Up);
    izSM = digitalRead(Left);
    abSM = digitalRead(Down);
    if (arSM == HIGH) {
      cSM++;
      submenuM();
      delay(500);
    }
    if (abSM == HIGH) {
      cSM--;
      submenuM();
      delay(500);
    }
    if (cSM > 3) {
      cSM = 3;
    }
    if (cSM == 0) {
      cSM = 1;
    }
    if (izSM == HIGH) {
      opcion = 0;
      cSM = 0;
      delay(500);
    }
  }
}
// rutina para mostrar el menu principal
void menu() {
  switch (c1) {
    case 0:
      c1 = 1;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(2, 1);
      lcd.print(">Ajustes");
      lcd.setCursor(7, 2);
      lcd.print("Clave");
      lcd.setCursor(7, 3);
      lcd.print("Modo");

      break;
    case 2:
      lcd.clear();
      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(3, 1);
      lcd.print("Ajustes");
      lcd.setCursor(6, 2);
      lcd.print(">Clave");
      lcd.setCursor(7, 3);
      lcd.print("Modo");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(3, 1);
      lcd.print("Ajustes");
      lcd.setCursor(7, 2);
      lcd.print("Clave");
      lcd.setCursor(6, 3);
      lcd.print(">Modo");
      break;
  }
}
// rutina para mostrar el su menu de ajustes
void submenuA() {
  switch (cSA) {
    case 0:
      cSA = 1;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(3, 1);
      lcd.print(">Tiempo de Purga");
      lcd.setCursor(6, 2);
      lcd.print("Tiempo Post-Purga");
      lcd.setCursor(6, 3);
      lcd.print("Tiempo UV");;
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(3, 1);
      lcd.print("Tiempo de Purga");
      lcd.setCursor(5, 2);
      lcd.print(">Tiempo Post-Purga");
      lcd.setCursor(5, 3);
      lcd.print("Tiempo UV");;

      break;
    case 3:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(3, 1);
      lcd.print("Tiempo de Purga");
      lcd.setCursor(5, 2);
      lcd.print("Tiempo Post-Purga");
      lcd.setCursor(4, 3);
      lcd.print(">Tiempo UV");;
      break;
  }
}
// rutina para mostrar el su menu de Clave
void submenuC() {
  switch (cSC) {
    case 0:
      cSC = 1;
      break;
    case 1:
      lcd.setCursor(4, 0);
      lcd.print("Clave");
      lcd.setCursor(3, 1);
      lcd.print("Digite la Clave");
      lcd.setCursor(6, 2);
      lcd.blink();
      break;
    case 2:

      break;
  }
}
// rutina para mostrar el su menu de Mantenimineto
void submenuM() {
  switch (cSM) {
    case 0:
      cSM = 1;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Modo");
      lcd.setCursor(3, 1);
      lcd.print(">Mantenimineto");
      lcd.setCursor(6, 2);
      lcd.print("Servicio");
      lcd.setCursor(6, 3);
      lcd.print("Apagado");;
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Modo");
      lcd.setCursor(3, 1);
      lcd.print("Mantenimineto");
      lcd.setCursor(5, 2);
      lcd.print(">Servicio");
      lcd.setCursor(5, 3);
      lcd.print("Apagado");;

      break;
    case 3:
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Modo");
      lcd.setCursor(3, 1);
      lcd.print("Mantenimineto");
      lcd.setCursor(5, 2);
      lcd.print("Servicio");
      lcd.setCursor(4, 3);
      lcd.print(">Apagado");;
      break;
  }
}
