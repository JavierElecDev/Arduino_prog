int valor1,valor2,entrada1 = 2,entrada2 = 3;
int valor3,valor4,entrada3 = 4,entrada4 = 5;
int valor5,entrada5 =6;
void setup() {
Serial.begin(9600);
pinMode(entrada1, INPUT);
pinMode(entrada2, INPUT);
pinMode(entrada3, INPUT);
pinMode(entrada4, INPUT);
pinMode(entrada5, INPUT);
}

void loop() {
 valor1 = digitalRead(entrada1);
 valor2 = digitalRead(entrada2);
 valor3 = digitalRead(entrada3);
 valor4 = digitalRead(entrada4);
 valor5 = digitalRead(entrada5);
 if(valor1 == HIGH){
  Serial.println("1");
  delay(300);
 }
 else if(valor2 == HIGH){
   Serial.println("2");
   delay(300);
 }
 else if(valor3 == HIGH){
   Serial.println("3");
   delay(300);
 }
 else if(valor4 == HIGH){
   Serial.println("4");
   delay(300);
 }
 else if(valor5 == HIGH){
   Serial.println("5");
   delay(300);
 }
}
