#include <Nextion.h>

int Down = 28, Right = 26, Left = 24, Up = 22;
int arriba, abajo, derecha, izquierda;
int clave[4] = {2, 6, 7, 3}; //vector que contiene la clave
int cX = 0;// contador para el desplazamiento del cursor
int c1, c2, c3, c4;// contadores para almacenar los digitos de la clave
int ingreso[4];
int leer = 0, val, valX, confirmar;
unsigned long tiempoLeer = 0;
int periodoLeer = 80;
//objetos nextion(pantalla tactil inicio clave)
NexNumber cla1 = NexNumber(1, 2, "n0");
NexNumber cla2 = NexNumber(1, 3, "n1");
NexNumber cla3 = NexNumber(1, 4, "n2");
NexNumber cla4 = NexNumber(1, 5, "n3");

void setup() {
  nexInit();
  Serial2.begin(9600);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Down, INPUT);

}

void loop() {
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  if (millis() >= tiempoLeer + periodoLeer) {
    val = Serial2.read();
    tiempoLeer = millis();
  }
  if (val == 20) {
    cX++;
    delay(300);
  } else if (val == 30) {
    cX--;
    delay(300);
  }

  if (derecha == HIGH) {
    Serial2.print("click b0,1");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(300);
  }
  if (izquierda == HIGH) {
    Serial2.print("click b1,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    delay(300);
  }
  if (cX < 1) {
    cX = 0;
  }

  if (cX == 0) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);

    if (val == 1) {
      c1++;
      if (c1 > 8) {
        c1 = 9;
      }
      cla1.setValue(c1);
      delay(300);
    } else if (val == 2) {
      c1--;
      if (c1 == 0 || c1 < 0) {
        c1 = 0;
      }
      delay(300);
      cla1.setValue(c1);
    }
    if (arriba == HIGH) {
      Serial2.print("click b2,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    if (abajo == HIGH) {
      Serial2.print("click b3,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    ingreso[0] = c1;
  }
  if (cX == 1) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (val == 1) {
      c2++;
      if (c2 > 8) {
        c2 = 9;
      }
      delay(300);
      cla2.setValue(c2);
    } else if (val == 2) {
      c2--;
      if (c2 < 1) {
        c2 = 0;
      }
      delay(300);
      cla2.setValue(c2);
    }
    if (arriba == HIGH) {
      Serial2.print("click b2,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    if (abajo == HIGH) {
      Serial2.print("click b3,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    ingreso[1] = c2;
  }
  if (cX == 2) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);

    if (val == 1) {
      c3++;
      if (c3 > 8) {
        c3 = 9;
      }
      delay(300);
      cla3.setValue(c3);
    } else if (val == 2) {
      c3--;
      if (c3 < 1) {
        c3 = 0;
      }
      delay(300);
      cla3.setValue(c3);
    }
    if (arriba == HIGH) {
      Serial2.print("click b2,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    if (abajo == HIGH) {
      Serial2.print("click b3,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    ingreso[2] = c3;
  }
  if (cX == 3) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);

    if (val == 1) {
      c4++;
      if (c4 > 8) {
        c4 = 9;
      }
      delay(300);
      cla4.setValue(c4);
    } else if (val == 2) {
      c4--;
      if (c4 < 1) {
        c4 = 0;
      }
      delay(300);
      cla4.setValue(c4);
    }
    if (arriba == HIGH) {
      Serial2.print("click b2,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    if (abajo == HIGH) {
      Serial2.print("click b3,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    ingreso[3] = c4;
  }

  if (cX > 3) {
    ingreso[0] = c1; ingreso[1] = c2; ingreso[2] = c3; ingreso[3] = c4;
    if (clave[0] == ingreso[0] && clave[1] == ingreso[1] && clave[2] == ingreso[2] && clave[3] == ingreso[3]) {
      Serial2.print("p5.pic=4");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(2000);
      Serial2.print("page 2");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    else {

      Serial2.print("p5.pic=5");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(1000);
      c1 = 0; c2 = 0; c3 = 0; c4 = 0; cX = 0;
      cla1.setValue(c1);
      cla2.setValue(c2);
      cla3.setValue(c3);
      cla4.setValue(c4);
      Serial2.print("p5.pic=6");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
  }
}
