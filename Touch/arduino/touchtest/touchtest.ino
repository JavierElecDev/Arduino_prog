#include <EEPROM.h>
#include <Nextion.h>// libreria para comunicacion con pantalla tactial nextion
#define Reinicio asm("jmp 0x0000") //reseteo por codigo
//pines analogicos para lectura del los sensores
#define pin_sensorEI A5// pin sensor MAF Exaust
#define pin_sensorD A6// pin sensor MAF DownFlow
#define pin_sensorP A7// pin sensor Presion
//variables para obtener los valores de lectura
float AJP, AJE, AJD, AJI;
float adcEI, adcD, adcP; //Variable para obtener los valores del conversor AD
float voltajeEI, voltajeD, voltajeP; //Variable para obtener el voltaje de salida para el sensor
float variableE, variableD, variableI, variableP; //Variable final del sensor
float relvoltvarE = 0.1 , relvoltvarD = 0.1, relvoltvarI = 0.1, relvoltvarP = 0.1; //Relación Voltaje/Variable del sensor
//botones del panel frontal de la cabina
int Down = 28, Right = 26, Left = 24, Up = 22;
int arriba, abajo, derecha, izquierda;
int Motor = 3, Lampara = 6, UV = 5, TomaE = 4, Alarma = 7;//Pines Potencia para activar los reles de estado solido SSR
int PV1 = 9, PV2 = 8; // Posicion de la ventana
int estadoM = 0, estadoL = 0, estadoUV = 0, estadoT = 0, VentAr = 0, VentAba = 0;// pines de encalve para la parte de potencia
int CMotor = 0, CLampara = 0, CUV = 0, CTomaE = 0;//Lectura para activar Potencia
//variables encendido
int acceder, comenzar;// variables para tiempo de pulsado del boton para enceder la maquina
unsigned long tiempoComenzar = 0;//variable conteo de teimpo para acceder a pantalla de inicio y acceder al menu
int periodoComenzar = 1000;//variable conteo de teimpo para acceder a pantalla de inicio y acceder al menu
//variables clave del incio
int iniciar = 0; // variable para control del bucle de la contraseña de inicio
int clave[4] = {2, 6, 7, 3}; //vector que contiene la clave
int cX = 0;// contador para el desplazamiento del cursor
int cl1, cl2, cl3, cl4;// contadores para almacenar los digitos de la clave
int leer = 0, val, valX, confirmar;
unsigned long tiempoLeer = 0;
int periodoLeer = 50;
//variables para apagar la pantalla
unsigned long tiempoNXOFF = 0;//variable conteo de teimpo para apagar la pantalla
int periodoNXOFF = 1000;//variable conteo de teimpo para apagar la pantalla
int NXOFF = 0;// contador para apagar la pantalla
//Variables para conteo de tiempo de purga / tiempo post-purga
int p[4] = {0, 0, 2, 0};//vector que almacena el tiempo de purga
int skip, skipTPP; // variables para saltar el tiempo de purga y post-purga
int s1, s2, m1, m2, sC;// segundos y minutos
int periodo = 1000;// perido de un segundo para el conteo
int salirTP, salir;// varible flag para salir del tiempo de purga
unsigned long tiempoActualTP = 0;// variable de control para el conteo el teimpo de purga
unsigned long tiempoAnterior = 0, tiempoActual = 0; // variables para el parpadeo de mensaje de la posicion del vidrio
boolean stparpadeo = true;//variable para el mensaje de adveternica de posicion del vidrio
//variables conteo de la vida de la lampara UV
unsigned long tiemUV = 0;//variable teimpo para contar el tiempo de uso de la luz UV
int periUV = 1000;//periodo contador UV
int UVL, HUVL, MUVL;// contador para almacenar el tiempo de uso de la luz UV
int TMUVL, THUVL, TAMUVL, TAHUVL; // variables para guardar y mostrar el tiempo de la lampara UV
int conteoUV;//variable para inciar el conteo de la UV
//variables para control del menu
unsigned long tiempoMenu = 0;//variable conteo de teimpo para acceder al menu
int periodoMenu = 500;//variable conteo de teimpo para acceder al menu
int salirM , entrarM; // variables para acceder al menu
int  cSA, selSA;//  contador menu Ajustes
int  cSC;//contador menu Clave
int  cSM, selSM;// Felchas y contador menu Mantenimineto
int  cSS, selSS; // Felchas y contador menu Servicio
int c1 = 1, c2 = 0, opcion = 0;// Contadores menu general
int cambioT;// variable para altenar entre la toma electrica y el menu
int cSED, selSED;// Felchas y contador menu Selecion E/D
// variablre para los valores ExFlow , Down Flow , In Flow
float exflow, downflow , inflow, presion;
//variables control tiempo lampara UV
int cXTUV;// contadores para el desplazamiento del cursor tiempo lampara UV
int cTUV1, cTUV2, cTUV3, cTUV4;// contadores para almacenar los valores del tiempo de uso de la lampara UV
int TiempoUV[4] = {0, 0, 1, 5}; // vector para establecer el tiempo de la lampara UV
int sUV1, sUV2, mUV1, mUV2;
//variable pocision de la ventana
int posVC;
//variables control cabina botones pantalla tactil
int TToma;
//objetos nextion(pantalla tactil inicio clave)
NexNumber cla1 = NexNumber(1, 2, "n0");
NexNumber cla2 = NexNumber(1, 3, "n1");
NexNumber cla3 = NexNumber(1, 4, "n2");
NexNumber cla4 = NexNumber(1, 5, "n3");
//objetos nextion(tiempo de purga)
NexNumber m2tp = NexNumber(2, 3, "n0");
NexNumber m1tp = NexNumber(2, 4, "n1");
NexNumber s2tp = NexNumber(2, 5, "n2");
NexNumber s1tp = NexNumber(2, 6, "n3");


