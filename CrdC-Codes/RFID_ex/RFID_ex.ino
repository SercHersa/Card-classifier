#include <SPI.h>          //  Libreria SPI
#include <MFRC522.h>      // Libreria  MFRC522
#include <Servo.h>        // Libreria  SERVO

#define RST_PIN  9      // Pin de reset
#define SS_PIN  10      // Pin de slave select
#define robotAlert 6    // Pin donde el robot le dirá a Arduino que ya se alineo con la caja

MFRC522 mfrc522(SS_PIN, RST_PIN);               // Objeto mfrc522 enviando pines de slave select y reset

byte LecturaUID[4];                             // Array para almacenar el UID leido
byte tarjeta1[4]= {0xA6, 0xAC, 0x15, 0x30} ;       // Tarjeta número 1
byte tarjeta2[4]= {0xF6, 0xC6, 0x2F, 0x30} ;       // Tarjeta número 2
byte tarjeta3[4]= {0xD6, 0x4D, 0x2A, 0x30} ;       // Tarjeta número 3
byte tarjeta4[4]= {0x06, 0x4C, 0x32, 0x30} ;       // Tarjeta número 4
byte tarjeta5[4]= {0x16, 0xBF, 0x3B, 0x32} ;       // Tarjeta número 5
byte tarjeta6[4]= {0xD6, 0xED, 0x6F, 0x33} ;       // Tarjeta número 6
byte tarjeta7[4]= {0xA6, 0x73, 0x4A, 0x32} ;       // Tarjeta número 7
byte tarjeta8[4]= {0xD6, 0xBF, 0x1B, 0x30} ;       // Tarjeta número 8
byte tarjeta9[4]= {0xC6, 0xC8, 0x30, 0x30} ;       // Tarjeta número 9
byte tarjeta10[4]= {0xE3, 0xAA, 0xE4, 0x16} ;       // Tarjeta número 10

Servo servoGrip;                                // Asocia la libreria servo a servoGrip

int codigo[] = {2, 3, 4, 5};
bool rA = false;


//---------------------------setup------------------------------
void setup() {
  servoGrip.attach(7);                        // Asocia el  pin 7 al servo y lleva a 170 grados
  servoGrip.write(180);                     //Abrir gripper 

  for(int i = 0; i < 4; i++){
    pinMode(codigo[i], OUTPUT);
    digitalWrite(codigo[i],LOW);
  }

  pinMode(robotAlert, INPUT);
    
  Serial.begin(9600);                       // inicializa comunicacion por monitor serie a 9600 bps
  SPI.begin();                              // inicializa bus SPI
  mfrc522.PCD_Init();                       // inicializa modulo lector
  Serial.println("Sistema Activado");       // Muestra texto de inicio del sistema
}

