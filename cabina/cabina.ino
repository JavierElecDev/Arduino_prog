#include <EEPROM.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
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
//clave maestra
int cM[4] = {8, 9, 0, 1};//clave por si el cliente olvida la clave(clave maestra)
//variables para comenzar el trabajo de la maquina
unsigned long tiempoComenzar = 0;//variable conteo de teimpo para acceder al menu
int periodoComenzar = 1000;//variable conteo de teimpo para acceder al menu
//variables refresco LCD
unsigned long tiempoLCD = 0;
int periodoLCD = 1000;
int acceder, comenzar;// variables para tiempo de pulsado del boton para enceder la maquina
//variables para el parapadeo de mensaje de posicion del vidrio
boolean stparpadeo = true;//variable para el mensaje de adveternica de posicion del vidrio
unsigned long tiempoAnterior = 0, tiempoActual = 0; // variables para el parpadeo de mensaje de la posicion del vidrio
int limpiarlcd = 0 , ecribirlcd = 0, limpventAR = 0, limpventAba; // variable para el refresco de la pantalla LCD 20x04
//configuracion salidas de potencias. entradas del tablero
int Right = 26, Up = 22, Down = 28, Left = 24;//Pines para los pulsadores
int arriba, abajo, derecha, izquierda;//Flehas panel frontal
int Motor = 3, Lampara = 6, UV = 5, TomaE = 4, Alarma = 7;//Pines Potencia para activar los reles de estado solido SSR
int CMotor = 0, CLampara = 0, CUV = 0, CTomaE = 0;//Lectura para activar Potencia
int PV1 = 9, PV2 = 8; // Posicion de la ventana
int encender = 1, estadoM = 0, estadoL = 0, estadoUV = 0, estadoT = 0, VentAr = 0, VentAba = 0;// pines de encalve para la parte de potencia
//configuracion para ingreso de la clave:
int iniciar = 0; // variable para control del bucle de la contraseña de inicio
int CLA[4]; //vector que contiene la clave
int cX;// contador para el desplazamiento del cursor
int cl1, cl2, cl3, cl4;// contadores para almacenar los digitos de la clave
//variales para el cambio de clave
int ccX;// contador para el desplazamiento del cursor de cambio de contraeña
int cc1, cc2, cc3, cc4;// contadores para almacenar los digitos de la clave
int nueva;//variable para acceder al cambio de clave
//Variables para conteo de tiempo de purga / tiempo post-purga
int skip, skipTPP; // variables para saltar el tiempo de purga y post-purga
int s1, s2, m1, m2, sC;// segundos y minutos
int periodo = 1000;// perido de un segundo para el conteo
int salirTP, salir;// varible flag para salir del tiempo de purga
unsigned long tiempoActualTP = 0;// variable de control para el conteo el teimpo de purga
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
//variables para tiempo de purga

int cXTp; // contador para el desplazamiento del cursor tiempo purga
int cTp1, cTp2, cTp3, cTp4;// contadores para almacenar los valores del tiempo de purga
int p[4] = {0, 0, 2, 0};//vector que almacena el tiempo de purga
//variables para tiempo post-purga

int cXTpp, regresoTpp;// contadores para el desplazamiento del cursor tiempo post-purga
int cTpp1, cTpp2, cTpp3, cTpp4;// contadores para almacenar los valores del tiempo de post-purga
int pp[4] = {0, 0, 2, 2};//vector que almacena el tiempo de post-purga
//variables control tiempo lampara UV

int cXTUV;// contadores para el desplazamiento del cursor tiempo lampara UV
int cTUV1, cTUV2, cTUV3, cTUV4;// contadores para almacenar los valores del tiempo de uso de la lampara UV
int TiempoUV[4] = {0, 0, 1, 5}; // vector para establecer el tiempo de la lampara UV
int sUV1, sUV2, mUV1, mUV2;
//Variables de control modo Mantenimento
unsigned long tiempoMant;//variable conteo de teimpo para salir del modo Mantenimiento
int periodoMant = 2000;//variable conteo de teimpo para salir del modo Mantenimiento
int salirMant; //variables para salir del modo Mantenimiento
// variablre para ajuste ExFlow , Down Flow , In Flow
float exflow, downflow , inflow, presion;
//variables para el menu apagar
int cSal, selSal;
//variables para apagar la pantalla LCD
unsigned long tiempoLCDOFF = 0;//variable conteo de teimpo para apagar la LCD
int periodoLCDOFF = 1000;//variable conteo de teimpo para apagar la LCD
int LCDOFF;// contador para apagar la LCD
//variables conteo de la vida de la lampara UV
unsigned long tiemUV = 0;//variable teimpo para contar el tiempo de uso de la luz UV
int periUV = 1000;//periodo contador UV
int UVL, HUVL, MUVL;// contador para almacenar el tiempo de uso de la luz UV
int TMUVL, THUVL, TAMUVL, TAHUVL; // variables para guardar y mostrar el tiempo de la lampara UV
int conteoUV;//variable para inciar el conteo de la UV
//variables funcion clave de servicio
int claveSE[4] = {4, 4, 4, 4}; //vector que contiene la clave
int cSX;// contador para el desplazamiento del cursor
int cS1, cS2, cS3, cS4;// contadores para almacenar los digitos de la clave
int iniciarS;
// Constructor de la librería de LCD 20x04
// Aqui se configuran los pines asignados a la pantalla LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
void setup() {
  lcd.begin(20, 4);
  //estado incial pantalla LCD apagada
  lcd.noDisplay();
  lcd.noBacklight();
  cX = 7;// contador para la posicion del cursor para ingreso de la clave
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
  //contadores para movimiento de cursor en funciones
  cXTp = 6;//contador para movimiento de cursor en funcion cambio tiempo de puga
  cXTpp = 6;//contador para movimiento de cursor en funcion cambio tiempo de post-purga
  ccX = 7; //contador para movimiento de cursor en funcion cambio de clave
  cXTUV = 6;//contador para movimiento de cursor en funcion cambio tiempo de lampara UV
  //bucle de control ingreso de clave al inciar la cabina
}