void setup() {
  //Potencia
  pinMode(Down, INPUT);
  pinMode(Right, INPUT);
  pinMode(Up, INPUT);
  pinMode(Left, INPUT);
  //pines para activar el modulo SSR
  pinMode(Motor, OUTPUT);
  pinMode(Lampara, OUTPUT);
  pinMode(UV, OUTPUT);
  pinMode(TomaE, OUTPUT);
  pinMode(Alarma, OUTPUT);
  //estado incial para logica inversa modulos SSR
  digitalWrite(Alarma, HIGH);
  digitalWrite(Motor, LOW);
  digitalWrite(Lampara, HIGH);
  digitalWrite(UV, HIGH);
  digitalWrite(TomaE, HIGH);
  //Pos Ventana sensado de posicion
  pinMode(PV1, INPUT);
  pinMode(PV2, INPUT);
  //configuracion para comunicacion con pantalla tactil nextion
  nexInit();//inicializacion de conexion
  Serial2.begin(9600);// comunicacion mediante puerto serial numero2 a 9600 baudios
  //Serial2.print("sleep=1");

}

void loop() {
  /* while (comenzar != 1) {
     inicio();
    }*/
  while (iniciar != 1) {
    inicioClave();
  }
  while (salirTP != 1) {
    TiempodePurga();
  }
  controlCabina();
}
//funcion para mostrar la pantalla de incio
void inicio() {
  arriba  = digitalRead(Up);
  s1 = 0; s2 = 0; m1 = 0; m2 = 0;
  cl1 = 0; cl2 = 0; cl3 = 0; cl4 = 0;
  if (arriba == HIGH) {
    if (millis() >= tiempoComenzar + periodoComenzar) {
      acceder++;
      tiempoComenzar = millis();
    }
  } else {
    acceder = 0;
  }
  if (acceder == 2) {
    Serial2.print("page 0");
    delay(2000);
    Serial2.print("page 1");
    comenzar = 1;
  }
}
void controlCabina() {
  //Configuracion para lectura de los sensores
  EEPROM.get(50, AJP);
  EEPROM.get(60, AJE);
  EEPROM.get(70, AJD);
  EEPROM.get(80, AJI);
  EEPROM.get(90, TMUVL );
  EEPROM.get(95, THUVL );
  adcEI = analogRead(pin_sensorEI);
  adcD = analogRead(pin_sensorD);
  adcP = analogRead(pin_sensorP);
  voltajeEI = adcEI * 5 / 1023;
  voltajeD = adcD * 5 / 1023;
  voltajeP = adcP * 5 / 1023;
  variableE = voltajeEI * relvoltvarE;
  variableD = voltajeEI * relvoltvarD;
  variableI = voltajeEI * relvoltvarI;
  variableP = voltajeP * relvoltvarP;
  // variables para mostrar el tiempo de la lampara UV
  TAMUVL = TMUVL + MUVL;
  TAHUVL = THUVL + HUVL;
  //Control Partes de potencia
  CTomaE  = digitalRead(Up);
  CMotor = digitalRead(Right);
  CLampara = digitalRead(Down);
  CUV = digitalRead(Left);
  //lectura posicion ventana
  VentAr = digitalRead(PV1);
  VentAba = digitalRead(PV2);
  if (Serial2.available() >= 0) {
    if (millis() >= tiempoLeer + periodoLeer) {
      val = Serial2.read();
      tiempoLeer = millis();
      //Serial.print(val);
    }
  }
  //estructura para entrar para entrar a la funcion menu
  if (millis() >= tiempoMenu + periodoMenu) {
    //lecturas prueto serie (pulsasiones de la pantalla tactil)
    if (voltajeEI <= 1.20) {
      exflow = variableE;
      inflow = variableI;
      downflow = variableD;
    }
    else {
      exflow = variableE + AJE;
      inflow = variableI + AJI;
      downflow = variableD + AJD;
    }
    if (voltajeP <= 1.55) {
      presion = variableP;
    }
    else {
      presion = variableP + AJP;
    }
    if ( presion >= 1.75) {
      /* lcd.setCursor(6, 3);
        lcd.print("CAMBIAR FILTRO");*/
    }
    if ( TAHUVL >= 2900) {
      /*  lcd.setCursor(6, 3);
        lcd.print("CAMBIAR Foco UV");*/
    }


    if (TAMUVL < 60) {
      /* lcd.setCursor(20, 2);
        lcd.print(TAMUVL);
        lcd.setCursor(22, 2);
        lcd.print("m");*/
    }
    else {
      /*  lcd.setCursor(20, 2);
        lcd.print(TAHUVL);
        lcd.setCursor(23, 2);
        lcd.print("h");*/
    }
    if (val == 2) {
      estadoT++;
      //entrarM++;
      delay(300);
    }
    if (CTomaE == HIGH) {
      entrarM++;
      estadoT++;
      //TToma++;
      Serial2.print("bt1.pic=19");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("bt1.pic2=18");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
    }
    else {
      entrarM = 0;
    }
    tiempoMenu = millis();
  }

  if (estadoT == 1) {
    digitalWrite(TomaE, LOW);
  }
  /*  if (TToma > 0) {
      TToma = 0;
    }*/
  else if (estadoT == 1 && entrarM == 1) {
    digitalWrite(TomaE, LOW);
  }
  else if (estadoT > 1 && entrarM == 0 ) {
    digitalWrite(TomaE, HIGH);
    Serial2.print("bt1.pic=18");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.print("bt1.pic2=19");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
    estadoT = 0;
  }
  if (entrarM == 4 || val == 10) {
    conteoUV = 0;
    salirM = 1;
    entrarM = 0;
    Serial2.print("page 4");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  while (salirM == 1) {
    menu();
    estadoUV = 0;
    digitalWrite(UV, HIGH);
    estadoL = 0;
    digitalWrite(Lampara, HIGH);
    digitalWrite(Alarma, HIGH);
  }
  //activar motor
  if (CMotor == HIGH) {
    digitalWrite(Motor, HIGH);
  }
  if (estadoM > 1) {
    digitalWrite(Motor, LOW);
    estadoM = 0;
    Serial2.print("bt2.pic=15");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.print("bt2.pic2=14");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  if (CMotor == HIGH) {
    estadoM++;
    delay(800);
    Serial2.print("bt2.pic=14");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.print("bt2.pic2=15");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  if (val == 3) {
    digitalWrite(Motor, HIGH);
    estadoM++;
    delay(800);
  }
  //Alarma
  if (VentAba == LOW) {
    if (downflow < 0.28) {
      digitalWrite(Alarma, LOW);
      if ((millis() >= 800 + tiempoActual) && stparpadeo == true) {
        stparpadeo = false;
        /*lcd.setCursor(6, 3);
          lcd.print("Perdida de Flujo ");*/
        tiempoAnterior = millis();
      }
      if ((millis() >= 800 + tiempoAnterior) && stparpadeo == false) {
        stparpadeo = true;
        /* lcd.setCursor(2, 1);
          lcd.setCursor(6, 3);
          lcd.print("                 ");*/
        tiempoActual = millis();
      }
    } else {
      digitalWrite(Alarma, HIGH);
    }
  }
  //pos incorrecta ventana
  if (VentAba == LOW && VentAr == LOW) {
    estadoL = 0;
    estadoUV = 0;
    digitalWrite(UV, HIGH);
    digitalWrite(Lampara, HIGH);
    if ((millis() >= 600 + tiempoActual) && stparpadeo == true) {
      stparpadeo = false;
      Serial2.print("p1.pic=6");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      tiempoAnterior = millis();
    }
    if ((millis() >= 600 + tiempoAnterior) && stparpadeo == false) {
      stparpadeo = true;
      Serial2.print("p1.pic=42");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      tiempoActual = millis();
    }
  }
  // pos ventana arriba control lampara normal
  if (VentAr == HIGH) {
    posVC = 1;
    if (posVC == 1) {
      Serial2.print("p1.pic=6");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    posVC = 0;
    //activar lampara
    if (CLampara == HIGH) {
      digitalWrite(Lampara, LOW);
    }
    if (estadoL > 1) {
      digitalWrite(Lampara, HIGH);
      estadoL = 0;
      Serial2.print("bt0.pic=10");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("bt0.pic2=11");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    if (CLampara == HIGH) {
      estadoL++;
      delay(800);
      Serial2.print("bt0.pic=11");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("bt0.pic2=10");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    if (val == 1) {
      digitalWrite(Lampara, LOW);
      estadoL++;
      delay(300);
    }
  }
  if (VentAba == HIGH) { //pos ventana abajo control lampara uv
    digitalWrite(Alarma, HIGH);
    //activar lampara uv
    if (CUV == HIGH) {
      digitalWrite(UV, LOW);
    }
    if (estadoUV > 1) {
      digitalWrite(UV, HIGH);
      estadoUV = 0;
      Serial2.print("bt3.pic=20");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("bt3.pic2=21");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(300);
      // conteoUV = 0;
    }
    if (CUV == HIGH) {
      estadoUV++;
      delay(800);
      Serial2.print("bt3.pic=21");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("bt3.pic2=20");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
    if (val == 4) {
      digitalWrite(UV, LOW);
      estadoUV++;
      delay(300);
    }
    while (estadoUV == 1) {
      VentAba = digitalRead(PV2);
      CUV = digitalRead(Left);
      if (VentAba == LOW) {
        digitalWrite(UV, HIGH);
        estadoUV = 0;
      }
      if (estadoUV > 1) {
        digitalWrite(UV, HIGH);
        estadoUV = 0;
      }
      if (CUV == HIGH) {
        estadoUV++;
        delay(800);
      }
      if (millis() >= tiemUV + periUV) {
        UVL++;
        sUV1++;
        tiemUV = millis();
      }
      if (sUV1 == 9) {
        sUV2++;
        sUV1 = 0;
      }
      if (sUV2 == 6) {
        mUV1++;
        sUV2 = 0;
      }
      if (mUV1 == 9) {
        mUV2++;
        mUV1 = 0;
      }
      if (mUV2 == 9) {
        mUV2 = 0;
      }
      if (TiempoUV[0] == mUV2 && TiempoUV[1] == mUV1 && TiempoUV[2] == sUV2 && TiempoUV[3] == sUV1 ) {
        estadoUV = 0;
        sUV1 = 0; sUV2 = 0; mUV1 = 0; mUV2 = 0;
        digitalWrite(UV, HIGH);
      }

      if (UVL == 60) {
        UVL = 0;
        MUVL++;
      }
      if (MUVL == 60) {
        MUVL = 0;
        HUVL++;
      }
    }
    if ( estadoUV == 0) {
      sUV1 = 0; sUV2 = 0; mUV1 = 0; mUV2 = 0;
    }
  } else {
    digitalWrite(UV, HIGH);
  }
}

//funcion para el ingreso de la clave incial
void inicioClave() {
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);


  if (millis() >= tiempoLeer + periodoLeer) {
    izquierda = digitalRead(Left);
    derecha = digitalRead(Right);
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    val = Serial2.read();
    tiempoLeer = millis();

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
    if (cX == 0) {
      if (val == 1) {
        cl1++;
        if (cl1 > 8) {
          cl1 = 9;
        }
        cla1.setValue(cl1);
        cla1.getValue(cl1);
        delay(300);
      } else if (val == 2) {
        cl1--;
        if (cl1 == 0 || cl1 < 0) {
          cl1 = 0;
        }
        delay(300);
        cla1.setValue(cl1);
      }

    }
    if (cX == 1) {
      if (val == 1) {
        cl2++;
        if (cl2 > 8) {
          cl2 = 9;
        }
        delay(300);
        cla2.setValue(cl2);
      } else if (val == 2) {
        cl2--;
        if (cl2 < 1) {
          cl2 = 0;
        }
        delay(300);
        cla2.setValue(cl2);
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
    }
    if (cX == 2) {
      if (val == 1) {
        cl3++;
        if (cl3 > 8) {
          cl3 = 9;
        }
        delay(300);
        cla3.setValue(cl3);
      } else if (val == 2) {
        cl3--;
        if (cl3 < 1) {
          cl3 = 0;
        }
        delay(300);
        cla3.setValue(cl3);
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
    }
    if (cX == 3) {
      if (val == 1) {
        cl4++;
        if (cl4 > 8) {
          cl4 = 9;
        }
        delay(300);
        cla4.setValue(cl4);
      } else if (val == 2) {
        cl4--;
        if (cl4 < 1) {
          cl4 = 0;
        }
        delay(300);
        cla4.setValue(cl4);
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
    }

    if (cX > 3) {
      if (clave[0] == cl1 && clave[1] == cl2 && clave[2] == cl3 && clave[3] == cl4) {
        Serial2.print("p5.pic=4");//comando para enviar ordenes a la pantalla tactil
        Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(2000);
        iniciar = 1;
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
        cl1 = 0; cl2 = 0; cl3 = 0; cl4 = 0; cX = 0;
        cla1.setValue(cl1);
        cla2.setValue(cl2);
        cla3.setValue(cl3);
        cla4.setValue(cl4);
        Serial2.print("p5.pic=6");//comando para enviar ordenes a la pantalla tactil
        Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.print("p1.pic=2");//comando para enviar ordenes a la pantalla tactil
        Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.print("p4.pic=3");//comando para enviar ordenes a la pantalla tactil
        Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
        Serial2.write(0xff);
        Serial2.write(0xff);
      }
    }
    // apagado de pantalla caso de que no se ingrese ninguna clave
    if (millis() >= tiempoNXOFF + periodoNXOFF) {
      NXOFF++;
      tiempoNXOFF = millis();
    }
    else if (derecha == HIGH || izquierda == HIGH || arriba == HIGH || abajo == HIGH ) {
      NXOFF = 0;
    }
    else if (val == 20 || val == 30 || val == 2 || val == 1) {
      NXOFF = 0;
    }
    if (NXOFF == 30) {
      Serial2.print("sleep=1");
      cl1 = 0; cl2 = 0; cl3 = 0; cl4 = 0;
      Reinicio;
      delay(500);
    }
  }
}
//Funcion para el tiempo de purga
void TiempodePurga() {
  /*EEPROM.get(20, p[0]);
    EEPROM.get(22, p[1]);
    EEPROM.get(24, p[2]);
    EEPROM.get(26, p[3]);*/
  digitalWrite(Motor, HIGH);
  tiempoActualTP = millis();
  VentAr = digitalRead(PV1);
  if (VentAr == HIGH) {
    salir = 0;
  }
  while (salir != 1) {
    VentAr = digitalRead(PV1);
    arriba  = digitalRead(Up);
    derecha = digitalRead(Right);
    abajo = digitalRead(Down);
    val = Serial2.read();
    if (VentAr == LOW) {
      salir = 1;
    }
    if (millis() >= tiempoActualTP + periodo) {
      s1++;
      tiempoActualTP = millis();

    }
    if (s1 == 9) {
      s2++;
      s1 = 0;
    }
    if (s2 == 6) {
      m1++;
      s2 = 0;
    }
    if (m1 == 9) {
      m2++;
      m1 = 0;
    }
    if (m2 == 9) {
      m2 = 0;
    }
    m2tp.setValue(m2);
    m1tp.setValue(m1);
    s2tp.setValue(s2);
    s1tp.setValue(s1);
    if (p[0] == m2 && p[1] == m1 && p[2] == s2 && p[3] == s1 ) {
      Serial2.print("p1.pic=8");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(2000);
      salirTP = 1;
      salir = 1;
      digitalWrite(Lampara, LOW);
      Serial2.print("page 3");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(200);
    }
    if (arriba == HIGH || abajo == HIGH || derecha == HIGH) {
      skip++;
      delay(1000);
    }
    else  {
      skip = 0;
    }
    if (skip == 5 || val == 10) {
      salirTP = 1;
      salir = 1;
      Serial2.print("page 3");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(200);
    }
  }

  if (VentAba == LOW) {
    if ((millis() >= 1000 + tiempoActual) && stparpadeo == true) {
      stparpadeo = false;
      Serial2.print("p1.pic=6");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      tiempoAnterior = millis();
    }
    if ((millis() >= 1000 + tiempoAnterior) && stparpadeo == false) {
      stparpadeo = true;
      Serial2.print("p1.pic=42");//comando para enviar ordenes a la pantalla tactil
      Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
      Serial2.write(0xff);
      Serial2.write(0xff);
      tiempoActual = millis();
    }
  }
}
/////////////
void menu() {
  arriba = digitalRead(Up);
  derecha = digitalRead(Right);
  abajo = digitalRead(Down);
  izquierda = digitalRead(Left);
  if (arriba == HIGH)
  {
    c1--;
    delay(600);
  }
  if (abajo == HIGH)
  {
    c1++;
    delay(600);
  }
  if (derecha == HIGH)
  {
    c2++;
    delay(600);
  }
  if (izquierda == HIGH) {
    salirM = 2;
    estadoT = 1;
    delay(500);
    Serial2.print("page 3");//comando para enviar ordenes a la pantalla tactil
    Serial2.write(0xff);//espacio entre el set de ordenes para la pantalla tactil
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  if (c1 > 3 || c1 < 1) {
    c1 = 0;
  }
  if (c1 == 1) {
    
  }
  else if(c1 == 2){
    
  }
    else if(c1 == 3){
    
  }
  if (c2 > 1) {
    c2 = 0;
  }
  if (c1 == 1 && c2 == 1) {
    opcion = 1;
  }
  if (c1 == 2 && c2 == 1) {
    opcion = 2;
  }
  if (c1 == 3 && c2 == 1) {
    opcion = 3;
  }

}
