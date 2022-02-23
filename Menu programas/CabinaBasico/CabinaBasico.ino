#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#define pin_sensor A5// pin sensor MAF
float adc;// variable paraobtener lectura del sensor
boolean stparpadeo = true;
unsigned long tiempoAnterior = 0, tiempoActual = 0; // variables para el parpadeo de mensaje de la posicion del vidrio
int limpiarlcd = 0 , ecribirlcd = 0, limpventAR = 0, limpventAba; // variable para el refresco de la pantalla LCD 20x04
int Up = 13 , Down = 12, Right = 10, Left = 11;//Pines para los pulsadores
int arriba, abajo, derecha, izquierda;//Flehas panel frontal
int Motor = 7, Lampara = 6, UV = 5, TomaE = 4;//PinesPotencia
int CMotor = 0, CLampara = 0, CUV = 0, CTomaE = 0;//Lectura para activar Potencia
int AMotor = 22, ALampara = 23, AUV = 24, ATomaE = 25;//Entradas pulsadores para control potencia
int PV1 = 9, PV2 = 8; // Posicion de la ventana
int encender = 1, estadoM, estadoL, estadoUV, estadoT, VentAr = 0, VentAba = 0;
// Constructor de la librería de LCD 20x04
// Aqui se configuran los pines asignados a la pantalla del PCF8574
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()
{
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

  //Potencia
  pinMode(AMotor, INPUT);
  pinMode(ALampara, INPUT);
  pinMode(AUV, INPUT);
  pinMode(ATomaE, INPUT);
  //pines para activar el modulo SSR
  pinMode(Motor, OUTPUT);
  pinMode(Lampara, OUTPUT);
  pinMode(UV, OUTPUT);
  pinMode(TomaE, OUTPUT);
  //estado incial para logica inversa modulos SSR
  digitalWrite(Motor, HIGH);
  digitalWrite(Lampara, HIGH);
  digitalWrite(UV, HIGH);
  digitalWrite(TomaE, HIGH);
  //Pos Ventana sensado de posicion
  pinMode(PV1, INPUT);
  pinMode(PV2, INPUT);
}

void loop()
{
  //Control Partes de potencia
  CMotor = digitalRead(AMotor);
  CLampara = digitalRead(ALampara);
  CTomaE = digitalRead(ATomaE);
  CUV = digitalRead(AUV);
  //lectura pocision ventana
  VentAr = digitalRead(PV1);
  VentAba = digitalRead(PV2);
  //activar motor
  if (CMotor == HIGH) {
    digitalWrite(Motor, LOW);
  }
  if (estadoM > 1) {
    digitalWrite(Motor, HIGH);
    estadoM = 0;
  }
  if (CMotor == HIGH) {
    estadoM++;
    delay(300);
    lcd.clear();
  }
  //activar Toma Electrica
  if (CTomaE == HIGH) {
    digitalWrite(TomaE, LOW);
  }
  if (estadoT > 1) {
    digitalWrite(TomaE, HIGH);
    estadoT = 0;
  }
  if (CTomaE == HIGH) {
    estadoT++;
    delay(300);
  }
  //pos ventana
  if (VentAba == LOW && VentAr == LOW) {
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
      if ((millis() >= 1000 + tiempoActual) && stparpadeo == true) {
        stparpadeo = false;
        lcd.setCursor(0, 0);
        lcd.print("     ALERTA       ");
        lcd.setCursor(0, 1);
        lcd.print("Ubique el Vidrio");
        lcd.setCursor(0, 2);
        lcd.print("                  ");
        lcd.setCursor(0, 3);
        lcd.print("                  ");
        lcd.clear();
        tiempoAnterior = millis();
      }
      if ((millis() >= 1000 + tiempoAnterior) && stparpadeo == false) {
        stparpadeo = true;
        lcd.setCursor(0, 0);
        lcd.print("     ALERTA       ");
        lcd.setCursor(0, 1);
        lcd.print("Ubique el Vidrio");
        lcd.setCursor(0, 2);
        lcd.print("                  ");
        lcd.setCursor(0, 3);
        lcd.print("                  ");
        tiempoActual = millis();
        limpiarlcd = 0;
      }
    }
  }
  if (VentAr == HIGH) {  // pos ventana arriba control lampara normal
    //activar lampara
    limpventAR = 1;
    limpventAR = 0;
    if (limpventAR == 1) {
      lcd.clear();
      delay(200);
      limpventAR = 0;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    Unicamente    ");
      lcd.setCursor(0, 1);
      lcd.print("    Luz Blanca    ");
      lcd.setCursor(0, 2);
      lcd.print("                  ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
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
      delay(300);
    }
  }
  if (VentAba == HIGH) { //pos ventana abajo control lampara uv y normal
    //activar lampara
    limpventAba = 1;
    limpventAba = 0;
    if (limpventAba == 1) {
      lcd.clear();
      delay(200);
      limpventAba = 0;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Luz Ultravioleta ");
      lcd.setCursor(0, 1);
      lcd.print("   Luz Blanca   ");
      lcd.setCursor(0, 2);
      lcd.print("                ");
      lcd.setCursor(0, 3);
      lcd.print("                  ");
    }
    if (CUV == HIGH) {
      digitalWrite(UV, LOW);
    }
    if (estadoUV > 1) {
      digitalWrite(UV, HIGH);
      estadoUV = 0;
    }
    if (CUV == HIGH) {
      estadoUV++;
      delay(300);
    }
  }
}
