#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#define pin_sensor A5 //Pin del sensor
float adc; //Variable para obtener los valores en el 1 paso
float voltaje; //Variable para obtener el voltaje en el 2 paso
float variable; //Variable final del sensor en el 3 paso
float rel_voltaje_variable = 0.1; //Relación Voltaje/Variable del sensor
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
lcd.begin(20,4);

}

void loop() {
  //Paso 1, conversión ADC de la lectura del pin analógico
  adc = analogRead(pin_sensor);
  lcd.setCursor(0,0);
  lcd.print(adc);
 
  //Paso 2, obtener el voltaje
  voltaje = adc * 5 / 1023;
  lcd.setCursor(0,1);
  lcd.println(voltaje);
 
  //Paso 3, obtener la variable de medida del sensor
  variable = voltaje * rel_voltaje_variable;
  lcd.setCursor(0,2);
  lcd.print(variable);
 
  delay(1000);

}
