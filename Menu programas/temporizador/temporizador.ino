#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
int p[4] = {1, 0, 0, 0};
int s1, s2, m1, m2, salir;
int periodo = 1000;
unsigned long tiempoActual = 0;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(16, 4);

}

void loop() {
tiempoActual = millis();
  while (salir != 1) {
    if (millis() >= tiempoActual + periodo) {
      s1++;
      tiempoActual = millis();
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
      lcd.setCursor(0, 1);
      lcd.print("contador finalizado");
    }
    if (p[0] == m2 && p[1] == m1 && p[2] == s2 && p[3] == s1 ) {
      salir = 1;
    }
    lcd.setCursor(0, 0);
    lcd.print(m2);
    lcd.setCursor(1, 0);
    lcd.print(m1);
    lcd.setCursor(2, 0);
    lcd.print(":");
    lcd.setCursor(3, 0);
    lcd.print(s2);
    lcd.setCursor(4, 0);
    lcd.print(s1);
  }
}
