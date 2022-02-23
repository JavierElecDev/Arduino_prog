#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
int Down = 25, Right = 23, Left = 24, Up = 22;//Pines entrada para lectura de pulsadores
int arriba, abajo, derecha, izquierda;//Flehas panel frontal
int arSA, drSA, izSA , abSA, cSA, selSA;// Felchas y contador menu Ajustes
int arSC, drSC, izSC, abSC , cSC, selSC;// Felchas y contador menu Clave
int arSM, drSM, izSM, abSM , cSM, selSM;// Felchas y contador menu Mantenimineto
int c1 = 1, c2 = 0, opcion = 0;// Contadores menu general
//variables para tiempo de purga
int arTP, abTP, drTP, izTP;//Variables para la lectura de las Flehas panel frontal
int cXTp; // contador para el desplazamiento del cursor tiempo purga
int cTp1, cTp2, cTp3, cTp4;// contadores para almacenar los valores del tiempo de purga
int TiempoPurga[4];
//variables para tiempo post-purga
int arTPP, abTPP, drTPP, izTPP;//Variables para la lectura de las Flehas panel frontal
int cXTpp, regresoTpp;// contadores para el desplazamiento del cursor tiempo post-purga
int cTpp1, cTpp2, cTpp3, cTpp4;// contadores para almacenar los valores del tiempo de post-purga
int TiempoPostPurga[4];
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
  cXTp = 5;
  cXTpp = 5;
  menu();
}

void loop() {
  arriba = digitalRead(Up);
  derecha = digitalRead(Right);
  abajo = digitalRead(Down);
  if (opcion == 0) {
    menu();
  }
  if (arriba == HIGH)
  {
    c1++;
    menu();
    delay(300);
  }
  if (abajo == HIGH)
  {
    c1--;
    menu();
    delay(300);
  }
  if (derecha == HIGH)
  {
    c2++;
    lcd.clear();
    delay(300);
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

}
// rutina para mostrar el menu principal
void menu() {
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
    drSA = digitalRead(Right);
    submenuA();
    if (arSA == HIGH) {
      cSA++;
      submenuA();
      delay(300);
    }
    if (abSA == HIGH) {
      cSA--;
      submenuA();
      delay(300);
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
      lcd.clear();
      delay(300);
    }
  }
  while (opcion == 2) {
    c1 = 0;
    c2 = 0;
    submenuC();
    izSC = digitalRead(Left);
    if (izSC == HIGH) {
      opcion = 0;
      cSC = 0;
      lcd.clear();
      delay(300);
    }
  }
  while (opcion == 3) {
    c1 = 0;
    c2 = 0;
    arSM = digitalRead(Up);
    izSM = digitalRead(Left);
    abSM = digitalRead(Down);
    submenuM();
    if (arSM == HIGH) {
      cSM++;
      submenuM();
      delay(300);
    }
    if (abSM == HIGH) {
      cSM--;
      submenuM();
      delay(300);
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
      lcd.clear();
      delay(300);
    }
  }
  switch (c1) {
    case 0:
      c1 = 1;
      break;
    case 1:

      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(2, 1);
      lcd.print(" >Ajustes");
      lcd.setCursor(7, 2);
      lcd.print("  Clave");
      lcd.setCursor(7, 3);
      lcd.print("  Modo");

      break;
    case 2:

      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(3, 1);
      lcd.print("  Ajustes");
      lcd.setCursor(6, 2);
      lcd.print(" >Clave");
      lcd.setCursor(7, 3);
      lcd.print("  Modo");
      break;
    case 3:

      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(3, 1);
      lcd.print("  Ajustes");
      lcd.setCursor(7, 2);
      lcd.print("  Clave");
      lcd.setCursor(6, 3);
      lcd.print(" >Modo");
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
      if (drSA == HIGH) {
        selSA = 1;
        lcd.clear();
      }
      while (selSA == 1) {
        tiempoPurga();
      }
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(3, 1);
      lcd.print(">Tiempo de Purga");
      lcd.setCursor(6, 2);
      lcd.print("Tiempo Post-Purga");
      lcd.setCursor(6, 3);
      lcd.print("Tiempo UV");
      break;
    case 2:
      if (drSA == HIGH) {
        selSA = 1;
        lcd.clear();
      }
      while (selSA == 1) {
        tiempoPostPurga();
      }
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

      lcd.setCursor(4, 0);
      lcd.print("Modo");
      lcd.setCursor(0, 1);
      lcd.print(" >Mantenimineto");
      lcd.setCursor(4, 2);
      lcd.print(" Servicio");
      lcd.setCursor(4, 3);
      lcd.print(" Apagado");;
      break;
    case 2:

      lcd.setCursor(4, 0);
      lcd.print("Modo");
      lcd.setCursor(0, 1);
      lcd.print(" Mantenimineto");
      lcd.setCursor(4, 2);
      lcd.print(" >Servicio");
      lcd.setCursor(4, 3);
      lcd.print(" Apagado");;

      break;
    case 3:

      lcd.setCursor(4, 0);
      lcd.print("Modo");
      lcd.setCursor(0, 1);
      lcd.print(" Mantenimineto");
      lcd.setCursor(4, 2);
      lcd.print(" Servicio");
      lcd.setCursor(4, 3);
      lcd.print(" >Apagado");;
      break;
  }
}
//funcion para establecer el tiempo de purga
void tiempoPurga() {
  izTP = digitalRead(Left);
  drTP = digitalRead(Right);
  lcd.setCursor(0, 0);
  lcd.print(cXTp);
  //cTp1 = 0; cTp2 = 0; cTp3 = 0; cTp3 = 0;
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
    cXTp = 5;
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
    cXTp = 5;
    lcd.setCursor(4, 3);
    lcd.print("Tiempo Establecido");
    delay(2000);
    cTp1 = 0; cTp2 = 0; cTp3 = 0; cTp4 = 0;
    selSA = 0;
    lcd.clear();
  }
  if (izTP == HIGH && cXTp == 5) {
    selSA = 0;
    cXTp = 5;
    cTp1 = 0; cTp2 = 0; cTp3 = 0; cTp4 = 0;
    lcd.clear();
  }
}
//funcion para establecer el tiempo post-purga:
void tiempoPostPurga() {
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
    cXTpp = 5;
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
    cXTpp = 5;
    lcd.setCursor(4, 3);
    lcd.print("Tiempo Establecido");
    delay(2000);
    cTpp1 = 0; cTpp2 = 0; cTpp3 = 0; cTpp4 = 0;
    selSA = 0;
    lcd.clear();
  }
  if (izTPP == HIGH && cXTpp == 5) {
    selSA = 0;
    cXTpp = 5;
    cTpp1 = 0; cTpp2 = 0; cTpp3 = 0; cTpp4 = 0;
    lcd.clear();
  }
}
//funcion para establecer la clave
