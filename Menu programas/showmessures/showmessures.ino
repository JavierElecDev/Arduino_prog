#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#define pin_sensorED A5// pin sensor MAF Exaust
#define pin_sensorI A6// pin sensor MAF DownFlow
#define pin_sensorP A7// pin sensor Presion
float adcED, adcI, adcP; //Variable para obtener los valores del conversor AD
float voltajeED, voltajeI, voltajeP; //Variable para obtener el voltaje de salida para el sensor
float variableE, variableD, variableI, variableP; //Variable final del sensor
float relvoltvarE = 100.00,relvoltvarD = 100.00, relvoltvarI = 100.00, relvoltvarP = 100.00; //Relación Voltaje/Variable del sensor
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(20, 4);

}

void loop() {
  adcED = analogRead(pin_sensorED);
  adcI = analogRead(pin_sensorI);
  adcP = analogRead(pin_sensorP);
  voltajeED = adcED * 5 / 1023;
  voltajeI = adcI * 5 / 1023;
  voltajeP = adcP * 5 / 1023;
  variableE = voltajeED * relvoltvarE;
  variableD = voltajeED * relvoltvarD;
  variableI = voltajeI * relvoltvarI;
  variableP = voltajeP * relvoltvarP;

  lcd.setCursor(0, 0);
  lcd.print("ExF:");
  lcd.setCursor(4, 0);
  lcd.print(variableE);

  lcd.setCursor(0, 1);
  lcd.print("InF:");
  lcd.setCursor(4, 1);
  lcd.println(variableI);

  lcd.setCursor(0, 2);
  lcd.print("Filter:");
  lcd.setCursor(7, 2);
  lcd.print("5%");
  delay(1000);

}