void loop() {
  while (comenzar != 1) {
    inicio();
  }
  while (iniciar != 1) {
    inicioClave();
  }

  while (salirTP != 1) {
    TiempodePurga();
  }
  controlCabina();
}
//funcion de incio
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
    comenzar = 1;
    lcd.begin(16, 4);
    lcd.setCursor(4, 0);
    lcd.print("JP InGlobal");
    lcd.setCursor(0, 1);
    lcd.print("Cabina Bio-Seguridad");
    lcd.setCursor(9, 2);
    lcd.print("Clase 2");
    lcd.setCursor(9, 3);
    lcd.print("Tipo A2");
    delay(3000);
    lcd.clear();
  }
}
//Funcion para el control de funciones de potencia de la cabina
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
  //estructura para entrar para entrar a la funcion menu
  if (millis() >= tiempoMenu + periodoMenu) {
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
      lcd.setCursor(6, 3);
      lcd.print("CAMBIAR FILTRO");
    }
    if ( TAHUVL >= 2900) {
      lcd.setCursor(6, 3);
      lcd.print("CAMBIAR Foco UV");
    }

    lcd.setCursor(0, 0);
    lcd.print("ExF:");
    lcd.setCursor(4, 0);
    lcd.print(exflow);
    lcd.setCursor(8, 0);
    lcd.print("   ");

    lcd.setCursor(11, 0);
    lcd.print("InF:");
    lcd.setCursor(15, 0);
    lcd.print(inflow);
    lcd.setCursor(0, 1);
    lcd.print("   ");

    lcd.setCursor(0, 1);
    lcd.print("DnF:");
    lcd.setCursor(4, 1);
    lcd.print(downflow);
    lcd.setCursor(8, 1);
    lcd.print("   ");

    lcd.setCursor(4, 2);
    lcd.print("Filter:");
    lcd.setCursor(11, 2);
    lcd.print(presion);
    lcd.setCursor(16, 2);
    lcd.print("UVL:");
    if (TAMUVL < 60) {
      lcd.setCursor(20, 2);
      lcd.print(TAMUVL);
      lcd.setCursor(22, 2);
      lcd.print("m");
    }
    else {
      lcd.setCursor(20, 2);
      lcd.print(TAHUVL);
      lcd.setCursor(23, 2);
      lcd.print("h");
    }
    lcd.setCursor(4, 3);
    lcd.print("                   ");
    if (CTomaE == HIGH) {
      entrarM++;
      estadoT++;
    }
    else {
      entrarM = 0;
    }
    tiempoMenu = millis();
  }
  if (estadoT == 1) {
    digitalWrite(TomaE, LOW);
    lcd.setCursor(11, 1);
    lcd.print("Toma:ON  ");
  }
  else if (estadoT == 0) {
    lcd.setCursor(11, 1);
    lcd.print("Toma:OFF  ");
  }
  else if (estadoT == 1 && entrarM == 1) {
    digitalWrite(TomaE, LOW);
    lcd.setCursor(11, 1);
    lcd.print("Toma:ON  ");
  }
  else if (estadoT > 1 && entrarM == 0 ) {
    digitalWrite(TomaE, HIGH);
    lcd.setCursor(11, 1);
    lcd.print("Toma:OFF  ");
    estadoT = 0;
  }
  if (entrarM == 4) {
    conteoUV = 0;
    salirM = 1;
    lcd.clear();
    entrarM = 0;
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
  }
  if (CMotor == HIGH) {
    estadoM++;
    delay(800);
  }
  //Alarma
  if (VentAba == LOW) {
    if (downflow < 0.28) {
      digitalWrite(Alarma, LOW);
      if ((millis() >= 800 + tiempoActual) && stparpadeo == true) {
        stparpadeo = false;
        lcd.setCursor(6, 3);
        lcd.print("Perdida de Flujo ");
        tiempoAnterior = millis();
      }
      if ((millis() >= 800 + tiempoAnterior) && stparpadeo == false) {
        stparpadeo = true;
        lcd.setCursor(2, 1);
        lcd.setCursor(6, 3);
        lcd.print("                 ");
        tiempoActual = millis();
        limpiarlcd = 0;
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
    limpiarlcd = 1;
    limpiarlcd = 0;
    if (limpiarlcd == 1) {
      lcd.clear();
      delay(200);
      limpiarlcd = 0;
    }
    else {
      if ((millis() >= 600 + tiempoActual) && stparpadeo == true) {
        stparpadeo = false;
        lcd.setCursor(6, 3);
        lcd.print("Ubique el Vidrio");
        tiempoAnterior = millis();
      }
      if ((millis() >= 600 + tiempoAnterior) && stparpadeo == false) {
        stparpadeo = true;
        lcd.setCursor(6, 3);
        lcd.print("                 ");
        tiempoActual = millis();
        limpiarlcd = 0;
      }
    }
  }
  // pos ventana arriba control lampara normal
  if (VentAr == HIGH) {
    //activar lampara

    limpventAR = 1;
    limpventAR = 0;
    if (limpventAR == 1) {
      lcd.clear();
      delay(200);
      limpventAR = 0;
    } else {
      //lecturaes en pantalla
    }
    if (CLampara == HIGH) {
      digitalWrite(Lampara, LOW);
    }
    if (estadoL > 1) {
      digitalWrite(Lampara, HIGH);
      estadoL = 0;
    }
    if (CLampara == HIGH) {
      estadoL++;
      delay(800);
    }
  }
  if (VentAba == HIGH) { //pos ventana abajo control lampara uv
    digitalWrite(Alarma, HIGH);
    //activar lampara
    limpventAba = 1;
    limpventAba = 0;
    if (limpventAba == 1) {
      lcd.clear();
      delay(200);
      limpventAba = 0;
    } else {
      //lecturaes en pantalla

    }
    if (CUV == HIGH) {
      digitalWrite(UV, LOW);
    }
    if (estadoUV > 1) {
      digitalWrite(UV, HIGH);
      estadoUV = 0;
      // conteoUV = 0;
    }
    if (CUV == HIGH) {
      estadoUV++;
      delay(800);
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
      lcd.setCursor(7, 3);
      lcd.print(mUV2);
      lcd.setCursor(8, 3);
      lcd.print(mUV1);
      lcd.setCursor(9, 3);
      lcd.print(":");
      lcd.setCursor(10, 3);
      lcd.print(sUV2);
      lcd.setCursor(11, 3);
      lcd.print(sUV1);
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
//Funcion para la clave de inicio de la cabina
void inicioClave() {
  EEPROM.get(10, CLA[0]);
  EEPROM.get(12, CLA[1]);
  EEPROM.get(14, CLA[2]);
  EEPROM.get(16, CLA[3]);
  lcd.setCursor(1, 0);
  lcd.print("Ingrese la Clave:");
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  lcd.setCursor(7, 1);
  lcd.print(cl1);
  lcd.setCursor(8, 1);
  lcd.print(cl2);
  lcd.setCursor(9, 1);
  lcd.print(cl3);
  lcd.setCursor(10, 1);
  lcd.print(cl4);
  if (derecha == HIGH) {
    cX++;
    delay(600);
  }
  if (izquierda == HIGH) {
    cX--;
    delay(600);
  }
  if (cX < 7) {
    cX = 7;
  }

  if (cX == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cl1++;
      delay(600);
    }
    if (abajo == HIGH) {
      cl1--;
      delay(600);
    }
    if (cl1 > 9) {
      cl1 = 9;
    }
    else if (cl1 < 0) {
      cl1 = 0;
    }
    lcd.setCursor(cX, 1);
    lcd.print(cl1);
    lcd.setCursor(cX + 4, 2);
    lcd.print("^   ");
  }
  else if (cX == 8) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cl2++;
      delay(600);
    }
    if (abajo == HIGH) {
      cl2--;
      delay(600);
    }
    if (cl2 > 9) {
      cl2 = 9;
    }
    else if (cl2 < 0) {
      cl2 = 0;
    }
    lcd.setCursor(cX, 1);
    lcd.print(cl2);
    lcd.setCursor(cX + 3, 2);
    lcd.print(" ^  ");
  }
  else if (cX == 9) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cl3++;
      delay(600);
    }
    if (abajo == HIGH) {
      cl3--;
      delay(600);
    }
    if (cl3 > 9) {
      cl3 = 9;
    }
    else if (cl3 < 0) {
      cl3 = 0;
    }
    lcd.setCursor(cX, 1);
    lcd.print(cl3);
    lcd.setCursor(cX + 2, 2);
    lcd.print("  ^ ");

  }
  else if (cX == 10) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cl4++;
      delay(600);
    }
    if (abajo == HIGH) {
      cl4--;
      delay(600);
    }
    if (cl4 > 9) {
      cl4 = 9;
    }
    else if (cl4 < 0) {
      cl4 = 0;
    }
    lcd.setCursor(cX, 1);
    lcd.print(cl4);
    lcd.setCursor(cX + 1, 2);
    lcd.print("   ^");
  }
  if (cX > 10) {
    if (CLA[0] == cl1 && CLA[1] == cl2 && CLA[2] == cl3 && CLA[3] == cl4) {
      lcd.setCursor(6, 3);
      lcd.print("ClAVE CORRECTA");
      delay(2000);
      iniciar = 1;
      digitalWrite(Motor, HIGH);
    }
    else if (cM[0] == cl1 && cM[1] == cl2 && cM[2] == cl3 && cM[3] == cl4) {
      lcd.setCursor(6, 3);
      lcd.print("ClAVE CORRECTA");
      delay(2000);
      iniciar = 1;
      digitalWrite(Motor, HIGH);
    }
    else {
      lcd.setCursor(1, 0);
      lcd.print("CLAVE INCORRECTA");
      cl1 = 0; cl2 = 0; cl3 = 0; cl4 = 0;
      delay(2000);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ingrese la Clave:");
      cX = 7;
    }
  }
  // apagado de pantalla lcd en caso de que no se ingrese ninguna clave
  if (millis() >= tiempoLCDOFF + periodoLCDOFF) {
    LCDOFF++;
    tiempoLCDOFF = millis();
  }
  else if (derecha == HIGH || izquierda == HIGH || arriba == HIGH || abajo == HIGH) {
    LCDOFF = 0;
  }
  if (LCDOFF == 30) {
    lcd.noDisplay();
    lcd.noBacklight();
    cl1 = 0; cl2 = 0; cl3 = 0; cl4 = 0;
    cX = 7;
    Reinicio;
    delay(500);
  }
}

