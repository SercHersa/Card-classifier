int numero[] = {4, 16, 17, 5};

void setup() {
  Serial.begin(115200);                       // inicializa comunicacion por monitor serie a 9600 bps

  for(int i = 0; i < 4; i++){
    pinMode(numero[i], OUTPUT);
    digitalWrite(numero[i],LOW);
  }

}

void loop() {
  for(int i = 0; i < 16; i++){
    codificador(i);
    delay(1000);
  }

  

}

void codificador(int dato){
  if (dato == 0){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
  }

  else if (dato == 1){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
  }

  else if(dato == 2){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[1],HIGH);
  }
  
  else if(dato == 3){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
    digitalWrite(numero[1],HIGH);
  }

  else if(dato == 4){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[2],HIGH);
  }
  
  else if(dato == 5){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
    digitalWrite(numero[2],HIGH);
  }

  else if(dato == 6){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[1],HIGH);
    digitalWrite(numero[2],HIGH);
  }

  else if(dato == 7){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
    digitalWrite(numero[1],HIGH);
    digitalWrite(numero[2],HIGH);
  }

  else if(dato == 8){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 9){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 10){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[1],HIGH);
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 11){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
    digitalWrite(numero[1],HIGH);
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 12){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[2],HIGH);
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 13){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[0],HIGH);
    digitalWrite(numero[2],HIGH);
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 14){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],LOW);
    }
    digitalWrite(numero[1],HIGH);
    digitalWrite(numero[2],HIGH);
    digitalWrite(numero[3],HIGH);
  }

  else if(dato == 15){
    for(int i = 0; i < 4; i++){
      digitalWrite(numero[i],HIGH);
    }
  }
}
