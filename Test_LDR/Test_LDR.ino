/* Prog avec generic ESP8266 et appuis sur bouton flash si necessaire */
const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
 
void setup() 
{
    Serial.begin(9600);
 
    pinMode(LDR, INPUT);
    pinMode(BUTTON, INPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}
 
void loop()
{
    Serial.print("LDR: ");
    Serial.println(analogRead(LDR)); // vers 0 = sombre
    Serial.print("BUTTON: ");
    Serial.println(digitalRead(BUTTON));
 
//    analogWrite(RED, random(0,1023));
//    analogWrite(GREEN, random(0,1023));
//    analogWrite(BLUE, random(0,1023));
    
if (analogRead(LDR)>100) {
  analogWrite(BLUE, 1024);
  Serial.println("1");
  delay(500);
}
else {
  analogWrite(BLUE, 0);
  Serial.println("0");
}


    delay(5);
}
