#include <SPI.h>          //  Libreria SPI
#include <MFRC522.h>      // Libreria  MFRC522

#define RST_PIN  22      // Pin de reset
#define SS_PIN  21      // Pin de slave select
#define robotAlert 2    // Pin donde el robot le dirá a Arduino que ya se alineo con la caja
#define pin_servo 13    // Pin donde le dirá al robot si abrir o cerrar servo
#define pin_par 15      // Pin donde le dirá por cual ruta deberá irse 

MFRC522 rfid(SS_PIN, RST_PIN);               // Objeto mfrc522 enviando pines de slave select y reset

byte LecturaUID[4];                             // Array para almacenar el UID leido

int codigo[] = {4, 16, 17, 5};
bool rA = false;
byte dato, n;
byte alto = 0;


//---------------------------setup------------------------------
void setup() {
  pinMode(robotAlert, INPUT);
  pinMode(pin_servo, OUTPUT);
  pinMode(pin_par, OUTPUT);

  digitalWrite(pin_servo,LOW);
  digitalWrite(pin_par,LOW);
  
  for(int i = 0; i < 4; i++){
    pinMode(codigo[i], OUTPUT);
    digitalWrite(codigo[i],LOW);
  }
  
  Serial.begin(115200);                       // inicializa comunicacion por monitor serie a 9600 bps
  SPI.begin();                              // inicializa bus SPI
  rfid.PCD_Init();                       // inicializa modulo lector
  Serial.println("Sistema Activado");       // Muestra texto de inicio del sistema
}


//---------------------------loop------------------------------
void loop() {
  rA = digitalRead(robotAlert);
  Serial.println(rA); 
  delay(100);
  
  if (rA == 0){
    lecturaRFID();
  
    
  }

  else{
    digitalWrite(pin_servo, LOW);
    Serial.println("Gripper abierto");
    delay(2000);
  }

  
  /*n++;
  if (n >7)n=0;
  delay(1000);*/

      
  
      
      
      
      
   
}


 //---------------------------functions------------------------------

void lecturaRFID(){
  if (rfid.PICC_IsNewCardPresent()){
    if (rfid.PICC_ReadCardSerial()){
      //Serial.print("UID: ");
      for (int i = 0; i < rfid.uid.size; i++){
        if (rfid.uid.uidByte[i] < 0x10){
          Serial.print(" 0");
        }
        else{
          Serial.print(" ");
        }
        LecturaUID[i] = rfid.uid.uidByte[i];       // almacena en array el byte del UID leido      
        Serial.print(LecturaUID[i],HEX);
      }
      Serial.println();

      rfid.PICC_HaltA();

      selectNibble(2);
    }
  }
}

void selectNibble(int num){
  if (num == 0){
    dato = LecturaUID[0] >> 4;
  }
  else if (num == 1){
    dato = LecturaUID[0] & 0xF;
  }
  else if (num == 2){
    dato = LecturaUID[1] >> 4;
  }
  else if (num == 3){
    dato = LecturaUID[1] & 0xF;
  }
  else if (num == 4){
    dato = LecturaUID[2] >> 4;
  }
  else if (num == 5){
    dato = LecturaUID[2] & 0xF;
  }
  else if (num == 6){
    dato = LecturaUID[3] >> 4;
  }
  else if (num == 7){
    dato = LecturaUID[3] & 0xF;
  }
  Serial.println(dato);
  codificador(dato);    
}

/*
boolean comparaUID(byte lectura[],byte tarjeta[]) // funcion comparaUID
{
  for (byte i=0; i < 4; i++){         // bucle recorre de a un byte por vez el UID
  if(lectura[i] != tarjeta[i])        // si byte de UID leido es distinto a tarjeta
    return(false);          // retorna falso
  }
  return(true);           // si los 4 bytes coinciden retorna verdadero
}

void tomarTarjeta() {
  servoGrip.write(90);         //Cerrar gripper
}
*/

void codificador(int code){
  if (code == 0){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    
    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }
  
  else if (code == 1){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 2){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    
    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }
  
  else if(code == 3){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[1],HIGH);

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 4){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[2],HIGH);
    
    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }
  
  else if(code == 5){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[2],HIGH);

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 6){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[2],HIGH);
    
    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }

  else if(code == 7){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[2],HIGH);
    
    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 8){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }

  else if(code == 9){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 10){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }

  else if(code == 11){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 12){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[2],HIGH);
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }

  else if(code == 13){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[2],HIGH);
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  else if(code == 14){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[2],HIGH);
    digitalWrite(codigo[3],HIGH);

    digitalWrite(pin_par, HIGH);
    Serial.println("Ruta par");
  }

  else if(code == 15){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],HIGH);
    }

    digitalWrite(pin_par, LOW);
    Serial.println("Ruta impar");
  }

  digitalWrite(pin_servo, HIGH);
  Serial.println("Gripper cerrado");
  delay(2000);
}