//Funcion para el tiempo de purga
void TiempodePurga() {
  EEPROM.get(20, p[0]);
  EEPROM.get(22, p[1]);
  EEPROM.get(24, p[2]);
  EEPROM.get(26, p[3]);
  digitalWrite(Motor, HIGH);
  lcd.setCursor(1, 0);
  lcd.print(" Tiempo de Purga");
  lcd.setCursor(1, 2);
  lcd.print("               ");
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
    if (p[0] == m2 && p[1] == m1 && p[2] == s2 && p[3] == s1 ) {
      lcd.setCursor(5, 3);
      lcd.print("PURGA FINALIZADA");
      delay(2000);
      salirTP = 1;
      salir = 1;
      digitalWrite(Lampara, LOW);
      lcd.clear();
      delay(200);
    }
    lcd.setCursor(7, 1);
    lcd.print(m2);
    lcd.setCursor(8, 1);
    lcd.print(m1);
    lcd.setCursor(9, 1);
    lcd.print(":");
    lcd.setCursor(10, 1);
    lcd.print(s2);
    lcd.setCursor(11, 1);
    lcd.print(s1);
    lcd.setCursor(6, 3);
    lcd.print("                ");
    if (arriba == HIGH || abajo == HIGH || derecha == HIGH) {
      skip++;
      delay(1000);
    }
    else  {
      skip = 0;
    }
    if (skip == 5) {
      salirTP = 1;
      salir = 1;
      lcd.clear();
      delay(200);
    }
  }

  if (VentAba == LOW) {
    if ((millis() >= 1000 + tiempoActual) && stparpadeo == true) {
      stparpadeo = false;
      lcd.setCursor(6, 3);
      lcd.print("Ubique el Vidrio");
      tiempoAnterior = millis();
    }
    if ((millis() >= 1000 + tiempoAnterior) && stparpadeo == false) {
      stparpadeo = true;
      lcd.setCursor(6, 3);
      lcd.print("                ");
      tiempoActual = millis();
      limpiarlcd = 0;
    }
  }
}
// rutina para mostrar el menu principal
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
    lcd.clear();
    delay(600);
  }
  if (izquierda == HIGH) {
    salirM = 2;
    estadoT = 1;
    delay(500);
    lcd.clear();
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
    arriba = digitalRead(Up);
    izquierda = digitalRead(Left);
    abajo = digitalRead(Down);
    derecha = digitalRead(Right);
    submenuA();
    if (arriba == HIGH) {
      cSA--;
      submenuA();
      delay(600);
    }
    if (abajo == HIGH) {
      cSA++;
      submenuA();
      delay(600);
    }
    if (cSA > 3) {
      cSA = 3;
    }
    if (cSA == 0) {
      cSA = 1;
    }
    if (izquierda == HIGH) {
      opcion = 0;
      cSA = 0;
      lcd.clear();
      delay(600);
    }
  }
  while (opcion == 2) {
    c1 = 0;
    c2 = 0;
    arriba = digitalRead(Up);
    izquierda = digitalRead(Left);
    abajo = digitalRead(Down);
    derecha = digitalRead(Right);
    CambiarClave();
  }
  while (opcion == 3) {
    c1 = 0;
    c2 = 0;
    arriba = digitalRead(Up);
    izquierda = digitalRead(Left);
    abajo = digitalRead(Down);
    derecha = digitalRead(Right);
    submenuM();
    if (arriba == HIGH) {
      cSM--;
      submenuM();
      delay(600);
    }
    if (abajo == HIGH) {
      cSM++;
      submenuM();
      delay(600);
    }
    if (cSM > 3) {
      cSM = 3;
    }
    if (cSM == 0) {
      cSM = 1;
    }
    if (izquierda == HIGH) {
      opcion = 0;
      cSM = 0;
      lcd.clear();
      delay(600);
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
      lcd.print(" >Ajustes   ");
      lcd.setCursor(7, 2);
      lcd.print("  Clave     ");
      lcd.setCursor(7, 3);
      lcd.print("  Modo      ");

      break;
    case 2:

      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(3, 1);
      lcd.print("  Ajustes   ");
      lcd.setCursor(6, 2);
      lcd.print(" >Clave     ");
      lcd.setCursor(7, 3);
      lcd.print("  Modo      ");
      break;
    case 3:

      lcd.setCursor(7, 0);
      lcd.print("Menu");
      lcd.setCursor(3, 1);
      lcd.print("  Ajustes   ");
      lcd.setCursor(7, 2);
      lcd.print("  Clave     ");
      lcd.setCursor(6, 3);
      lcd.print(" >Modo      ");
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
      EEPROM.get(20, p[0]);
      EEPROM.get(22, p[1]);
      EEPROM.get(24, p[2]);
      EEPROM.get(26, p[3]);
      if (derecha == HIGH) {
        selSA = 1;
        lcd.clear();
      }
      while (selSA == 1) {
        EtiempoPurga();
      }
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(0, 1);
      lcd.print(" >Tiempo de Purga ");
      lcd.setCursor(4, 2);
      lcd.print(" Tiempo Post-Purga ");
      lcd.setCursor(4, 3);
      lcd.print(" Tiempo UV ");
      break;
    case 2:
      EEPROM.get(30, pp[0]);
      EEPROM.get(32, pp[1]);
      EEPROM.get(34, pp[2]);
      EEPROM.get(36, pp[3]);
      if (derecha == HIGH) {
        selSA = 1;
        lcd.clear();
      }
      while (selSA == 1) {
        EtiempoPostPurga();
      }
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(0, 1);
      lcd.print(" Tiempo de Purga ");
      lcd.setCursor(4, 2);
      lcd.print(" >Tiempo Post-Purga ");
      lcd.setCursor(4, 3);
      lcd.print(" Tiempo UV ");;

      break;
    case 3:
      EEPROM.get(40, TiempoUV[0]);
      EEPROM.get(42, TiempoUV[1]);
      EEPROM.get(44, TiempoUV[2]);
      EEPROM.get(46, TiempoUV[3]);
      if (derecha == HIGH) {
        selSA = 1;
        lcd.clear();
      }
      while (selSA == 1) {
        EtiempoUV();
      }
      lcd.setCursor(4, 0);
      lcd.print("Ajustes");
      lcd.setCursor(0, 1);
      lcd.print(" Tiempo de Purga ");
      lcd.setCursor(4, 2);
      lcd.print(" Tiempo Post-Purga ");
      lcd.setCursor(4, 3);
      lcd.print(" >Tiempo UV ");;
      break;
  }
}
// rutina para cambiar la Clave
void CambiarClave() {
  EEPROM.get(10, CLA[0]);
  EEPROM.get(12, CLA[1]);
  EEPROM.get(14, CLA[2]);
  EEPROM.get(16, CLA[3]);
  lcd.setCursor(0, 0);
  lcd.print("Ingrese clave Actual");
  lcd.setCursor(7, 1);
  lcd.print(cc1);
  lcd.setCursor(8, 1);
  lcd.print(cc2);
  lcd.setCursor(9, 1);
  lcd.print(cc3);
  lcd.setCursor(10, 1);
  lcd.print(cc4);
  if (izquierda == HIGH && ccX == 7) {
    opcion = 0;
    salirM = 1;
    ccX = 6;
    cc1 = 0; cc2 = 0; cc3 = 0; cc4 = 0;
    lcd.clear();
  }
  if (derecha == HIGH) {
    ccX++;
    delay(600);
  }
  if (izquierda == HIGH) {
    ccX--;
    delay(600);
  }
  if (ccX < 7) {
    ccX = 7;
  }

  if (ccX == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cc1++;
      delay(600);
    }
    if (abajo == HIGH) {
      cc1--;
      delay(600);
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
  else if (ccX == 8) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cc2++;
      delay(600);
    }
    if (abajo == HIGH) {
      cc2--;
      delay(600);
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
  else if (ccX == 9) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cc3++;
      delay(600);
    }
    if (abajo == HIGH) {
      cc3--;
      delay(600);
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
  else if (ccX == 10) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cc4++;
      delay(600);
    }
    if (abajo == HIGH) {
      cc4--;
      delay(600);
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
  if (ccX > 10) {
    if (CLA[0] == cc1 && CLA[1] == cc2 && CLA[2] == cc3 && CLA[3] == cc4) {
      lcd.setCursor(6, 3);
      lcd.print("ClAVE CORRECTA");
      delay(2000);
      ccX = 7;
      nueva = 1;
      cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0;
      lcd.clear();
    }    else if (cM[0] == cc1 && cM[1] == cc2 && cM[2] == cc3 && cM[3] == cc4) {
      lcd.setCursor(6, 3);
      lcd.print("ClAVE CORRECTA");
      delay(2000);
      cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0;
      nueva = 1;
      ccX = 7;
      lcd.clear();
    }
    else {
      lcd.setCursor(4, 3);
      lcd.print("CLAVE INCORRECTA");
      cc1 = 0; cc2 = 0; cc3 = 0; cc4 = 0;
      delay(2000);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ingrese la Clave:");
      ccX = 7;
    }
  }
  while (nueva == 1) {
    izquierda = digitalRead(Left);
    derecha = digitalRead(Right);
    lcd.setCursor(0, 0);
    lcd.print(ccX);
    lcd.setCursor(0, 0);
    lcd.print("Ingrese Clave Nueva");
    lcd.setCursor(7, 1);
    lcd.print(cc1);
    lcd.setCursor(8, 1);
    lcd.print(cc2);
    lcd.setCursor(9, 1);
    lcd.print(cc3);
    lcd.setCursor(10, 1);
    lcd.print(cc4);
    if (izquierda == HIGH && ccX == 7) {
      opcion = 0;
      nueva = 0;
      salirM = 1;
      ccX = 7;
      cc1 = 0; cc2 = 0; cc3 = 0; cc4 = 0;
      lcd.clear();
    }
    if (derecha == HIGH) {
      ccX++;
      delay(600);
    }
    if (izquierda == HIGH) {
      ccX--;
      delay(600);
    }
    if (ccX < 7) {
      ccX = 7;
    }

    if (ccX == 7) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc1++;
        delay(600);
      }
      if (abajo == HIGH) {
        cc1--;
        delay(600);
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
    else if (ccX == 8) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc2++;
        delay(600);
      }
      if (abajo == HIGH) {
        cc2--;
        delay(600);
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
    else if (ccX == 9) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc3++;
        delay(600);
      }
      if (abajo == HIGH) {
        cc3--;
        delay(600);
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
    else if (ccX == 10) {
      abajo = digitalRead(Down);
      arriba = digitalRead(Up);
      if (arriba == HIGH) {
        cc4++;
        delay(600);
      }
      if (abajo == HIGH) {
        cc4--;
        delay(600);
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
    if (ccX > 10) {
      EEPROM.put(10, cc1);
      EEPROM.put(12, cc2);
      EEPROM.put(14, cc3);
      EEPROM.put(16, cc4);
      lcd.setCursor(4, 3);
      lcd.print("CLAVE CAMBIADA");
      ccX = 7;
      cc1 = 0, cc2 = 0, cc3 = 0, cc4 = 0;
      opcion = 0;
      nueva = 0;
      salirM = 1;
      delay(2000);
      lcd.clear();
    }
  }
}
// rutina para mostrar el sub menu de Modo
void submenuM() {
  switch (cSM) {
    case 0:
      cSM = 1;
      break;
    case 1:
      if (derecha == HIGH) {
        selSM = 1;
        digitalWrite(Lampara, LOW);
        lcd.clear();
      }
      while (selSM == 1) {
        Mantenimiento();
      }
      lcd.setCursor(0, 0);
      lcd.print("      Modo     ");
      lcd.setCursor(0, 1);
      lcd.print(" >Mantenimineto      ");
      lcd.setCursor(4, 2);
      lcd.print(" Servicio            ");
      lcd.setCursor(4, 3);
      lcd.print(" Apagado             ");
      break;
    case 2:
      if (derecha == HIGH) {
        selSM = 1;
        delay(600);
        lcd.clear();
      }
      while (selSM == 1) {
        claveServicio();
      }
      lcd.setCursor(0, 0);
      lcd.print("      Modo     ");
      lcd.setCursor(0, 1);
      lcd.print(" Mantenimineto      ");
      lcd.setCursor(4, 2);
      lcd.print(" >Servicio          ");
      lcd.setCursor(4, 3);
      lcd.print(" Apagado            ");
      break;
    case 3:
      if (derecha == HIGH) {
        selSM = 1;
        delay(600);
        lcd.clear();
      }
      while (selSM == 1) {
        Apagar();
      }
      lcd.setCursor(0, 0);
      lcd.print("      Modo          ");
      lcd.setCursor(0, 1);
      lcd.print(" Mantenimineto      ");
      lcd.setCursor(4, 2);
      lcd.print(" Servicio           ");
      lcd.setCursor(4, 3);
      lcd.print(" >Apagado           ");
      break;
  }
}
//funcion para establecer el tiempo de purga
void EtiempoPurga() {
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  cTp1 = p[0]; cTp2 = p[1]; cTp3 = p[2]; cTp4 = p[3];
  lcd.setCursor(4, 0);
  lcd.print("Establezca");
  lcd.setCursor(2, 1);
  lcd.print("Tiempo de purga:");
  lcd.setCursor(11, 2);
  lcd.print(cTp1);
  lcd.setCursor(12, 2);
  lcd.print(cTp2);
  lcd.setCursor(13, 2);
  lcd.print(":");
  lcd.setCursor(14, 2);
  lcd.print(cTp3);
  lcd.setCursor(15, 2);
  lcd.print(cTp4);
  if (derecha == HIGH) {
    cXTp++;
    delay(600);
  }
  if (izquierda == HIGH) {
    cXTp--;
    delay(600);
  }

  if (cXTp < 7) {
    cXTp = 6;
  }
  if (cXTp == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTp1++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTp1--;
      delay(600);
    }
    if (cTp1 > 9) {
      cTp1 = 9;
    }
    else if (cTp1 < 1) {
      cTp1 = 0;
    }
    p[0] = cTp1;
    lcd.setCursor(cXTp + 4, 2);
    lcd.print(cTp1);
    lcd.setCursor(cXTp + 4, 3);
    lcd.print("^    ");
  }
  else if (cXTp == 8) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTp2++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTp2--;
      delay(600);
    }
    if (cTp2 > 9) {
      cTp2 = 9;
    }
    else if (cTp2 < 1) {
      cTp2 = 0;
    }
    p[1] = cTp2;
    lcd.setCursor(cXTp + 4, 2);
    lcd.print(cTp2);
    lcd.setCursor(cXTp + 3, 3);
    lcd.print(" ^   ");
  }
  else if (cXTp == 9) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTp3++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTp3--;
      delay(600);
    }
    if (cTp3 > 9) {
      cTp3 = 9;
    }
    else if (cTp3 < 1) {
      cTp3 = 0;
    }
    p[2] = cTp3;
    lcd.setCursor(cXTp + 5, 2);
    lcd.print(cTp3);
    lcd.setCursor(cXTp + 2, 3);
    lcd.print("   ^ ");
  }
  else if (cXTp == 10) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTp4++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTp4--;
      delay(600);
    }
    if (cTp4 > 9) {
      cTp4 = 9;
    }
    else if (cTp4 < 1) {
      cTp4 = 0;
    }
    p[3] = cTp4;
    lcd.setCursor(cXTp + 5, 2);
    lcd.print(cTp4);
    lcd.setCursor(cXTp + 1, 3);
    lcd.print("    ^");
  }
  if (cXTp > 10) {
    cXTp = 6;
    lcd.setCursor(4, 3);
    lcd.print("Tiempo Establecido");
    delay(2000);
    EEPROM.put(20, p[0]);
    EEPROM.put(22, p[1]);
    EEPROM.put(24, p[2]);
    EEPROM.put(26, p[3]);
    selSA = 0;
    lcd.clear();
  }
  if (izquierda == HIGH && cXTp == 6) {
    selSA = 0;
    cXTp = 6;
    cTp1 = 0; cTp2 = 0; cTp3 = 0; cTp4 = 0;
    lcd.clear();
  }
}
//funcion para establecer el tiempo post-purga:
void EtiempoPostPurga() {
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  cTpp1 = pp[0]; cTpp2 = pp[1]; cTpp3 = pp[2]; cTpp4 = pp[3];
  lcd.setCursor(4, 0);
  lcd.print("Establezca");
  lcd.setCursor(1, 1);
  lcd.print("Tiempo post-purga:");
  lcd.setCursor(11, 2);
  lcd.print(cTpp1);
  lcd.setCursor(12, 2);
  lcd.print(cTpp2);
  lcd.setCursor(13, 2);
  lcd.print(":");
  lcd.setCursor(14, 2);
  lcd.print(cTpp3);
  lcd.setCursor(15, 2);
  lcd.print(cTpp4);
  if (derecha == HIGH) {
    cXTpp++;
    delay(600);
  }
  if (izquierda == HIGH) {
    cXTpp--;
    delay(600);
  }

  if (cXTpp < 7) {
    cXTpp = 6;
  }
  if (cXTpp == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTpp1++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTpp1--;
      delay(600);
    }
    if (cTpp1 > 9) {
      cTpp1 = 9;
    }
    else if (cTpp1 < 1) {
      cTpp1 = 0;
    }
    pp[0] = cTpp1;
    lcd.setCursor(cXTpp + 4, 2);
    lcd.print(cTpp1);
    lcd.setCursor(cXTpp + 4, 3);
    lcd.print("^    ");
  }
  else if (cXTpp == 8) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTpp2++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTpp2--;
      delay(600);
    }
    if (cTpp2 > 9) {
      cTpp2 = 9;
    }
    else if (cTpp2 < 1) {
      cTpp2 = 0;
    }
    pp[1] = cTpp2;
    lcd.setCursor(cXTpp + 4, 2);
    lcd.print(cTpp2);
    lcd.setCursor(cXTpp + 3, 3);
    lcd.print(" ^   ");
  }
  else if (cXTpp == 9) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTpp3++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTpp3--;
      delay(600);
    }
    if (cTpp3 > 9) {
      cTpp3 = 9;
    }
    else if (cTpp3 < 1) {
      cTpp3 = 0;
    }
    pp[2] = cTpp3;
    lcd.setCursor(cXTpp + 5, 2);
    lcd.print(cTpp3);
    lcd.setCursor(cXTpp + 2, 3);
    lcd.print("   ^ ");
  }
  else if (cXTpp == 10) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTpp4++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTpp4--;
      delay(600);
    }
    if (cTpp4 > 9) {
      cTpp4 = 9;
    }
    else if (cTpp4 < 1) {
      cTpp4 = 0;
    }
    pp[3] = cTpp4;
    lcd.setCursor(cXTpp + 5, 2);
    lcd.print(cTpp4);
    lcd.setCursor(cXTpp + 1, 3);
    lcd.print("    ^");
  }
  if (cXTpp > 10) {
    cXTpp = 6;
    lcd.setCursor(4, 3);
    lcd.print("Tiempo Establecido");
    delay(2000);
    EEPROM.put(30, pp[0]);
    EEPROM.put(32, pp[1]);
    EEPROM.put(34, pp[2]);
    EEPROM.put(36, pp[3]);
    selSA = 0;
    lcd.clear();
  }
  if (izquierda == HIGH && cXTpp == 6) {
    selSA = 0;
    cXTpp = 6;
    cTpp1 = 0; cTpp2 = 0; cTpp3 = 0; cTpp4 = 0;
    lcd.clear();
  }
}
//Funcion para establecer el tiempo de la lampara UV
void EtiempoUV() {
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  cTUV1 = TiempoUV[0]; cTUV2 = TiempoUV[1]; cTUV3 = TiempoUV[2]; cTUV4 = TiempoUV[3];
  lcd.setCursor(5, 0);
  lcd.print("Establezca");
  lcd.setCursor(1, 1);
  lcd.print("Tiempo Lampara UV:");
  lcd.setCursor(11, 2);
  lcd.print(cTUV1);
  lcd.setCursor(12, 2);
  lcd.print(cTUV2);
  lcd.setCursor(13, 2);
  lcd.print(":");
  lcd.setCursor(14, 2);
  lcd.print(cTUV3);
  lcd.setCursor(15, 2);
  lcd.print(cTUV4);
  if (derecha == HIGH) {
    cXTUV++;
    delay(600);
  }
  if (izquierda == HIGH) {
    cXTUV--;
    delay(600);
  }

  if (cXTUV < 7) {
    cXTUV = 6;
  }
  if (cXTUV == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTUV1++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTUV1--;
      delay(600);
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
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTUV2++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTUV2--;
      delay(600);
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
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTUV3++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTUV3--;
      delay(600);
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
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cTUV4++;
      delay(600);
    }
    if (abajo == HIGH) {
      cTUV4--;
      delay(600);
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
    cXTUV = 6;
    lcd.setCursor(4, 3);
    lcd.print("Tiempo Establecido");
    delay(2000);
    EEPROM.put(40, TiempoUV[0]);
    EEPROM.put(42, TiempoUV[1]);
    EEPROM.put(44, TiempoUV[2]);
    EEPROM.put(46, TiempoUV[3]);
    selSA = 0;
    lcd.clear();
  }
  if (izquierda == HIGH && cXTUV == 6) {
    selSA = 0;
    cXTUV = 6;
    cTUV1 = 0; cTUV2 = 0; cTUV3 = 0; cTUV4 = 0;
    lcd.clear();
  }
}
//funcion para aceder al modo mantenimiento
void Mantenimiento() {
  CMotor = digitalRead(Right);
  CLampara = digitalRead(Down);
  izquierda = digitalRead(Left);
  CTomaE = digitalRead(Up);

  lcd.setCursor(0, 0);
  lcd.print("Modo de Mantenimento");
  lcd.setCursor(4, 2);
  lcd.print("Filter:");
  lcd.setCursor(11, 2);
  lcd.print(presion);
  lcd.setCursor(16, 2);
  lcd.print("UVL:");
  if (TAMUVL < 60) {
    lcd.setCursor(20, 2);
    lcd.print(TAMUVL);
    lcd.setCursor(22, 2);
    lcd.print("m");
  }
  else {
    lcd.setCursor(20, 2);
    lcd.print(TAHUVL);
    lcd.setCursor(23, 2);
    lcd.print("h");
  }
  if (CMotor == HIGH) {
    digitalWrite(Motor, LOW);
  }
  if (estadoM > 1) {
    digitalWrite(Motor, HIGH);
    estadoM = 0;
  }
  if (CMotor == HIGH) {
    estadoM++;
    delay(600);
  }
  if (CLampara == HIGH) {
    digitalWrite(Lampara, LOW);
  }
  if (estadoL > 1) {
    digitalWrite(Lampara, HIGH);
    estadoL = 0;
  }
  if (CLampara == HIGH) {
    estadoL++;
    delay(600);
  }
  if (CTomaE == HIGH) {
    digitalWrite(TomaE, LOW);
  }
  if (estadoT > 1) {
    digitalWrite(TomaE, HIGH);
    estadoT = 0;
  }
  if (CTomaE == HIGH) {
    estadoT++;
    delay(600);
  }
  if (izquierda == HIGH) {
    if (millis() >= tiempoMant + periodoMant) {
      salirMant++;
      tiempoMant = millis();
    }
  } else {
    salirMant = 0;
  }
  if (salirMant == 2) {
    selSM = 0;
    salirMant = 0;
  }
  if (CTomaE == HIGH && CMotor == HIGH) {
    lcd.setCursor(6, 3);
    lcd.print(" Vida UV Reiniciada ");
    TAHUVL = 0;
    TAMUVL = 0;
    delay(2000);
    EEPROM.put(90, TMUVL );
    EEPROM.put(95, THUVL );
    lcd.setCursor(6, 3);
    lcd.print("                    ");
  }
}
//Funcion clave de servicio
void claveServicio() {
  lcd.setCursor(1, 0);
  lcd.print("Ingrese la Clave:");
  izquierda = digitalRead(Left);
  derecha = digitalRead(Right);
  lcd.setCursor(7, 1);
  lcd.print(cS1);
  lcd.setCursor(8, 1);
  lcd.print(cS2);
  lcd.setCursor(9, 1);
  lcd.print(cS3);
  lcd.setCursor(10, 1);
  lcd.print(cS4);
  if (izquierda == HIGH && cSX == 7) {
    selSM = 0;
    cSX = 7;
    cS1 = 0; cS2 = 0; cS3 = 0; cS4 = 0;
    lcd.clear();
  }
  if (derecha == HIGH) {
    cSX++;
    delay(600);
  }
  if (izquierda == HIGH) {
    cSX--;
    delay(600);
  }
  if (cSX < 7) {
    cSX = 7;
  }

  if (cSX == 7) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cS1++;
      delay(600);
    }
    if (abajo == HIGH) {
      cS1--;
      delay(600);
    }
    if (cS1 > 9) {
      cS1 = 9;
    }
    else if (cl1 < 0) {
      cS1 = 0;
    }
    lcd.setCursor(cSX, 1);
    lcd.print(cS1);
    lcd.setCursor(cSX + 4, 2);
    lcd.print("^   ");
  }
  else if (cSX == 8) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cS2++;
      delay(600);
    }
    if (abajo == HIGH) {
      cS2--;
      delay(600);
    }
    if (cS2 > 9) {
      cS2 = 9;
    }
    else if (cS2 < 0) {
      cS2 = 0;
    }
    lcd.setCursor(cSX, 1);
    lcd.print(cS2);
    lcd.setCursor(cSX + 3, 2);
    lcd.print(" ^  ");
  }
  else if (cSX == 9) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cS3++;
      delay(600);
    }
    if (abajo == HIGH) {
      cS3--;
      delay(600);
    }
    if (cS3 > 9) {
      cS3 = 9;
    }
    else if (cS3 < 0) {
      cS3 = 0;
    }
    lcd.setCursor(cSX, 1);
    lcd.print(cS3);
    lcd.setCursor(cSX + 2, 2);
    lcd.print("  ^ ");

  }
  else if (cSX == 10) {
    abajo = digitalRead(Down);
    arriba = digitalRead(Up);
    if (arriba == HIGH) {
      cS4++;
      delay(600);
    }
    if (abajo == HIGH) {
      cS4--;
      delay(600);
    }
    if (cS4 > 9) {
      cS4 = 9;
    }
    else if (cS4 < 0) {
      cS4 = 0;
    }
    lcd.setCursor(cSX, 1);
    lcd.print(cS4);
    lcd.setCursor(cSX + 1, 2);
    lcd.print("   ^");
  }
  if (cSX > 10) {
    if (claveSE[0] == cS1 && claveSE[1] == cS2 && claveSE[2] == cS3 && claveSE[3] == cS4) {
      lcd.setCursor(6, 3);
      lcd.print("ClAVE CORRECTA");
      delay(2000);
      iniciarS = 1;
      cSX = 7;
      cS1 = 0; cS2 = 0; cS3 = 0; cS4 = 0;
    }
    else {
      lcd.setCursor(4, 3);
      lcd.print("CLAVE INCORRECTA");
      cS1 = 0; cS2 = 0; cS3 = 0; cS4 = 0;
      delay(2000);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ingrese la Clave:");
      cSX = 7;
    }
  }
  while (iniciarS == 1) {
    submenuS();
  }
}
//Funcion Menu Servicio
void submenuS() {
  izquierda = digitalRead(Left);
  arriba = digitalRead(Up);
  abajo = digitalRead(Down);
  derecha = digitalRead(Right);
  if (izquierda == HIGH) {
    selSM = 0;
    iniciarS = 0;
  }
  if (arriba == HIGH) {
    cSS--;
    delay(600);
  }
  if (abajo == HIGH) {
    cSS++;
    delay(600);
  }
  if (cSS > 3) {
    cSS = 3;
  }
  if (cSS < 1) {
    cSS = 1;
  }
  switch (cSS) {
    case 0:
      cSS = 1;
      break;
    case 1:
      if (derecha == HIGH) {
        selSS = 1;
        lcd.clear();
        delay(600);

      }
      while (selSS == 1) {
        selectED();
      }

      lcd.setCursor(0, 0);
      lcd.print("      Servicio      ");
      lcd.setCursor(0, 1);
      lcd.print(" >Sensor Flujo E/I ");
      lcd.setCursor(4, 2);
      lcd.print(" Sensor Flujo DnF ");
      lcd.setCursor(4, 3);
      lcd.print(" Sensor de Presion ");

      break;
    case 2:
      EEPROM.get(70, AJD);
      if (derecha == HIGH) {
        selSS = 1;
        lcd.clear();

      }
      while (selSS == 1) {
        sensorFlujoD();
      }

      lcd.setCursor(4, 0);
      lcd.print("Servicio");
      lcd.setCursor(0, 1);
      lcd.print(" Sensor Flujo E/D ");
      lcd.setCursor(4, 2);
      lcd.print(" >Sensor Flujo DnF ");
      lcd.setCursor(4, 3);
      lcd.print(" Sensor de Presion ");

      break;
    case 3:
      EEPROM.get(50, AJP);
      if (derecha == HIGH) {
        selSS = 1;
        lcd.clear();
      }
      while (selSS == 1) {
        sensorPresion();
      }
      lcd.setCursor(4, 0);
      lcd.print("Servicio");
      lcd.setCursor(0, 1);
      lcd.print(" Sensor Flujo E/I ");
      lcd.setCursor(4, 2);
      lcd.print(" Sensor Flujo DnF ");
      lcd.setCursor(4, 3);
      lcd.print(" >Sensor de Presion ");
      break;
  }
}
//funcion sub menu de ajuste para ExFlow/DownFlow
void selectED() {
  izquierda = digitalRead(Left);
  arriba = digitalRead(Up);
  abajo = digitalRead(Down);
  derecha = digitalRead(Right);
  if (izquierda == HIGH) {
    selSS = 0;
    lcd.clear();
    delay(600);
  }
  if (arriba == HIGH) {
    cSED--;
    delay(600);
  }
  if (abajo == HIGH) {
    cSED++;
    delay(600);
  }
  if (cSED > 2) {
    cSED = 2;
  }
  if (cSED < 1) {
    cSED = 1;
  }
  switch (cSED) {
    case 0:
      cSED = 1;
      break;
    case 1:
      EEPROM.get(60, AJE);
      if (derecha == HIGH) {
        selSED = 1;
        lcd.clear();
      }
      while (selSED == 1) {
        sensorFlujoE();
      }

      lcd.setCursor(1, 0);
      lcd.print("Seleccione Ajuste:");
      lcd.setCursor(4, 1);
      lcd.print(" >Exaust ");
      lcd.setCursor(8, 2);
      lcd.print(" In Flow ");
      lcd.setCursor(4, 3);
      lcd.print("                 ");

      break;
    case 2:
      EEPROM.get(80, AJI);
      if (derecha == HIGH) {
        selSED = 1;
        lcd.clear();
      }
      while (selSED == 1) {
        sensorFlujoIn();
      }

      lcd.setCursor(1, 0);
      lcd.print("Seleccione Ajuste:");
      lcd.setCursor(4, 1);
      lcd.print(" Exaust ");
      lcd.setCursor(8, 2);
      lcd.print(" >In Flow ");
      lcd.setCursor(4, 3);
      lcd.print("              ");

      break;
  }
}
//Funcion ajuste Exflow
void sensorFlujoE() {
  adcEI = analogRead(pin_sensorEI);
  voltajeEI = adcEI * 5 / 1023;
  variableE = voltajeEI * relvoltvarE;
  arriba = digitalRead(Up);
  izquierda = digitalRead(Left);
  abajo = digitalRead(Down);
  if (millis() >= tiempoLCD + periodoLCD) {
    tiempoLCD = millis();
    exflow = variableE + AJE ;
    lcd.setCursor(0, 0);
    lcd.print("Ajuste Exaust");
    lcd.setCursor(0, 1);
    lcd.print("Fabrica:");
    lcd.setCursor(4, 2);
    lcd.print("Actual:");
    lcd.setCursor(11, 2);
    lcd.print(exflow);
    lcd.setCursor(4, 3);
    lcd.print("Ajuste: ");

  }

  if (arriba == HIGH) {
    AJE += 0.01;
    delay(30);
  }
  if (abajo == HIGH) {
    AJE -= 0.01;
    delay(30);
  }
  lcd.setCursor(13, 3);
  lcd.print(AJE, 2);
  if (izquierda) {
    selSED = 0;
    delay(600);
    EEPROM.put(60, AJE);
    lcd.clear();
  }
}
//Funcion ajuste Downflow
void sensorFlujoD() {
  adcD = analogRead(pin_sensorD);
  voltajeEI = adcEI * 5 / 1023;
  variableD = voltajeEI * relvoltvarD;
  arriba = digitalRead(Up);
  izquierda = digitalRead(Left);
  abajo = digitalRead(Down);
  if (millis() >= tiempoLCD + periodoLCD) {
    tiempoLCD = millis();
    downflow = variableD + AJD;
    lcd.setCursor(0, 0);
    lcd.print("Ajuste DownFlow");
    lcd.setCursor(0, 1);
    lcd.print("Fabrica:");
    lcd.setCursor(4, 2);
    lcd.print("Actual:");
    lcd.setCursor(11, 2);
    lcd.print(downflow);
    lcd.setCursor(4, 3);
    lcd.print("Ajuste: ");
  }
  if (arriba == HIGH) {
    AJD += 0.01;
    delay(30);
  }
  if (abajo == HIGH) {
    AJD -= 0.01;
    delay(30);
  }
  lcd.setCursor(13, 3);
  lcd.print(AJD, 2);
  if (izquierda == HIGH) {
    selSS = 0;
    delay(600);
    lcd.clear();
    EEPROM.put(70, AJD);
  }
}
//Funcion ajuste InFlow
void sensorFlujoIn() {
  adcEI = analogRead(pin_sensorEI);
  voltajeEI = adcEI * 5 / 1023;
  variableI = voltajeEI * relvoltvarI;
  arriba = digitalRead(Up);
  izquierda = digitalRead(Left);
  abajo = digitalRead(Down);
  if (millis() >= tiempoLCD + periodoLCD) {
    inflow = variableI + AJI;
    tiempoLCD = millis();
    lcd.setCursor(0, 0);
    lcd.print("Ajuste InFlow");
    lcd.setCursor(0, 1);
    lcd.print("Fabrica:");
    lcd.setCursor(4, 2);
    lcd.print("Actual:");
    lcd.setCursor(11, 2);
    lcd.print(inflow);
    lcd.setCursor(4, 3);
    lcd.print("Ajuste: ");
  }

  if (arriba == HIGH) {
    AJI += 0.01;
    delay(30);
  }
  if (abajo == HIGH) {
    AJI -= 0.01;
    delay(30);
  }
  lcd.setCursor(13, 3);
  lcd.print(AJI, 2);
  if (izquierda == HIGH) {
    selSED = 0;
    lcd.clear();
    delay(600);
    EEPROM.put(80, AJI);
  }
}
//Funcion ajuste sensor Presion
void sensorPresion() {
  adcP = analogRead(pin_sensorP);
  voltajeP = adcP * 5 / 1023;
  variableP = voltajeP * relvoltvarP;
  presion = variableP + AJP;
  arriba = digitalRead(Up);
  izquierda = digitalRead(Left);
  abajo = digitalRead(Down);
  lcd.setCursor(0, 0);
  lcd.print("Ajuste InFlow");
  lcd.setCursor(0, 1);
  lcd.print("Fabrica:");
  lcd.setCursor(4, 2);
  lcd.print("Actual:");
  lcd.setCursor(11, 2);
  lcd.print(presion);
  lcd.setCursor(4, 3);
  lcd.print("Ajuste: ");
  if (arriba == HIGH) {
    AJP += 0.01;
    delay(30);
  }
  if (abajo == HIGH) {
    AJP -= 0.01;
    delay(30);
  }
  lcd.setCursor(13, 3);
  lcd.print(AJP, 2);
  if (izquierda == HIGH) {
    selSS = 0;
    lcd.clear();
    delay(600);
    EEPROM.put(50, AJP);
  }
}
//Funcion Para Apagar la maquina
void Apagar() {
  izquierda = digitalRead(Left);
  arriba = digitalRead(Up);
  abajo = digitalRead(Down);
  derecha = digitalRead(Right);
  if (izquierda == HIGH) {
    selSM = 0;
    cSal = 0;
    lcd.clear();
    delay(600);
  }
  if (arriba == HIGH) {
    cSal--;
    delay(600);
  }
  if (abajo == HIGH) {
    cSal++;
    delay(600);
  }
  if (cSal > 2) {
    cSal = 2;
  }
  if (cSal < 1) {
    cSal = 1;
  }
  switch (cSal) {
    case 0:
      cSal = 1;
      break;
    case 1:
      if (derecha == HIGH) {
        selSal = 1;
        lcd.clear();
        delay(500);
        digitalWrite(Lampara, HIGH);
        digitalWrite(UV, HIGH);
        digitalWrite(TomaE, HIGH);
        s1 = 0; s2 = 0; m1 = 0; m2 = 0;
      }
      while (selSal == 1) {
        TiempoPostPurga();
      }

      lcd.setCursor(1, 0);
      lcd.print("Apagar?");
      lcd.setCursor(4, 1);
      lcd.print(" >SI ");
      lcd.setCursor(8, 2);
      lcd.print(" No ");
      lcd.setCursor(4, 3);
      lcd.print("                 ");

      break;
    case 2:
      if (derecha == HIGH) {
        selSal = 0;
        lcd.clear();
        delay(600);
        selSM = 0;
      }
      lcd.setCursor(1, 0);
      lcd.print("Apagar?");
      lcd.setCursor(4, 1);
      lcd.print(" SI ");
      lcd.setCursor(8, 2);
      lcd.print(" >NO ");
      lcd.setCursor(4, 3);
      lcd.print("              ");

      break;
  }
}
//funcion tiempo post-purga
void TiempoPostPurga() {
  TMUVL = TAMUVL;
  THUVL = TAHUVL;
  EEPROM.get(30, pp[0]);
  EEPROM.get(32, pp[1]);
  EEPROM.get(34, pp[2]);
  EEPROM.get(36, pp[3]);
  digitalWrite(Motor, HIGH);
  lcd.setCursor(4, 0);
  lcd.print("Post-Purga");
  lcd.setCursor(1, 2);
  lcd.print("               ");
  tiempoActualTP = millis();
  VentAr = digitalRead(PV1);
  if (VentAr == HIGH) {
    salir = 0;
  }
  while (salir != 1) {
    VentAr = digitalRead(PV1);
    VentAr = digitalRead(PV1);
    arriba  = digitalRead(Up);
    derecha = digitalRead(Right);
    abajo = digitalRead(Down);
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
    if (pp[0] == m2 && pp[1] == m1 && pp[2] == s2 && pp[3] == s1 ) {
      digitalWrite(Motor, LOW);
      lcd.noDisplay();
      lcd.noBacklight();
      if (TAMUVL < 60) {
        EEPROM.put(90, TMUVL);
      }
      else {
        EEPROM.put(95, THUVL);
      }
      Reinicio;
    }
    lcd.setCursor(7, 1);
    lcd.print(m2);
    lcd.setCursor(8, 1);
    lcd.print(m1);
    lcd.setCursor(9, 1);
    lcd.print(":");
    lcd.setCursor(10, 1);
    lcd.print(s2);
    lcd.setCursor(11, 1);
    lcd.print(s1);
    lcd.setCursor(6, 3);
    lcd.print("                ");
    if (arriba == HIGH || abajo == HIGH || derecha == HIGH) {
      skipTPP++;
      delay(1000);
    }
    if (skipTPP == 5) {
      if (TAMUVL < 60) {
        EEPROM.put(90, TMUVL);
      }
      else {
        EEPROM.put(95, THUVL);
      }
      Reinicio;
    }
  }

  if (VentAba == LOW) {
    if ((millis() >= 600 + tiempoActual) && stparpadeo == true) {
      stparpadeo = false;
      lcd.setCursor(6, 3);
      lcd.print("Ubique el Vidrio");
      tiempoAnterior = millis();
    }
    if ((millis() >= 600 + tiempoAnterior) && stparpadeo == false) {
      stparpadeo = true;
      lcd.setCursor(6, 3);
      lcd.print("                ");
      tiempoActual = millis();
      limpiarlcd = 0;
    }
  }
}
