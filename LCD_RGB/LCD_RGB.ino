#include <LiquidCrystal.h>

int PR = 0, PB = 0, PG = 0, CR = 22, CB = 23, CG = 24;
int count = 0;
int red = 10, blue = 11, green = 12;
float R = 0, G = 0, B = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
void setup() {
  lcd.begin(16,2);
  lcd.print("Vizualizando");
  delay(2000);
  lcd.clear();

  pinMode(CR, INPUT);
  pinMode(CB, INPUT);
  pinMode(CG, INPUT);

  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  
  analogWrite(blue,125);
  analogWrite(green,87);
  analogWrite(red,145);
}

void loop() {
  PB = digitalRead(CB);
  PG = digitalRead(CG); 
  PR = digitalRead(CR);
  lcd.setCursor(0,0);
  lcd.print(R,0);
  lcd.setCursor(5,0);
  lcd.print(G,0);
  lcd.setCursor(10,0);
  lcd.print(B,0);
  if(PB == HIGH){
    B++;
    delay(50);
    analogWrite(red,R);
  }
  if(PG == HIGH){
    G++;
    delay(50);
    analogWrite(green,G);
  }
  if(PR == HIGH){
    R++;
    delay(50);
    analogWrite(blue,B);
  }
  
}
