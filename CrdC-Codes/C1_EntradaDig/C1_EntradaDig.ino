#define LED 2
#define entrada 4

bool dato;

void setup(){
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);
  pinMode(entrada, INPUT);
}

void loop(){
  dato = digitalRead(entrada);
  
  if (dato == 1){
    digitalWrite(LED, HIGH);
    Serial.print("La lectura es de: ");
    Serial.println(dato);
  }
  
  else {
    digitalWrite(LED, LOW);
    Serial.print("La lectura es de: ");
    Serial.println(dato);
  }
}