//---------------------------loop------------------------------
void loop() {
  rA = digitalRead(robotAlert);
  
  while (rA == false){
    if ( ! mfrc522.PICC_IsNewCardPresent())   // si no hay una tarjeta presente
    return;                                 // retorna al loop esperando por una tarjeta
  
    if ( ! mfrc522.PICC_ReadCardSerial())     // si no puede obtener datos de la tarjeta
      return;                                 // retorna al loop esperando por otra tarjeta
    
    Serial.print("UID:");                     // muestra texto UID:
    for (byte i = 0; i < 4; i++) {            // bucle recorre de a un byte por vez el UID
      if (mfrc522.uid.uidByte[i] < 0x10){     // si el byte leido es menor a 0x10
        Serial.print(" 0");                   // imprime espacio en blanco y numero cero
      }
    
      else{                                   // sino
        Serial.print(" ");                      // imprime un espacio en blanco
      }
    
      Serial.print(mfrc522.uid.uidByte[i], HEX);    // imprime el byte del UID leido en hexadecimal
      LecturaUID[i] = mfrc522.uid.uidByte[i];       // almacena en array el byte del UID leido      
    }
          
    Serial.print("\t");         // imprime un espacio de tabulacion             
                    
    
    if(comparaUID(LecturaUID, tarjeta1)){       // llama a funcion comparaUID con tarjeta1
      Serial.println("Tarjeta 1");              // si retorna verdadero muestra texto  de tarjeta 1
      coding(1);                                // Envía 1 en binario
    }
  
    else if(comparaUID(LecturaUID, tarjeta2)){  // llama a funcion comparaUID con tarjeta2
      Serial.println("Tarjeta 2");              // si retorna verdadero muestra texto  de tarjeta 2
      coding(2);                                // Envía 2 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta3)){  // llama a funcion comparaUID con tarjeta3
      Serial.println("Tarjeta 3");              // si retorna verdadero muestra texto  de tarjeta 3
      coding(3);                                // Envía 3 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta4)){  // llama a funcion comparaUID con tarjeta4
      Serial.println("Tarjeta 4");              // si retorna verdadero muestra texto  de tarjeta 4
      coding(4);                                // Envía 4 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta5)){  // llama a funcion comparaUID con tarjeta5
      Serial.println("Tarjeta 5");              // si retorna verdadero muestra texto  de tarjeta 5
      coding(5);                                // Envía 5 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta6)){  // llama a funcion comparaUID con tarjeta6
      Serial.println("Tarjeta 6");              // si retorna verdadero muestra texto  de tarjeta 6
      coding(6);                                // Envía 6 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta7)){  // llama a funcion comparaUID con tarjeta7
      Serial.println("Tarjeta 7");              // si retorna verdadero muestra texto  de tarjeta 7
      coding(7);                                // Envía 7 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta8)){  // llama a funcion comparaUID con tarjeta8
      Serial.println("Tarjeta 8");              // si retorna verdadero muestra texto  de tarjeta 8
      coding(8);                                // Envía 8 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta9)){  // llama a funcion comparaUID con tarjeta9
      Serial.println("Tarjeta 9");              // si retorna verdadero muestra texto  de tarjeta 9
      coding(9);                                // Envía 9 en binario
    }

    else if(comparaUID(LecturaUID, tarjeta10)){   // llama a funcion comparaUID con tarjeta10
      Serial.println("Tarjeta 10");               // si retorna verdadero muestra texto  de tarjeta 10
      coding(10);                                 // Envía 10 en binario
    }
  
    else {          // si retorna falso
      Serial.println("No Registrado");            // muestra texto equivalente a acceso denegado          
      coding(0);                                  // Envía 0 en binario
    }  
  
    mfrc522.PICC_HaltA();                         // detiene comunicacion con tarjeta  
    
  }
  
  coding(15);
  Serial.println("Robot llegó");
  delay(2000);
  rA == false;
  
  
  
}

//---------------------------functions------------------------------
void tomarTarjeta() {
  servoGrip.write(135);         //Cerrar gripper
}


boolean comparaUID(byte lectura[],byte tarjeta[]) // funcion comparaUID
{
  for (byte i=0; i < 4; i++){         // bucle recorre de a un byte por vez el UID
  if(lectura[i] != tarjeta[i])        // si byte de UID leido es distinto a tarjeta
    return(false);          // retorna falso
  }
  return(true);           // si los 4 bytes coinciden retorna verdadero
}

void coding(int code){
  if (code == 0){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    servoGrip.write(180);       //Abrir gripper
  }
  
  else if (code == 1){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 2){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }
  
  else if(code == 3){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[1],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 4){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[2],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }
  
  else if(code == 5){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[2],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 6){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[2],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 7){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[2],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 8){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[3],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 9){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[0],HIGH);
    digitalWrite(codigo[3],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 10){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],LOW);
    }
    digitalWrite(codigo[1],HIGH);
    digitalWrite(codigo[3],HIGH);
    tomarTarjeta();                       // funcion para tomar tarjeta
  }

  else if(code == 15){
    for(int i = 0; i < 4; i++){
      digitalWrite(codigo[i],HIGH);
    }
    servoGrip.write(180);               //Abrir gripper y soltar tarjeta
  }
}
