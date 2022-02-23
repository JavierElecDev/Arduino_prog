#include <EEPROM.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int CLA[4] = {1, 8, 7, 4};
int p[4] = {0, 0, 3, 0};
int pp[4] = {0, 0, 2, 2};
int TiempoUV[4] = {0, 0, 2, 0};
float AJP = 0, AJE = 0, AJD = 0, AJI = 0;
int TMUVL = 0, THUVL = 0;
void setup() {

  EEPROM.put(90, TMUVL);
  EEPROM.put(95, THUVL);
  /*  EEPROM.put(50, AJP);
    EEPROM.put(60, AJE);
    EEPROM.put(70, AJD);
    EEPROM.put(80, AJI);
    /* EEPROM.put(20, p[0]);
    EEPROM.put(22, p[1]);
    EEPROM.put(24, p[2]);
    EEPROM.put(26, p[3]);

    EEPROM.put(30, pp[0]);
    EEPROM.put(32, pp[1]);
    EEPROM.put(34, pp[2]);
    EEPROM.put(36, pp[3]);

    EEPROM.put(40, TiempoUV[0]);
    EEPROM.put(42, TiempoUV[1]);
    EEPROM.put(44, TiempoUV[2]);
    EEPROM.put(46, TiempoUV[3]);*/
  lcd.begin(16, 4);
}

void loop() {

    lcd.setCursor(0, 0);
  lcd.print(EEPROM.get(90, TMUVL ));
  lcd.setCursor(0, 1);
  lcd.print(EEPROM.get(95, THUVL ));
  /* lcd.setCursor(0, 0);
    lcd.print(EEPROM.get(50, AJP ));
    lcd.setCursor(0, 1);
    lcd.print(EEPROM.get(60, AJE ));
    lcd.setCursor(4, 2);
    lcd.print(EEPROM.get(70, AJD ));
    lcd.setCursor(6, 0);
    lcd.print(EEPROM.get(80, AJI ));
    /*lcd.setCursor(8, 0);
     lcd.print(EEPROM.get(18, N4 ));
     lcd.setCursor(10, 0);
     lcd.print(EEPROM.get(20, N5 ));
     lcd.setCursor(12, 0);
     lcd.print(EEPROM.get(22, N6 ));
     lcd.setCursor(14, 0);
     lcd.print(EEPROM.get(24, N7 ));
     lcd.setCursor(4, 1);
     lcd.print(EEPROM.get(26, N8 ));
     lcd.setCursor(6, 1);
     lcd.print(EEPROM.get(28, N9 ));*/
}
